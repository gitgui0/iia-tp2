import subprocess
import concurrent.futures
import sys
import os
import time

# --- Configuration ---
BATCH_SIZES = [5, 20, 50, 100, 250, 500]
# Name of your batch execution script
TEST_SCRIPT = "run_batch_tests_ev.py"

def run_test_process(batch_size):
    """
    Executes the run_batch_tests_ev.py script with the given batch_size 
    as the ONLY command-line argument.
    """
    
    # The command we want to run: python run_batch_tests_ev.py [batch_size]
    # We pass the batch_size as the only argument for the C program's file index.
    command = [sys.executable, TEST_SCRIPT, str(batch_size)]
    
    start_time = time.time()
    print(f"[START] Batch size {batch_size}: Command: {' '.join(command)}")
    
    try:
        # Start the process and wait for it to complete
        result = subprocess.run(
            command, 
            check=True, 
            text=True, 
            capture_output=False,  # Set to False to let run_batch_tests_ev.py print its status directly
            shell=False
        )
        
        duration = time.time() - start_time
        print(f"[DONE] Batch size {batch_size}: Finished successfully in {duration:.2f} seconds.")
        
        return f"Batch size {batch_size} SUCCESS"
        
    except subprocess.CalledProcessError as e:
        # This catches a non-zero exit code from run_batch_tests_ev.py
        duration = time.time() - start_time
        print(f"[FAIL] Batch size {batch_size}: FAILED after {duration:.2f} seconds.")
        print(f"       Return Code: {e.returncode}")
        return f"Batch size {batch_size} FAILED"
        
    except FileNotFoundError:
        print(f"[CRITICAL] Could not find the script '{TEST_SCRIPT}'.")
        return f"Batch size {batch_size} FAILED (Script not found)"

if __name__ == "__main__":
    print(f"--- Launching {len(BATCH_SIZES)} concurrent test processes ---")
    print("Each run will write to its own file (e.g., results_file_5.txt).")
    
    # Use the number of tests as the maximum number of concurrent workers
    MAX_CONCURRENT_TESTS = len(BATCH_SIZES)
    
    # Use ProcessPoolExecutor for true parallel execution
    with concurrent.futures.ProcessPoolExecutor(max_workers=MAX_CONCURRENT_TESTS) as executor:
        
        # Submit all tasks
        future_to_size = {executor.submit(run_test_process, size): size for size in BATCH_SIZES}
        
        # Wait for all futures to complete
        for future in concurrent.futures.as_completed(future_to_size):
            size = future_to_size[future]
            try:
                # Retrieve the result (Success/Failed string)
                future.result() 
            except Exception as exc:
                print(f'[CRASH] Batch size {size} generated an unexpected exception: {exc}')

    print("\n==================================")
    print("All concurrent batch test runs are complete.")
    print("==================================")