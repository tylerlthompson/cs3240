/**
* Tyler Thompson
* WMU CS3240 - A1
* Jan 8, 2020
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "utils.h"
#include "apue.h"

/**
 * generate a random integer
 */
int get_rand_int(int min, int max) {
    srand(get_time_nano()); /* seed rand() with current unix time */
    return (rand() % (max - min)) + min;
}

/**
 * generate a random double
 */
double get_rand_double(int min, int max) {
    const int decimal_place = 100000;
    return ((double)(get_rand_int(min*decimal_place, max*decimal_place)) / decimal_place);
}

/**
 * get the number of nano seconds since epoch
 */
long get_time_nano(void) {
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    return spec.tv_nsec;
}