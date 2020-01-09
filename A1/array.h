/**
* Tyler Thompson
* WMU CS3240 - A1
* Jan 8, 2020
*/

#ifndef ARRAY_H
#define ARRAY_H

extern double * createArray(int * size);
extern void print_array(double * array, int size);
extern int array(double * in_array, int size);
extern void * insert_merge_sort(void * args_t);
extern void insert_sorted(double * array, int size);
extern int get_rand_int(int min, int max);

#endif