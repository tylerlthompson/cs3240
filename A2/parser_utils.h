/**
* Tyler Thompson
* WMU CS3240 - A2
* Jan 29, 2020
*/

#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H

#include <stdio.h>
#include "card.h"

/* parser function arguments */
typedef struct _args {
    char *buf;
    int buf_length;
    void **dest;
} parser_args;

extern void parse_file(FILE *data_file, CARD *cards, CARD **sorted_cards, int *cards_size);
extern void parse_data(char *buf, int buf_length, int column_num, CARD *card);
extern void parse_string(parser_args args);
extern void parse_int(parser_args args);
extern void parse_rarity(parser_args args);
extern char * rarity_to_string(enum Rarity rarity);
extern void insert_sorted_card(CARD **cards, CARD *card, int size);
extern int binary_search_card(CARD **cards, CARD search_card, int cards_length);

#endif