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
#include "array.h"
#include "utils.h"

#define ARRAY_BREAK_SIZE 1000 /* max size of array in each thread using insert sort */

linked_list * createLinkedList_Filled(int * size) {
    int i;
    *size = get_rand_int(10000, 12000);
    linked_list *head, *cur_node;
    head = create_linked_list(*size);
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
    head->next = NULL;
    head->data = 0.0;
    cur_node = head;
    for (i=0; i<size-1; i++) {
        linked_list *new_node = malloc(sizeof(linked_list));
        cur_node->next = new_node;
        cur_node->data = 0.0;
        cur_node = cur_node->next;
    }
    cur_node->next = NULL;
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

    if (!write_list(thread_args.in_list, thread_args.size, "linked.csv")) ret = -1;
    free(in_list);
    printf("Done.\n");
    return ret;
}

void * insert_merge_sort_list(void * args_t) {
    struct arg_struct_list * args = args_t;

    if (args->size <= ARRAY_BREAK_SIZE) {
        // do insert sort
        linked_list *cur_node, *next_node;
        linked_list *sorted_list = create_linked_list(1);

        sorted_list->data = args->in_list->data;
        args->in_list = args->in_list->next;
        
        for(cur_node=args->in_list; cur_node!=NULL; cur_node=next_node) {
            next_node = cur_node->next;
            insert_sorted_list(&sorted_list, cur_node);
        }
        
        args->in_list = sorted_list;
    }
    else {
        pthread_t thread_1, thread_2;

        /* calculate size of each half of the list */
        int thread_size_1 = args->size / 2;
        int thread_size_2 = (args->size / 2) + (args->size % 2);
       
        /* create lists for both halves and fill them */
        linked_list * head_1 = create_linked_list(thread_size_1);
        linked_list * head_2 = create_linked_list(thread_size_2);
    
        linked_list *cur_node, *main_node = args->in_list;
        for (cur_node=head_1; cur_node->next!=NULL; cur_node=cur_node->next) {
            cur_node->data = main_node->data;
            main_node = main_node->next;
        }
        for (cur_node=head_2; cur_node->next!=NULL; cur_node=cur_node->next) {
            cur_node->data = main_node->data;
            main_node = main_node->next;
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
        merge_list(thread_args_1.in_list, thread_args_2.in_list, &(args->in_list));

        /* free memory of temporary arrays */
        free(head_1);
        free(head_2);
        

    }
    args->thread_complete = 1;
    pthread_exit(0);
}

void insert_sorted_list(linked_list ** head, linked_list * node) {
    linked_list * cur_node;

    if (*head == NULL || (*head)->data >= node->data) {
        node->next = *head;
        *head = node;
    }
    else {
        for (cur_node=*head; cur_node->next != NULL && cur_node->next->data < node->data; cur_node=cur_node->next);
        node->next = cur_node->next;
        cur_node->next = node;
    }
}

void merge_list(linked_list *src_1, linked_list *src_2, linked_list **dest) {
    linked_list temp;
    linked_list *cur_node = &temp;
    temp.next = NULL;
    while (1) {
        if (src_1 == NULL) {
            cur_node->next = src_2;
            break;
        }
        else if (src_2 == NULL) {
            cur_node->next = src_1;
            break;
        }
        if (src_1->data <= src_2->data) {
            swap_nodes(&(cur_node->next), &src_1);
        }
        else {
            swap_nodes(&(cur_node->next), &src_2);
        }
        cur_node = cur_node->next;
    }
    (*dest) = temp.next;
}

void print_linked_list(linked_list * head) {
    linked_list * cur_node;
    for (cur_node=head; cur_node != NULL; cur_node=cur_node->next) printf(">%f<\n", cur_node->data);
}

void swap_nodes(linked_list **a, linked_list **b) {
    linked_list* temp_node = *b;   
    *b = temp_node->next;  
    temp_node->next = *a;  
    *a = temp_node;  
}

int write_list(linked_list * in_list, int size, char * file_name) {
    int i = 0, ret_val = 0;
    double * write_buf = malloc(sizeof(double) * (size));;
    linked_list *cur_node;
    for (cur_node=in_list; cur_node!=NULL; cur_node=cur_node->next) {
        write_buf[i] = cur_node->data;
        i++;
    }
    ret_val = write_array(write_buf, size, file_name);
    free(write_buf);
    return ret_val;
}