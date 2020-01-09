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
    struct node *last;
} linked_list;

extern int sort_linked(linked_list in_array, int size);

#endif
