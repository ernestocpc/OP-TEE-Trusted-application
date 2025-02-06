/*
 * Programmer   : Regis Rodolfo Schuch
 * Date         : 10 June 2024
 *              : Applied Computing Research Group, Unijui University, Brazil
 *              : regis.schuch@unijui.edu.br
 *              :
 * Title        : memory-out-experiment.c 
 *              :
 * Description  : The memory-out-experiment.c programme carries out memory performance tests, measuring the times required to allocate,
 *              : write, read and free memory blocks of varying sizes. The results are recorded in a CSV file.
 *              : Code description:
 *              : 1) Constants and Definitions
 *              :  a) NUM_TESTS is the number of tests to be performed for each block size.
 *              :  b) MIN_BLOCK_SIZE is the minimum block size to be tested (100 MB).
 *              :  c) MAX_BLOCK_SIZE is the maximum block size to be tested (1 GB).
 *              :  d) BLOCK_STEP is the block size increment at each step (100 MB).
 *              : 2) Allocation: measures the time to allocate a memory block of the specified size using malloc.
 *              :  a) clock_gettime(CLOCK_MONOTONIC, &start) records the start time.
 *              :  b) malloc(block_size) allocates the memory.
 *              :  c) clock_gettime(CLOCK_MONOTONIC, &end) records the end time.
 *              :  d) The elapsed time is calculated and stored in allocation_time.
 *              : 3) Write: Measures the time it takes to write to the entire memory block.
 *              :  a) clock_gettime(CLOCK_MONOTONIC, &start) records the start time.
 *              :  b) A loop writes values to each byte of the memory block.
 *              :  c) clock_gettime(CLOCK_MONOTONIC, &end) records the end time.
 *              :  d) The elapsed time is calculated and stored in write_time.
 *              : 4) Reading: Measures the time taken to read the entire memory block.
 *              :  a) clock_gettime(CLOCK_MONOTONIC, &start) records the start time.
 *              :  b) A loop reads values from each byte of the memory block.
 *              :  c) clock_gettime(CLOCK_MONOTONIC, &end) records the end time.
 *              :  d) The elapsed time is calculated and stored in read_time.
 *              : 5) Release: Measures the time to release the allocated memory block.
 *              :  a) clock_gettime(CLOCK_MONOTONIC, &start) records the start time.
 *              :  b) free(block) frees the memory.
 *              :  c) clock_gettime(CLOCK_MONOTONIC, &end) records the end time.
 *              :  d) The elapsed time is calculated and stored in free_time. 
 *              :
 * Compile      :
 * Capabilities : clang-morello -o memory-in-experiment memory-in-experiment.c -lm
 *              :
 * run          : ./memory-in-experiment	 
 * 
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* #define NUM_TESTS 30  */ // Updated to record 30 tests per block size
#define MIN_BLOCK_SIZE (1024 * 1024 * 100) // 100 MB
#define MAX_BLOCK_SIZE (1024 * 1024 * 1000) // 1 GB
#define BLOCK_STEP (1024 * 1024 * 100) // 100 MB per step
#define ITERATIONS 1000000  // Defining 1 million iterations for write/read


void perform_test(int num_of_trials, size_t block_size, FILE *log_file) {
    /* for (int test = 0; test < NUM_TESTS; test++) { */
    for (int trial = 0; trial < num_of_trials; trial++) {
        struct timespec start, end;
        double allocation_time, write_time, read_time, free_time;

        // Allocation
        clock_gettime(CLOCK_MONOTONIC, &start);
        char *block = (char *)malloc(block_size);
        clock_gettime(CLOCK_MONOTONIC, &end);
        if (block == NULL) {
            fprintf(log_file, "%zu,%d,Allocation failed,,,,\n", block_size / (1024 * 1024), trial + 1);
            return;
        }
        allocation_time = ((end.tv_sec - start.tv_sec) * 1000.0) + ((end.tv_nsec - start.tv_nsec) / 1e6);

        // Write
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (size_t i = 0; i < block_size; i++) {
            for (int j = 0; j < ITERATIONS / block_size; j++) {
                block[i] = (char)(i % 256);
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        write_time = ((end.tv_sec - start.tv_sec) * 1000.0) + ((end.tv_nsec - start.tv_nsec) / 1e6);

        // Read
        clock_gettime(CLOCK_MONOTONIC, &start);
        volatile char temp;
        for (size_t i = 0; i < block_size; i++) {
            for (int j = 0; j < ITERATIONS / block_size; j++) {
                temp = block[i];
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        read_time = ((end.tv_sec - start.tv_sec) * 1000.0) + ((end.tv_nsec - start.tv_nsec) / 1e6);

        // Free
        clock_gettime(CLOCK_MONOTONIC, &start);
        free(block);
        clock_gettime(CLOCK_MONOTONIC, &end);
        free_time = ((end.tv_sec - start.tv_sec) * 1000.0) + ((end.tv_nsec - start.tv_nsec) / 1e6);

        // Log the times in CSV format
        fprintf(log_file, "%zu,%d,%.3f,%.3f,%.3f,%.3f\n",
                block_size / (1024 * 1024), trial + 1, allocation_time, write_time, read_time, free_time);
    }
}

int main() {
  
    time_t t;   // not a primitive datatype
    time(&t);

 
    int num_of_trials= 0; /* number of repetitions of each operation */
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time); // Start time

    /* 
     * Activate these two lines to determine the number
     * of trials at run time.
      printf("\nType the number of trials e.g., 50 : ");
      scanf("%d", &num_of_trials);
     */
    num_of_trials=100;
    printf("Each operation will be executed %d times \n", num_of_trials);


    FILE *log_file = fopen("memory-out-experiment-results.csv", "w");
    if (log_file == NULL) {
        printf("Failed to open log file\n");
        return 1;
    }
    else {
        printf("csv file has been created and is now ready for collecting results!\n");
    }


    // Write CSV header
    fprintf(log_file, "Block Size (MB),Trial Num,Allocation Time (ms),Write Time (ms),Read Time (ms),Free Time (ms)\n");

    printf("\nThis program has been launched at (date and time): %s", ctime(&t));
    printf(" \nmemory allocate, write, read, free in execution now (no cheri compartments) ...\n");

    for (size_t block_size = MIN_BLOCK_SIZE; block_size <= MAX_BLOCK_SIZE; block_size += BLOCK_STEP) {
        perform_test(num_of_trials, block_size, log_file);
    }

    fclose(log_file);

    clock_gettime(CLOCK_MONOTONIC, &end_time); // End time

    double total_execution_time = ((end_time.tv_sec - start_time.tv_sec) * 1000.0) +
                                  ((end_time.tv_nsec - start_time.tv_nsec) / 1e6);

    // Log the total execution time to the file
    log_file = fopen("memory-out-experiment-results.csv", "a");
    if (log_file != NULL) {
        fprintf(log_file, "\nTotal execution time: %.3f milliseconds\n", total_execution_time);
        fclose(log_file);
    }

    return 0;
}
