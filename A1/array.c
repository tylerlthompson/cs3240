/**
* Tyler Thompson
* WMU CS3240 - A1
* Jan 8, 2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "array.h"
#include "utils.h"
#include "apue.h"

#define ARRAY_BREAK_SIZE 1000 /* max size of array in each thread using insert sort */

/**
 * create an array of random doubles
 */
double * createArray(int * size) {
    int i;
    *size = get_rand_int(10000, 12000);
    double * array = malloc(sizeof(double) * (*size));
    for (i=0; i<*size; i++) array[i] = get_rand_double(100, 10000);
    return array;
}

/**
 * sort an array of doubles using threaded insert-merge sort
 * array is written to file and memory freed when complete
 */
int sort_array(double * in_array, int size) {
    int ret = 0;
    pthread_t thread;
    clock_t start_time, end_time;
    struct arg_struct thread_args = {0, in_array, size};
    printf("Input Array Size: %d\n", size);
    start_time = clock();

    /* start main thread and wait for it to finish */
    pthread_create(&(thread), NULL, &insert_merge_sort, (void *) &(thread_args));
    while (!thread_args.thread_complete) usleep(1);
    pthread_join(thread, NULL);

    end_time = clock();
    print_runtime(start_time, end_time);
    if (!write_array(in_array, size, "array.csv")) ret = -1;
    free(in_array);
    printf("Done.\n");
    return ret;
}

/**
 * a threaded function that breaks up a double array into chunks and insert-merge sorts them
 */
void * insert_merge_sort(void * args_t) {
    int i;
    struct arg_struct * args = args_t;

    if (args->size <= ARRAY_BREAK_SIZE) {
        // do insert sort
        for (i=0; i < args->size; i++) insert_sorted(args->in_array, args->in_array[i], i);
    }
    else {
        pthread_t thread_1, thread_2;

        /* calculate size of each half of the array */
        int thread_size_1 = args->size / 2;
        int thread_size_2 = (args->size / 2) + (args->size % 2);

        /* create arrays for both halves and fill them */
        double * array_1 = malloc(sizeof(double)*thread_size_1);
        double * array_2 = malloc(sizeof(double)*thread_size_2);
        for (i=0; i<thread_size_1; i++) array_1[i] = args->in_array[i];
        for (i=0; i<thread_size_2; i++) array_2[i] = args->in_array[i+thread_size_1];

        /* start thread to process both halves */
        struct arg_struct thread_args_1 = {0, array_1, thread_size_1};
        struct arg_struct thread_args_2 = {0, array_2, thread_size_2};
        pthread_create(&(thread_1), NULL, &insert_merge_sort, (void *) &(thread_args_1));
        pthread_create(&(thread_2), NULL, &insert_merge_sort, (void *) &(thread_args_2));

        /* wait for threads to complete */
        while (!thread_args_1.thread_complete && !thread_args_2.thread_complete) usleep(1);

        /* join threads */
        pthread_join(thread_1, NULL);
        pthread_join(thread_2, NULL);

        // merge both halves back together
        merge(array_1, thread_size_1, array_2, thread_size_2, args->in_array);

        /* free memory of temporary arrays */
        free(array_1);
        free(array_2);

    }
    args->thread_complete = 1;
    pthread_exit(0);
}

/**
 * insert a double into a sorted array of doubles
 */
void insert_sorted(double * array, double value, int size) {
    int i;
    for (i=size-1; i>=0 && array[i] > value; i--) {
       array[i+1] = array[i];
    }
    array[i+1] = value;
}

/**
 * merge sort two arrays of doubles
 */
void merge(double * src_1, int src_size_1, double * src_2, int src_size_2, double * dest) {
    int i = 0, j = 0, k = 0;

    for (i = 0; i < src_size_1 + src_size_2;) { // loop through size of merged array
        if (j < src_size_1 && k < src_size_2) { // merge src1 and src2
            if (src_1[j] < src_2[k]) {
                dest[i] = src_1[j];
                j++;
            }
            else {
                dest[i] = src_2[k];
                k++;
            }
            i++;
        }
        else if (j == src_size_1) { // size of src 1 reached, copy in the rest of src 2
            while (i < src_size_1 + src_size_2) {
                dest[i] = src_2[k];
                i++;
                k++;
            }
        }
        else { // size of src 2 reached, copy in the rest of src 1
            while (i < src_size_1 + src_size_2) {
                dest[i] = src_1[j];
                i++;
                j++;
            }
        }
    }
}

/**
 * write an array of doubles to array.csv
 */
int write_array(double * array, int size, char * file_name) {
    printf("writing file %s\n", file_name);
    int i, ret = 0;
    FILE * array_file = fopen(file_name, "w");
    if (!array_file) {
        err_msg("failed to open file %s\n", file_name); 
        ret = -1;
    }
    else {
        for (i=0; i<size; i++) {
            if (!fprintf(array_file, "%f,", array[i])) {
                err_msg("failed to write to file %s\n", file_name);
                ret = -1;
            }
            else {
                if ((i+1) % 10 == 0) { // newline every 10 elements
                    if (!fprintf(array_file,"\n")) { 
                        err_msg("failed to write to file %s\n", file_name);
                        ret = -1;
                    }
                }
            }
        }
        fclose(array_file);
    }
    return ret;
}

/**
 * print an array of doubles
 */
void print_array(double * array, int size) {
    int i;
    for (i=0; i<size; i++) printf(">%f<\n", array[i]);
}