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

#define ARRAY_BREAK_SIZE 1000

/* arguments for a thread */
struct arg_struct {
    int thread_complete;
    double * in_array;
    int size;
};

int array(double * in_array, int size) {
    pthread_t threads[1];
    struct arg_struct thread_args[1];

    thread_args[0].in_array = in_array;
    thread_args[0].size = size;
    thread_args[0].thread_complete = 0;
    
    pthread_create(&(threads[0]), NULL, &insert_merge_sort, (void *) &(thread_args[0]));

    while (!thread_args[0].thread_complete) usleep(1);

    pthread_join(threads[0], NULL);
    printf("Done.\n");
    return 0;
}

void print_array(double * array, int size) {
    int i;
    for (i=0; i<size; i++) printf(">%f<\n", array[i]);
}

double * createArray(int * size) {
    int i;
    *size = get_rand_int(10000, 12000);
    double *ret_array = malloc(sizeof(double) * (*size));
    for (i=0; i<*size; i++) {
        // ret_array[i] = (double) get_rand_int(0, 10000);
    }
    return ret_array;
}

void * insert_merge_sort(void * args_t) {
    struct arg_struct * args = args_t;
    printf("Size: %d\n", args->size);
    if (args->size <= ARRAY_BREAK_SIZE) {
        // do insert sort
        printf("Thread finished.\n");
    }
    else {
        pthread_t threads[2];
        struct arg_struct thread_args[2];

        thread_args[0].in_array = args->in_array;
        thread_args[0].size = args->size / 2;
        thread_args[0].thread_complete = 0;
        thread_args[1].in_array = &(args->in_array[args->size/2]);
        thread_args[1].size = args->size / 2;
        thread_args[1].thread_complete = 0;

        pthread_create(&(threads[0]), NULL, &insert_merge_sort, (void *) &(thread_args[0]));
        pthread_create(&(threads[1]), NULL, &insert_merge_sort, (void *) &(thread_args[1]));

        while (!thread_args[0].thread_complete) usleep(1);
        while (!thread_args[1].thread_complete) usleep(1);

        pthread_join(threads[0], NULL);
        pthread_join(threads[1], NULL);

        // merge sort


    }
    args->thread_complete = 1;
    pthread_exit(0);
    return 0;
}

void insert_sorted(double * array, int size) {
    int i;
    for (i=0; i<size; i++) {
       
    }
}

int get_rand_int(int min, int max) {
    return (rand() % (max - min)) + min;
}