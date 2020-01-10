/**
* Tyler Thompson
* WMU CS3240 - A1
* Jan 8, 2020
*/

#ifndef ARRAY_H
#define ARRAY_H

/* arguments for a thread */
struct arg_struct {
    int thread_complete;
    double * in_array;
    int size;
};

extern double * createArray(int * size);
extern int sort_array(double * in_array, int size);
extern void * insert_merge_sort(void * args_t);
extern void insert_sorted(double * array, double value, int size);
extern void merge(double * src_1, int src_size_1, double * src_2, int src_size_2, double * dest);
extern int write_array(double * array, int size, char * file_name);
extern void print_array(double * array, int size);

#endif