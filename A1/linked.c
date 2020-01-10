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

#define ARRAY_BREAK_SIZE 50 /* max size of array in each thread using insert sort */

linked_list * creatLinkedList(int * size) {
    int i;
    *size = get_rand_int(40, 50);
    linked_list *head, *cur_node;
    head = create_linked_list(*size);
    head->last = NULL;
    cur_node = head;
    for (i=0; i<*size; i++) {
        cur_node->data = get_rand_double(100, 10000);
        cur_node = cur_node->next;
    }
    return head;
}

linked_list * create_linked_list(int size) {
    int i;
    linked_list *head, *cur_node;
    head = malloc(sizeof(linked_list));
    cur_node = head;
    for (i=0; i<size; i++) {
        cur_node->next = malloc(sizeof(linked_list));
        cur_node->next->last = cur_node;
        cur_node = cur_node->next;
    }
    return head;
}

int sort_linked(linked_list * in_list, int size) {
    int ret = 0;
    pthread_t thread;
    time_t start_time, end_time;
    struct arg_struct_list thread_args = {0, in_list, size};
    // printf("Input Linked List Size: %d\n", size);
    start_time = get_time_nano();

    /* start main thread and wait for it to finish */
    pthread_create(&(thread), NULL, &insert_merge_sort_list, (void *) &(thread_args));
    while (!thread_args.thread_complete) usleep(1);
    pthread_join(thread, NULL);

    end_time = get_time_nano();
    printf("Sort Runtime: %ldms\n", (end_time-start_time)/1000000);

    // if (!write_array(in_list, size, "array.csv")) ret = -1;
    //free(in_list);
    printf("Done.\n");
    return ret;
}

void * insert_merge_sort_list(void * args_t) {
    struct arg_struct_list * args = args_t;

    if (args->size <= ARRAY_BREAK_SIZE) {
        // do insert sort
        // print_linked_list(args->in_list);
        
        // for (i=0; i < args->size; i++) insert_sorted_list(args->in_array, args->in_array[i], i);
        linked_list *cur_node;
        print_linked_list(args->in_list);
        printf("running insert sort, size %d, head %p\n", args->size, (void *) args->in_list);
        linked_list *sorted_list = create_linked_list(args->size);
        for(cur_node=args->in_list; cur_node->next; cur_node=cur_node->next) {
            insert_sorted_list(sorted_list, cur_node);
        }
        printf("final head %p\n", (void *) args->in_list);
        args->in_list = sorted_list;
        print_linked_list(sorted_list);
    }
    else {
        pthread_t thread_1, thread_2;

        /* calculate size of each half of the list */
        int thread_size_1 = args->size / 2;
        int thread_size_2 = (args->size / 2) + (args->size % 2);

        /* create lists for both halves and fill them */
        linked_list * head_1 = create_linked_list(thread_size_1);
        linked_list * head_2 = create_linked_list(thread_size_2);
    

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
        // free(head_1);
        // free(head_2);
        

    }
    args->thread_complete = 1;
    pthread_exit(0);
}

void insert_sorted_list(linked_list * head, linked_list * in_node) {
    linked_list * cur_node;
    printf("value to insert %f\n", in_node->data);
    for (cur_node=head; cur_node->last && cur_node->data > in_node->data; cur_node=cur_node->last) {
        // double temp = cur_node->next->data;
        cur_node->next->data = cur_node->data;
        // cur_node->data = temp;
        // swap_nodes(cur_node, cur_node->next);
    }
    cur_node->next->data = in_node->data;
}

void merge_list(linked_list * src_1, int src_size_1, linked_list * src_2, int src_size_2, linked_list * dest) {

}

void print_linked_list(linked_list * head) {
    linked_list * cur_node;
    for (cur_node=head; cur_node->next; cur_node=cur_node->next) printf(">%f<\n", cur_node->data);
}

linked_list * get_tail(linked_list * head) {
    linked_list * cur_node = head;
    while (cur_node->next) {
        cur_node=cur_node->next;
    }
    return cur_node;
}

void swap_nodes(linked_list * a, linked_list * b) {
    linked_list * temp = a;
    a->last = b;
    a->next = b->next;
    b->last = temp->last;
    b->next = a;
}
