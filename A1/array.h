/**
* Tyler Thompson
* WMU CS3240 - A1
* Jan 8, 2020
*/

#ifndef ARRAY_H
#define ARRAY_H


extern double * createArray(int * size);
extern int array(double * in_array, int size);
extern void * insert_merge_sort(void * args_t);
extern void insert_sorted(double * array, double value, int size);
extern void merge(double * src_1, int src_size_1, double * src_2, int src_size_2, double * dest);
extern int get_rand_int(int min, int max);
extern double get_rand_double(int min, int max);
extern void write_array(double * array, int size);
extern void print_array(double * array, int size);

#endif