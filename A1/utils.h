/**
* Tyler Thompson
* WMU CS3240 - A1
* Jan 8, 2020
*/

#ifndef UTILS_H
#define UTILS_H

extern int get_rand_int(int min, int max);
extern double get_rand_double(int min, int max);
extern unsigned long get_time_nano(void);
extern void print_runtime(clock_t start_time, clock_t end_time);

#endif
