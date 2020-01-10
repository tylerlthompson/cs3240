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
#include "linked.h"
#include "utils.h"

#define ARRAY_BREAK_SIZE 1000 /* max size of array in each thread using insert sort */

linked_list * creatLinkedList(int * size) {
    int i;
    *size = get_rand_int(10000, 12000);
    linked_list *head, *cur_node;
    head = (struct node*) malloc(sizeof(struct node*));
    cur_node = head;
    for (i=0; i<*size; i++) {
        cur_node->data = get_rand_double(100, 10000);
        cur_node->next = malloc(sizeof(linked_list*));
        cur_node = cur_node->next;
    }
    return head;
}

int sort_linked(linked_list * in_list, int size) {
    int ret = 0;
    pthread_t thread;
    time_t start_time, end_time;
    struct arg_struct_list thread_args = {0, in_list, size};
    printf("Input Linked List Size: %d\n", size);
    start_time = get_time_nano();

    /* start main thread and wait for it to finish */
    pthread_create(&(thread), NULL, &insert_merge_sort_list, (void *) &(thread_args));
    while (!thread_args.thread_complete) usleep(1);
    pthread_join(thread, NULL);

    end_time = get_time_nano();
    printf("Sort Runtime: %ldms\n", (end_time-start_time)/1000000);

    // if (!write_array(in_list, size, "array.csv")) ret = -1;
    free(in_list);
    printf("Done.\n");
    return ret;
}

void * insert_merge_sort_list(void * args_t) {
    int i;
    struct arg_struct_list * args = args_t;

    if (args->size <= ARRAY_BREAK_SIZE) {
        // do insert sort
        // for (i=0; i < args->size; i++) insert_sorted_list(args->in_array, args->in_array[i], i);
    }
    else {
        pthread_t thread_1, thread_2;

        /* calculate size of each half of the list */
        int thread_size_1 = args->size / 2;
        int thread_size_2 = (args->size / 2) + (args->size % 2);

        /* create lists for both halves and fill them */
        linked_list * cur_node;
        linked_list * head_1 = (struct node*) malloc(sizeof(struct node*));
        linked_list * head_2 = (struct node*) malloc(sizeof(struct node*));
        cur_node = head_1;
        for (i=0; i<thread_size_1; i++) {
            cur_node->data = args->in_list->data;
            cur_node->next = malloc(sizeof(linked_list*));
            cur_node = cur_node->next;
            args->in_list = args->in_list->next;
        }
        cur_node = head_2;
        for (i=0; i<thread_size_2; i++) {
            cur_node->data = args->in_list->data;
            cur_node->next = malloc(sizeof(linked_list*));
            cur_node = cur_node->next;
            args->in_list = args->in_list->next;
        }
    

        /* start thread to process both halves */
        struct arg_struct_list thread_args_1 = {0, head_1, thread_size_1};
        struct arg_struct_list thread_args_2 = {0, head_2, thread_size_2};
        pthread_create(&(thread_1), NULL, &insert_merge_sort_list, (void *) &(thread_args_1));
        pthread_create(&(thread_2), NULL, &insert_merge_sort_list, (void *) &(thread_args_2));

        /* wait for threads to complete */
        while (!thread_args_1.thread_complete && !thread_args_2.thread_complete) usleep(1);

        /* join threads */
        pthread_join(thread_1, NULL);
        pthread_join(thread_2, NULL);

        // merge both halves back together
        // merge(array_1, thread_size_1, array_2, thread_size_2, args->in_array);

        /* free memory of temporary arrays */
        

    }
    args->thread_complete = 1;
    pthread_exit(0);
}

void insert_sorted_list(linked_list * head, double value, int size) {

}

void merge_list(linked_list * src_1, int src_size_1, linked_list * src_2, int src_size_2, linked_list * dest) {

}

void print_linked_list(linked_list * head, int size) {
    linked_list * cur_node = head;
    while(cur_node->next) {
        printf(">%f<\n", cur_node->data);
        cur_node = cur_node->next;
    }
}