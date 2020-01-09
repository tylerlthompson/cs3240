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
 * write an array of doubles to array.csv
 */
int write_array(double * array, int size, char * file_name) {
    printf("writing file %s\n", file_name);
    int i, ret = 0;
    char write_buf[size][12];
    FILE * array_file = fopen(file_name, "w");
    if (!array_file) {
        err_msg("failed to open file %s\n", file_name); 
        ret = 1;
    }
    else {
        for (i=0; i<size; i++) { // print doubles to buffer
            if (i % 10 == 0) {
                sprintf(write_buf[i], "%f,\n", array[i]); // newline every 10 elements
            }
            else {
                sprintf(write_buf[i], "%f,", array[i]); 
            }
        }
        if (!fwrite(write_buf, 12, size, array_file)) {
            err_msg("failed to write to file %s\n", file_name);
            ret = 1;
        }
        fclose(array_file);
    }
    return ret;
}

/**
 * print an array of doubles
 */
void print_array(double * array, int size) {
    int i;
    for (i=0; i<size; i++) printf(">%f<\n", array[i]);
}

/**
 * get the number of nano seconds since epoch
 */
long get_time_nano(void) {
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    return spec.tv_nsec;
}