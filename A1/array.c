/**
* Tyler Thompson
* WMU CS3240 - A1
* Jan 8, 2020
*/

#include <stdio.h>
#include "array.h"
#include "utils.h"

double * createArray(int * size) {
    printf("%f", get_rand_double(10));
    return 0;
}

int array(double * array, int size) {
    int i = 0;
    for(i=0; i<size; i++){
        printf("%f", array[i]);
    }
    return 0;
}
