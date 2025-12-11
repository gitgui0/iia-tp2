import subprocess
import itertools
import re
import os
import sys # <-- NOVO MÓDULO PARA LER ARGUMENTOS

# --- CONFIGURAÇÃO E LEITURA DE ARGUMENTOS ---

# 1. Tenta ler o índice do ficheiro do argumento de linha de comando
try:
    # sys.argv[0] é o nome do script
    # sys.argv[1] é o primeiro argumento (o número do ficheiro)
    FIXED_FILE_INDEX = sys.argv[1]
except IndexError:
    # Se nenhum argumento for fornecido, usa um valor padrão e avisa o utilizador
    print("AVISO: Nenhum índice de ficheiro fornecido. Usando o valor padrão '5'.")
    FIXED_FILE_INDEX = '5'

# --- CONFIGURAÇÃO ---
# 1. Path to your executable
C_PROGRAM_PATH = r"C:\Users\deepz\Desktop\isec\2 ANO\IIA\TP2\cmake-build-debug\TP2.exe"
# 2. Directory where the executable and data files reside (Used for 'cwd')
C_PROGRAM_DIR = os.path.dirname(C_PROGRAM_PATH) 

# ALTERAÇÃO AQUI: O nome do ficheiro de saída usa o FIXED_FILE_INDEX
OUTPUT_FILE = f'results_file_{FIXED_FILE_INDEX}_torneio.txt' 

# --- PARAMETER ARRAYS (COM REPETIÇÕES) ---

# A. Groups: [Pop_Size, Mut_Prob, Recomb_Prob, Tourn_Size]
GROUPS = [
    # Muda PR
    ['100', '0.01', '0.3', '2'],
    ['100', '0.01', '0.5', '2'],
    ['100', '0.01', '0.7', '2'],
    # Muda PM
    ['100', '0.0', '0.7', '2'],
    ['100', '0.001', '0.7', '2'],
    ['100', '0.01', '0.7', '2'], # Duplicado 1
    ['100', '0.05', '0.7', '2'],
    # Muda TP
    ['10', '0.01', '0.7', '2'],
    ['50', '0.01', '0.7', '2'],
    ['100', '0.01', '0.7', '2'], # Duplicado 2
    # Muda TT
    ['100', '0.01', '0.7', '2'], # Duplicado 3
    ['100', '0.01', '0.7', '5'],
    ['100', '0.01', '0.7', '10'],
]

# B. Scenarios: [Recomb_Type, Mut_Type, Invalid_Handling, Selection_Method]
SCENARIOS = [
    ['1', '1', '0', '2'],
    ['1', '2', '0', '2'],
    ['1', '2', '1', '2'],
    ['1', '2', '2', '2'],
    ['2', '2', '2', '2'],
    ['3', '2', '2', '2'],
]

# C. Fixed Program Parameters
NUM_RUNS = '15'
NUM_ITERATIONS = '25'
ALGORITHM_CHOICE = 'e'
PARAM_INPUT_CHOICE = '2'

# --- HELPER FUNCTIONS (INALTERADAS) ---

def create_input_string(group_params, scenario_params):
    pop_size, mut_prob, recomb_prob, tourn_size = group_params
    recomb_type, mut_type, invalid_handling, selection_method = scenario_params
    
    input_str = (
        f"{FIXED_FILE_INDEX}\n" f"{NUM_RUNS}\n" f"{NUM_ITERATIONS}\n"
        f"{ALGORITHM_CHOICE}\n" f"{PARAM_INPUT_CHOICE}\n"
        f"{pop_size}\n" f"{mut_prob}\n" f"{recomb_prob}\n" f"{tourn_size}\n"
        f"{recomb_type}\n" f"{mut_type}\n" f"{invalid_handling}\n"
        f"{selection_method}\n" "\n"
    )
    return input_str

def extract_metrics(output):
    media_match = re.search(r"Melhor Solucao - Media de Distancias:\s*([\d.]+)", output)
    mbf_match = re.search(r"Melhor Solucao - MBF:\s*([\d.]+)", output)
    
    media = media_match.group(1) if media_match else "N/A"
    mbf = mbf_match.group(1) if mbf_match else "N/A"
    
    return media, mbf

# --- MAIN EXECUTION LOGIC ---

def run_automation():
    # 1. Setup file
    # O ficheiro de saída agora será, por exemplo, 'automated_results_file_5.txt'
    if not os.path.exists(OUTPUT_FILE):
        with open(OUTPUT_FILE, 'w') as f:
            f.write("") 

    print(f"O ficheiro de resultados será: {OUTPUT_FILE}")
    print(f"Total Grupos (incl. repetições) a testar: {len(GROUPS)}")
    print(f"Total de Cenários por Grupo: {len(SCENARIOS)}")
    
    # 2. Iterar sobre Grupos (Loop Externo)
    for i, group_params in enumerate(GROUPS):
        # Unpack Group parameters
        pop_size, mut_prob, recomb_prob, tourn_size = group_params
        
        # O índice 'i' + 1 é o número do grupo.
        group_number = i + 1

        # MUDAR AQUI SE EU QUISER MUDA RO FOMRATO DO .TXT

        print(f"\n--- Grupo {group_number}/{len(GROUPS)} START: TP={pop_size}, PM={mut_prob} ---")
        
        # 3. Abrir o ficheiro em modo append
        with open(OUTPUT_FILE, 'a') as f:
            
            # **1ª ESCRITA: RÓTULO DO GRUPO SEM TABULAÇÃO**
            group_context = (
                f"Grupo {group_number} " 
                f"TP={pop_size} PM={mut_prob} PR={recomb_prob} TT={tourn_size} "
            )
            # f.write(group_context)
            
            # 4. Iterar sobre Cenários (Loop Interno)
            for j, scenario_params in enumerate(SCENARIOS):
                
                # Unpack Scenario parameters
                recomb_type, mut_type, invalid_handling, selection_method = scenario_params
                
                print(f"  > Running Scenario {j + 1}/{len(SCENARIOS)}: Recomb={recomb_type}, Invalid={invalid_handling}")
                
                input_data = create_input_string(group_params, scenario_params)
                
                try:
                    # 5. Executar o C program
                    result = subprocess.run(
                        [C_PROGRAM_PATH],
                        input=input_data,
                        capture_output=True,
                        text=True,
                        check=True,
                        timeout=220, 
                        cwd=C_PROGRAM_DIR
                    )
                    
                    # 6. Extrair métricas
                    media, mbf = extract_metrics(result.stdout)
                    
                    print(f"  ✅ Success. Media: {media}, MBF: {mbf}")
                    
                    # 7. **2ª ESCRITA: VALORES TABULADOS**
                    # O formato é: \t [Média] \t [MBF]
                    if j == 0:
                        results_line = f"{media}\t{mbf}"
                    else:
                        results_line = f"\t{media}\t{mbf}"
                    
                    f.write(results_line)
                        
                except subprocess.CalledProcessError:
                    print(f"  ❌ ERROR: C Program failed.")
                    f.write("\tN/A\tN/A") 
                except subprocess.TimeoutExpired:
                    print(f"  ❌ ERROR: C Program timed out.")
                    f.write("\tN/A\tN/A")
                except Exception as e:
                    print(f"  ❌ An unexpected error occurred: {e}")
                    f.write("\tN/A\tN/A")
                    
            # 8. NO FINAL DO GRUPO, escrever o NEWLINE
            f.write("\n")
            print("--- GRUPO TERMINADO. Linha concluída. ---")


if __name__ == '__main__':
    run_automation()