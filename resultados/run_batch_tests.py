import os
import subprocess
import time
import re # Módulo para Expressões Regulares

# --- CONFIGURATION FIXA ---
EXECUTABLE_NAME = "TP2"
EXECUTABLE_DIR = "cmake-build-debug"

# Lista de identificadores de arquivo a testar (sem a extensão)
FILE_CHOICES = ["5", "20", "50", "100", "250", "500"]

# Parâmetros de Controlo
NUM_RUNS = 15
NUM_ITERATIONS = 30 # Iterações por Temperatura (Iter por Temp)
ALGORITHM_CHOICE = 'r' # Recristalização Simulada
PARAM_SOURCE = '2' # MODO MANUAL (opção 2 no C)

OUTPUT_FILE = "resultados_sa_exaustivo.txt"

# --- CONFIGURAÇÕES DE TESTE (Plano Exaustivo: 16 Cenários) ---
# Tmax, Tmin, alpha, Vizinhanca (1 ou 2), Aceita Igual (0 ou 1)
TEST_SCENARIOS = [
    # Cenários SA1.x: Vizinhança 1 (viz: 1)
    # Cenários ordenados do Pior/Mais Rápido para o Melhor/Mais Lento

    # 1. MÁXIMA VELOCIDADE (Pior Qualidade Esperada)
    {"id": "SA1.1_VelocMax", "Tmax": 10.0, "Tmin": 1.0, "alpha": 0.90, "viz": 1, "igual": 0},

    # 2. BASE (Referência)
    {"id": "SA1.2_Base", "Tmax": 100.0, "Tmin": 1.0, "alpha": 0.99, "viz": 1, "igual": 0},

    # 3. Aceita Igual na Base
    {"id": "SA1.3_BaseIgual", "Tmax": 100.0, "Tmin": 1.0, "alpha": 0.99, "viz": 1, "igual": 1},

    # 4. Isolar Tmax Alto (Mais Exploração Inicial)
    {"id": "SA1.4_TmaxAlto", "Tmax": 1000.0, "Tmin": 1.0, "alpha": 0.99, "viz": 1, "igual": 0},

    # 5. Rigor Intermédio (alpha Lento)
    {"id": "SA1.5_RigorLento", "Tmax": 500.0, "Tmin": 1.0, "alpha": 0.999, "viz": 1, "igual": 0},

    # 6. QUALIDADE MÁXIMA (Melhor Qualidade Esperada)
    {"id": "SA1.6_RigorMax", "Tmax": 1000.0, "Tmin": 0.01, "alpha": 0.999, "viz": 1, "igual": 0},

    # 7. Aceita Igual no Rigor
    {"id": "SA1.7_RigorMaxIgual", "Tmax": 1000.0, "Tmin": 0.01, "alpha": 0.999, "viz": 1, "igual": 1},


    # Cenários SA2.x: Vizinhança 2 (viz: 2) - Réplica para Comparação

     # 1. MÁXIMA VELOCIDADE (Pior Qualidade Esperada)
    {"id": "SA2.1_VelocMax", "Tmax": 10.0, "Tmin": 1.0, "alpha": 0.90, "viz": 2, "igual": 0},

    # 2. BASE (Referência)
    {"id": "SA2.2_Base", "Tmax": 100.0, "Tmin": 1.0, "alpha": 0.99, "viz": 2, "igual": 0},

    # 3. Aceita Igual na Base
    {"id": "SA2.3_BaseIgual", "Tmax": 100.0, "Tmin": 1.0, "alpha": 0.99, "viz": 2, "igual": 1},

    # 4. Isolar Tmax Alto (Mais Exploração Inicial)
    {"id": "SA2.4_TmaxAlto", "Tmax": 1000.0, "Tmin": 1.0, "alpha": 0.99, "viz": 2, "igual": 0},

    # 5. Rigor Intermédio (alpha Lento)
    {"id": "SA2.5_RigorLento", "Tmax": 500.0, "Tmin": 1.0, "alpha": 0.999, "viz": 2, "igual": 0},

    # 6. QUALIDADE MÁXIMA (Melhor Qualidade Esperada)
    {"id": "SA2.6_RigorMax", "Tmax": 1000.0, "Tmin": 0.01, "alpha": 0.999, "viz": 2, "igual": 0},

    # 7. Aceita Igual no Rigor
    {"id": "SA2.7_RigorMaxIgual", "Tmax": 1000.0, "Tmin": 0.01, "alpha": 0.999, "viz": 2, "igual": 1},
]


# --- SCRIPT LOGIC ---

def compile_project():
    """Skipping compilation as TP2.exe is already built."""
    print("--- 1. COMPILATION SKIPPED ---")
    print(f"Assuming executable is already built at: {EXECUTABLE_DIR}/{EXECUTABLE_NAME}.exe")
    return True

def parse_output(output_text):
    """
    Usa Expressões Regulares para extrair os valores.

    COMPENSAÇÃO: Assumimos que o código C TROCA OS RÓTULOS (Media de Distancias no C é o MBF, MBF no C é a Média).
    Retorna: (MEDIA REAL, MBF REAL)
    """

    # 1. Extrair os valores com os seus rótulos originais do C
    # Lemos o valor da linha "Media de Distancias" (que na verdade é o MBF)
    media_match = re.search(r"Media de Distancias: ([\d\.]+)", output_text, re.DOTALL)
    # Lemos o valor da linha "MBF" (que na verdade é a Média)
    mbf_match = re.search(r"MBF: ([\d\.]+)", output_text, re.DOTALL)

    media_dist_c = float(media_match.group(1)) if media_match else None
    mbf_c = float(mbf_match.group(1)) if mbf_match else None

    # 2. INVERTER A ATRIBUIÇÃO para a saída Python (logicamente correta)
    # Media Real é o que o C chama de MBF
    real_media_dist = mbf_c
    # MBF Real é o que o C chama de Media de Distancias
    real_mbf_value = media_dist_c

    return real_mbf_value, real_media_dist

def run_test_batch_manual():
    """Executa o programa para cada cenário de teste e coleta os resultados em grupos verticais."""

    executable_name_only = EXECUTABLE_NAME + ".exe"
    executable_path_full = os.path.join(EXECUTABLE_DIR, executable_name_only)

    if not os.path.exists(executable_path_full):
        print(f"ERROR: Executable '{executable_name_only}' not found at '{executable_path_full}'.")
        print("Please ensure you have built the project manually.")
        return

    original_cwd = os.getcwd()

    # Dicionário para armazenar todos os resultados: {file_choice: [(media1, mbf1), (media2, mbf2), ...]}
    # O índice do array interno corresponde ao índice do TEST_SCENARIOS
    all_results = {f: [] for f in FILE_CHOICES}

    # --- Alterar o diretório de trabalho ---
    try:
        os.chdir(EXECUTABLE_DIR)
        print(f"Changed directory to: {os.getcwd()}")
    except OSError as e:
        print(f"FATAL ERROR: Could not change directory to '{EXECUTABLE_DIR}'. {e}")
        return

    # --- Loop Principal: Iterar sobre Cenários ---
    for scenario_index, scenario in enumerate(TEST_SCENARIOS):
        s = scenario
        print("\n" + "#"*80)
        print(f"INICIANDO CENÁRIO [{scenario_index+1}/{len(TEST_SCENARIOS)}]: {s['id']} | Tmax: {s['Tmax']}, Tmin: {s['Tmin']}, alpha: {s['alpha']}, Viz: {s['viz']}, Igual: {s['igual']}")
        print("#"*80)

        # --- Loop Secundário: Iterar sobre Arquivos ---
        for file_choice in FILE_CHOICES:
            test_name = f"tourism_{file_choice}.txt"

            # Sequência de Input para o Subprocesso (Modo Manual: opção 2)
            input_sequence = (
                f"{file_choice}\n"
                f"{NUM_RUNS}\n"
                f"{NUM_ITERATIONS}\n"
                f"{ALGORITHM_CHOICE}\n"
                f"{PARAM_SOURCE}\n"
                f"{s['Tmax']}\n"
                f"{s['Tmin']}\n"
                f"{s['alpha']}\n"
                f"{s['viz']}\n"
                f"{s['igual']}\n"
            )

            input_data = input_sequence.encode('utf-8')
            media_real = "N/A"
            mbf_real = "N/A"

            try:
                start_time = time.time()
                result = subprocess.run(
                    [executable_name_only],
                    input=input_data,
                    capture_output=True,
                    check=True,
                    timeout=300 # Aumentado o timeout para 5 minutos
                )
                end_time = time.time()
                duration = end_time - start_time

                output_stdout = result.stdout.decode('utf-8', errors='ignore')

                # 1. Parsing do Output (Retorna (MEDIA_REAL, MBF_REAL))
                media_real, mbf_real = parse_output(output_stdout)

                # Armazenar (Média, MBF)
                all_results[file_choice].append((media_real, mbf_real))

                print(f"  > {test_name}: ✅ Success! Duration: {duration:.2f}s | Media: {media_real} | MBF: {mbf_real}")

            except subprocess.TimeoutExpired:
                print(f"  > {test_name}: ❌ ERROR: Timed out.")
                all_results[file_choice].append(("TIMEOUT", "TIMEOUT"))
            except subprocess.CalledProcessError as e:
                print(f"  > {test_name}: ❌ ERROR: Failed with code {e.returncode}.")
                all_results[file_choice].append(("FAIL", "FAIL"))
            except Exception as e:
                print(f"  > {test_name}: ❌ An unexpected error occurred: {e}")
                all_results[file_choice].append(("ERROR", "ERROR"))

            #time.sleep(1) # Pequena pausa entre testes

    # --- Geração do Arquivo de Saída (Formato Puro Vertical) ---
    print("\n" + "="*80)
    print(f"GERANDO ARQUIVO DE SAÍDA NO FORMATO PURO: {OUTPUT_FILE}")
    print("="*80)

    with open(os.path.join(original_cwd, OUTPUT_FILE), 'w') as results_file:

        # Cabeçalhos de referência (apenas no topo)
        results_file.write(f"# Resultados de Testes de Batch (Total de {len(TEST_SCENARIOS)} Cenários)\n")
        results_file.write("# Formato: [ID do Cenário] \\n [Média 5.txt] \\n [MBF 5.txt] \\n ... (até 500.txt)\n\n")

        # Iterar sobre os CENÁRIOS
        for i, scenario in enumerate(TEST_SCENARIOS):

            # 1. Escreve o ID do Cenário
            results_file.write(f"{scenario['id']}\n")

            # 2. Escreve os 12 valores (6 arquivos * 2 métricas) verticalmente
            for file_choice in FILE_CHOICES:
                # O índice 'i' corresponde ao cenário atual
                media, mbf = all_results[file_choice][i]

                # Formatação e escrita dos valores (sem rótulos!)
                media_str = f"{media:.2f}" if isinstance(media, float) else str(media)
                mbf_str = f"{mbf:.2f}" if isinstance(mbf, float) else str(mbf)

                results_file.write(f"{media_str}\n")
                results_file.write(f"{mbf_str}\n")

            # Adiciona uma linha vazia para separar o próximo cenário
            results_file.write("\n")


    # 3. Retornar ao diretório original
    os.chdir(original_cwd)
    print(f"\nReturned to original directory: {os.getcwd()}")


def main():
    if compile_project():
        run_test_batch_manual()
        print("\n" + "="*80)
        print(f"BATCH TESTING COMPLETED. RESULTS LOGGED IN: {OUTPUT_FILE}")
        print("="*80)

if __name__ == "__main__":
    main()