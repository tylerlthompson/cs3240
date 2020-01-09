/**
* Tyler Thompson
* WMU CS3240 - A1
* Jan 8, 2020
*/

#ifndef UTILS_H
#define UTILS_H

extern int get_rand_int(int min, int max);
extern double get_rand_double(int min, int max);
extern int write_array(double * array, int size, char * file_name);
extern void print_array(double * array, int size);
extern long get_time_nano(void);

#endif
