import subprocess
import re
import os
import sys

# --- CONFIGURAÇÃO E LEITURA DE ARGUMENTOS ---

try:
    # Tenta ler o índice do ficheiro via linha de comando (ex: python script.py 100)
    FIXED_FILE_INDEX = sys.argv[1]
except IndexError:
    print("AVISO: Nenhum índice de ficheiro fornecido. Usando o valor padrão '100'.")
    FIXED_FILE_INDEX = '100'

# --- CAMINHOS ---
C_PROGRAM_PATH = r"C:\Users\deepz\Desktop\isec\2 ANO\IIA\TP2\cmake-build-debug\TP2.exe"
C_PROGRAM_DIR = os.path.dirname(C_PROGRAM_PATH)
OUTPUT_FILE = f'results_hibrido_{FIXED_FILE_INDEX}_fraco.txt'

# --- VARIÁVEIS DE CONTROLO ---

# Aqui definimos apenas as abordagens que queremos testar (1 e 2)
# Isto funcionará como os teus "Cenários"
APPROACHES = ['1', '2']

# Parâmetros FIXOS retirados do teu log
# GA Params
NUM_RUNS = '15'
NUM_ITERATIONS_GA = '50'
ALGO_CHOICE = 'h'      # Hibrido
PARAM_MODE = '2'       # Manual
POP_SIZE = '5'
MUT_PROB = '0.01'
RECOMB_PROB = '0.5'
TOURN_SIZE = '2'
REC_TYPE = '1'         # Uniforme
MUT_TYPE = '1'         # Troca
INV_HANDLING = '0'     # Rep. Heuristica

# Hibrido / Local Search Params (Simulated Annealing)
LOCAL_ALGO = '2'       # Recristalizacao Simulada
TEMP_INI = '10'
TEMP_FIN = '1'
COOLING = '0.9'
LOCAL_ITERS = '10'
SAME_COST = '0'        # Nao aceita mesmo custo

# --- FUNÇÕES ---

def create_input_string(approach):
    """
    Cria a string de input exata para o Algoritmo Híbrido conforme o teu log.
    """
    input_str = (
        f"{FIXED_FILE_INDEX}\n"    # Escolha do Ficheiro (ex: 100)
        f"{NUM_RUNS}\n"            # Numero de Runs
        f"{NUM_ITERATIONS_GA}\n"   # Numero de iteracoes GA
        f"{ALGO_CHOICE}\n"         # h - Hibrido
        f"{PARAM_MODE}\n"          # 2 - Manualmente
        
        # Parametros GA
        f"{POP_SIZE}\n"
        f"{MUT_PROB}\n"
        f"{RECOMB_PROB}\n"
        f"{TOURN_SIZE}\n"
        f"{REC_TYPE}\n"
        f"{MUT_TYPE}\n"
        f"{INV_HANDLING}\n"
        
        # Variavel: Abordagem (1 ou 2)
        f"{approach}\n"
        
        # Parametros Algoritmo Local (SA)
        f"{LOCAL_ALGO}\n"          # 2 - Recristalizacao Simulada
        f"{TEMP_INI}\n"
        f"{TEMP_FIN}\n"
        f"{COOLING}\n"
        f"{LOCAL_ITERS}\n"
        f"{SAME_COST}\n"
        "\n"
    )
    return input_str

def extract_metrics(output):
    media_match = re.search(r"Melhor Solucao - Media de Distancias:\s*([\d.]+)", output)
    mbf_match = re.search(r"Melhor Solucao - MBF:\s*([\d.]+)", output)
    
    media = media_match.group(1) if media_match else "N/A"
    mbf = mbf_match.group(1) if mbf_match else "N/A"
    
    return media, mbf

# --- EXECUÇÃO ---

def run_automation():
    # 1. Preparar ficheiro
    if not os.path.exists(OUTPUT_FILE):
        with open(OUTPUT_FILE, 'w') as f:
            f.write("") # Cria vazio se nao existir

    print(f"O ficheiro de resultados será: {OUTPUT_FILE}")
    print(f"A testar Hibrido com Abordagens: {APPROACHES}")

    # Vamos tratar isto como UM grupo (parametros fixos) e DOIS cenarios (abordagens)
    # Para manter o formato de output que pediste.
    
    print(f"\n--- INICIANDO TESTE HIBRIDO (File {FIXED_FILE_INDEX}) ---")
    
    with open(OUTPUT_FILE, 'a') as f:
        
        # 1. Escrever cabeçalho da linha (Parametros fixos principais)
        # Podes editar este texto se quiseres que apareça outra coisa no txt
        header_text = (
            f"Hibrido (Pop{POP_SIZE} PM{MUT_PROB} PR{RECOMB_PROB}) "
            f"SA(Ti{TEMP_INI} Tf{TEMP_FIN} Cool{COOLING}) "
        )
        # f.write(header_text) # Se quiseres o texto antes dos numeros, descomenta

        # 2. Iterar sobre as Abordagens (1 e 2)
        for i, approach in enumerate(APPROACHES):
            
            print(f"  > A correr Abordagem {approach}...")
            
            input_data = create_input_string(approach)
            
            try:
                # Executar o programa C
                result = subprocess.run(
                    [C_PROGRAM_PATH],
                    input=input_data,
                    capture_output=True,
                    text=True,
                    check=True,
                    timeout=300, # Aumentei um pouco o timeout pois Hibrido demora mais
                    cwd=C_PROGRAM_DIR
                )
                
                # Extrair dados
                media, mbf = extract_metrics(result.stdout)
                print(f"    ✅ Sucesso. Media: {media}, MBF: {mbf}")
                
                # Formatar linha para o TXT
                # Se for o primeiro (Abordagem 1), escreve normal
                # Se for o segundo (Abordagem 2), escreve com tabs antes
                if i == 0:
                    results_line = f"{media}\t{mbf}"
                else:
                    results_line = f"\t{media}\t{mbf}"
                
                f.write(results_line)
                    
            except subprocess.CalledProcessError:
                print(f"    ❌ ERRO: O programa C falhou.")
                f.write("\tN/A\tN/A") 
            except subprocess.TimeoutExpired:
                print(f"    ❌ ERRO: Timeout.")
                f.write("\tN/A\tN/A")
            except Exception as e:
                print(f"    ❌ Erro inesperado: {e}")
                f.write("\tN/A\tN/A")
                
        # 3. Final da linha (depois das duas abordagens)
        f.write("\n")
        print("--- Linha concluída e salva. ---")

if __name__ == '__main__':
    run_automation()