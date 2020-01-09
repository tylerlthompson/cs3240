/**
* Tyler Thompson
* WMU CS3240 - A1
* Jan 7, 2020
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "array.h"

int main(int argc, char * argv[]) {
    int size;
    double * doubles_array = createArray(&size);
    print_array(doubles_array, size);
    array(doubles_array, size);
}
