/**
* Tyler Thompson
* WMU CS3240 - A1
* Jan 8, 2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "array.h"

#define ARRAY_BREAK_SIZE 1000 /* max size of array in each thread using insert sort */

/* arguments for a thread */
struct arg_struct {
    int thread_complete;
    double * in_array;
    int size;
};

double * createArray(int * size) {
    int i;
    *size = get_rand_int(10000, 12000);
    double * array = malloc(sizeof(double) * (*size));
    for (i=0; i<*size; i++) array[i] = get_rand_double(100, 10000);
    return array;
}

int array(double * in_array, int size) {
    pthread_t thread;
    struct arg_struct thread_args = {0, in_array, size};
    
    pthread_create(&(thread), NULL, &insert_merge_sort, (void *) &(thread_args));

    while (!thread_args.thread_complete) usleep(1);

    pthread_join(thread, NULL);
    write_array(in_array, size);

    printf("Done.\n");
    return 0;
}

void * insert_merge_sort(void * args_t) {
    int i;
    struct arg_struct * args = args_t;
    printf("Size: %d\n", args->size);
    // print_array(args->in_array, args->size);
    if (args->size <= ARRAY_BREAK_SIZE) {
        // do insert sort
        for (i=0; i < args->size; i++) {
            insert_sorted(args->in_array, get_rand_double(10, 10000), i);
        }
        printf("Thread finished.\n");
    }
    else {
        pthread_t thread_1, thread_2;
        int thread_size_1 = args->size / 2;
        int thread_size_2 = (args->size / 2) + (args->size % 2);
        printf("thread_size_1: %d thread_size_2: %d\n", thread_size_1, thread_size_2);
        double * array_1 = malloc(sizeof(double)*thread_size_1);
        double * array_2 = malloc(sizeof(double)*thread_size_2);
        struct arg_struct thread_args_1 = {0, array_1, thread_size_1};
        struct arg_struct thread_args_2 = {0, array_2, thread_size_2};

        pthread_create(&(thread_1), NULL, &insert_merge_sort, (void *) &(thread_args_1));
        pthread_create(&(thread_2), NULL, &insert_merge_sort, (void *) &(thread_args_2));

        while (!thread_args_1.thread_complete) usleep(1);
        while (!thread_args_2.thread_complete) usleep(1);

        pthread_join(thread_1, NULL);
        pthread_join(thread_2, NULL);

        // do merge sort
        merge(array_1, thread_size_1, array_2, thread_size_2, args->in_array);

        free(array_1);
        free(array_2);

    }
    args->thread_complete = 1;
    pthread_exit(0);
    return 0;
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
    int i, j, k;
    j = k = 0;

    for (i = 0; i < src_size_1 + src_size_2;) {
        if (j < src_size_1 && k < src_size_2) {
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
        else if (j == src_size_1) {
            for (; i < src_size_1 + src_size_2;) {
                dest[i] = src_2[k];
                i++;
                k++;
            }
        }
        else {
            for (; i < src_size_1 + src_size_2;) {
                dest[i] = src_1[j];
                i++;
                j++;
            }
        }
    }
}

/**
 * generate a random integer
 */
int get_rand_int(int min, int max) {
    return (rand() % (max - min)) + min;
}

/**
 * generate a random double
 */
double get_rand_double(int min, int max) {
    const int decimal_place = 100000;
    return ((double)(get_rand_int(min*decimal_place, max*decimal_place)) / decimal_place);
}

/**
 * write an array of doubles to array.csv
 */
void write_array(double * array, int size) {
    FILE * array_file = fopen("array.csv", "w");
    char write_buf[size][12];
    int i;
    for (i=0; i<size; i++) {
        sprintf(write_buf[i], "%f,", array[i]);
    }
    fwrite(write_buf, 12, size, array_file);
   
    fclose(array_file);
}

/**
 * print an array of doubles
 */
void print_array(double * array, int size) {
    int i;
    for (i=0; i<size; i++) printf(">%f<", array[i]);
}
