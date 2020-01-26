/**
* Tyler Thompson
* WMU CS3240 - A2
* Jan 29, 2020
*/

#include <stdlib.h>
#include <stdio.h>
#include "apue.h"

#include "parser_utils.h"

/* parser functions indexed in the same order as the columns in the source file */
static void (*parsers[8]) (parser_args) = {parse_int, parse_string, parse_string, parse_int, parse_string, parse_string, parse_string, parse_rarity};


void parse_file(FILE *data_file, CARD *cards, int *cards_size, CARD **sorted_cards) {
    int i = 0, quote_count = 0, column_num = 0, card_num = 0;
    char buf[1024];  // file reading buffer
    char next_char;

    while(!feof(data_file)) {
        buf[i] = fgetc(data_file);
        
        if ((buf[i] == ',') && (quote_count % 2 == 0)) { // detect when a comma is found outside of double quotes
            buf[i] = '\0';
            parse_data(buf, i, column_num, &(cards[card_num]));
            i = 0;
            column_num++;
        }
        else if ((buf[i] == '\n') && (quote_count % 2 == 0)) { // detect when newline is found outside of double quotes
            buf[i] = '\0';
            parse_data(buf, i, column_num, &(cards[card_num]));

            if (binary_search_card(sorted_cards, cards[card_num], card_num-1) == -1) { // search for existing duplicate card
                insert_sorted_card(sorted_cards, &(cards[card_num]), card_num);    
                card_num++;
            }
            else { // duplicate found
                (*cards_size)--;
            }
            i = 0;
            column_num = 0;
        }
        else if (buf[i] == '\\') {  // detect newline in string and replace with real newline
            next_char = fgetc(data_file);
            if (next_char == 'n') buf[i] = '\n';
            else ungetc(next_char, data_file);
            i++;
        }
        else if (buf[i] == '"') { // detect when two double quotes are found   
            next_char = fgetc(data_file);
            if (next_char == '"') {
                buf[i] = '"'; 
                i++;
            }
            else {
                ungetc(next_char, data_file);
                quote_count++;
            }
        }
        else i++;
    }
}

void parse_data(char *buf, int buf_length, int column_num, CARD *card) {
    void *dests[8] = {(void *) &(card->id),
                      (void *) &(card->name),
                      (void *) &(card->cost),
                      (void *) &(card->coverted_cost),
                      (void *) &(card->type),
                      (void *) &(card->text),
                      (void *) &(card->stats),
                      (void *) &(card->rarity)};
    parser_args args = {buf, buf_length, dests[column_num]};
    parsers[column_num](args);
}

void parse_string(parser_args args) {
    *(args.dest) = malloc(sizeof(char)*(args.buf_length+1));
    strcpy(*(args.dest), args.buf);
}

void parse_int(parser_args args) {
    *(unsigned int *)args.dest = atoi(args.buf);
}

void parse_rarity(parser_args args) {
    if (strcmp(args.buf, "uncommon") == 0) *(char *)args.dest = Uncommon;
    else if (strcmp(args.buf, "rare") == 0) *(char *)args.dest = Rare;
    else if (strcmp(args.buf, "mythic") == 0) *(char *)args.dest = Mythic;
    else *(char *)args.dest = Common;
}

char * rarity_to_string(enum Rarity rarity) {
    if (rarity == Uncommon) return "uncommon";
    else if (rarity == Rare) return "rare";
    else if (rarity == Mythic) return "mythic";
    else return "common";
}

void insert_sorted_card(CARD **cards, CARD *card, int size) {
    int i;
    for (i=size-1; i>=0 && strcmp(cards[i]->name, card->name) > 0; i--) {
        cards[i+1] = cards[i];
    }
    cards[i+1] = card;
}

int binary_search_card(CARD **cards, CARD search_card, int cards_length) {
    int found = -1, top = 0, btm = cards_length, mid, str_cmp;
    while (found == -1 && top <= btm) {
        mid = (btm + top) / 2;
        str_cmp = strcmp(search_card.name, cards[mid]->name);
        if (str_cmp == 0) found = mid;
        else if (str_cmp < 0) btm = mid - 1;
        else top = mid + 1;
    }
    return found;
}