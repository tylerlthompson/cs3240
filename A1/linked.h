/**
* Tyler Thompson
* WMU CS3240 - A1
* Jan 8, 2020
*/

#ifndef LINKED_H
#define LINKED_H

typedef struct node {
    struct node *next;
    double data;
} linked_list;

/* arguments for a thread */
struct arg_struct_list {
    int thread_complete;
    linked_list * in_list;
    int size;
};

extern linked_list * creatLinkedList(int * size);
extern int sort_linked(linked_list * in_list, int size);
extern void * insert_merge_sort_list(void * arg_t);
extern void insert_sorted_list(linked_list * head, double value, int size);
extern void merge_list(linked_list * src_1, int src_size_1, linked_list * src_2, int src_size_2, linked_list * dest);
extern void print_linked_list(linked_list * head, int size);

#endif
