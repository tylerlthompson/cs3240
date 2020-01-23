/**
* Tyler Thompson
* WMU CS3240 - A2
* Jan 29, 2020
*/

#include <stdlib.h>
#include <stdio.h>
#include "apue.h"
#include "card.h"

#define DATA_FILE_NAME "cards.csv"
#define CARD_COUNT 1614

typedef struct _args {
    char *buf;
    int buf_length;
    void **dest;
} parser_args;


void parse_data(char *buf, int buf_length, int column_num, CARD *card);
void parse_string(parser_args args);
void parse_int(parser_args args);
void parse_rarity(parser_args args);
char * rarity_to_string(enum Rarity rarity);
void print_cards(CARD *cards, int size);
void free_cards(CARD *cards, int size);
FILE * open_file(const char *file_name, int *line_count);

static void (*parsers[8]) (parser_args) = {parse_int, parse_string, parse_string, parse_int, parse_string, parse_string, parse_string, parse_rarity};

int main(int argc, char *argv[]) {
    int line_count;
    FILE *data_file = open_file(DATA_FILE_NAME, &line_count);
    CARD *cards = malloc(sizeof(CARD)*line_count);
    int i = 0, quote_count = 0, column_num = 0, card_num = 0;
    char buf[1024];  // file reading buffer
    char next_char;

    fgets(buf, 128, data_file); // throw out the first line

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
            i = 0;
            column_num = 0;
            card_num++;
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
    fclose(data_file);
    print_cards(cards, CARD_COUNT);
    free_cards(cards, CARD_COUNT);
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

void print_cards(CARD *cards, int size) {
    int i;
    const char separator[43] = "------------------------------------------";
    for (i=0; i<size; i++) {
        printf("%-30s%12s\n%-34s%8s\n%s\n%s\n%s\n%42s\n\n", 
            cards[i].name, 
            cards[i].cost, 
            cards[i].type, 
            rarity_to_string(cards[i].rarity),
            separator,
            cards[i].text,
            separator,
            cards[i].stats);
    }
}

void free_cards(CARD *cards, int size) {
    int i;
    for (i=0; i<size; i++) {
        free(cards[i].name);
        free(cards[i].cost);
        free(cards[i].type);
        free(cards[i].text);
        free(cards[i].stats);
    }
    free(cards);
}

FILE * open_file(const char *file_name, int *line_count) {
    int lines = 0;
    FILE *data_file = fopen(DATA_FILE_NAME, "r");
    if (!data_file) err_quit("Failed to open data file %s\n", DATA_FILE_NAME);
    while(!feof(data_file)) if (fgetc(data_file) == '\n') lines++;
    rewind(data_file);
    *line_count = lines;
    return data_file;
}
