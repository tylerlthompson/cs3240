/**
* Tyler Thompson
* WMU CS3240 - A1
* Jan 7, 2020
*/

#include <stdio.h>
#include <stdlib.h>
#include "array.h"
#include "linked.h"

int main(int argc, char * argv[]) {
    int size;
    double * doubles_array = createArray(&size);
    sort_array(doubles_array, size);
    linked_list * doubles_list = createLinkedList_Filled(&size);
    sort_linked(doubles_list, size);
    exit(0);
}
