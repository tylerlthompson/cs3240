/**
* Tyler Thompson
* WMU CS3240 - A3
* Jan 29, 2020
* Dup Test: Wind-Scarred Crag 
*/

#include <stdlib.h>
#include <stdio.h>
#include "card.h"
#include "apue.h"

#define INPUT_FILE_NAME "cards.csv"
#define OUTPUT_FILE_NAME "cards.txt"

/* parser function arguments */
typedef struct _args {
    char *buf;
    int buf_length;
    void **dest;
} parser_args;

void free_cards(CARD *cards, int size);
FILE * open_file(const char *file_name, int *line_count);
void write_cards(CARD **cards, int size, const char *file_name);
void parse_file(FILE *data_file, CARD *cards, CARD **sorted_cards, int *cards_size);
void parse_data(char *buf, int buf_length, int column_num, CARD *card);
void parse_string(parser_args args);
void parse_int(parser_args args);
void parse_rarity(parser_args args);
char * rarity_to_string(enum Rarity rarity);
void insert_sorted_card(CARD **cards, CARD *card, int size);
int binary_search_card(CARD **cards, CARD search_card, int cards_length);

/* parser functions indexed in the same order as the columns in the source file */
static void (*parsers[8]) (parser_args) = {parse_int, parse_string, parse_string, parse_int, parse_string, parse_string, parse_string, parse_rarity};


int main(int argc, char *argv[]) {
    int line_count, sorted_size;
    FILE *data_file = open_file(INPUT_FILE_NAME, &line_count);

    /* throw out first line */
    char buf[128];
    fgets(buf, 128, data_file); 
    line_count--;
    sorted_size = line_count;

    CARD **sorted_cards = malloc(sizeof(CARD*)*sorted_size);
    CARD *cards = malloc(sizeof(CARD)*line_count);
    
    parse_file(data_file, cards, sorted_cards, &sorted_size);

    fclose(data_file);
    write_cards(sorted_cards, sorted_size, OUTPUT_FILE_NAME);

    free_cards(cards, line_count);
    free(sorted_cards);

    printf("Done.\n");
}

/**
 * free memory for an array of cards
 */
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

/**
 * open a file and calculate how many new lines are in it
 */
FILE * open_file(const char *file_name, int *line_count) {
    int lines = 0;
    FILE *data_file = fopen(file_name, "r");
    if (!data_file) err_quit("Failed to open data file %s\n", file_name);
    printf("Opening data source file %s\n", file_name);
    while(!feof(data_file)) if (fgetc(data_file) == '\n') lines++;
    rewind(data_file);
    *line_count = lines;
    return data_file;
}

/**
 * write and array of cards to a file
 */
void write_cards(CARD **cards, int size, const char *file_name) {
    const char separator[43] = "------------------------------------------";
    int i;
    FILE *output_file = fopen(file_name, "w");
    if (!output_file) {
        err_msg("Failed to open output file %s\n", file_name); 
        return;
    }
    printf("Writing output to file %s\n", file_name);
    for (i=0; i<size; i++) {
        fprintf(output_file, "%-30s%12s\n%-34s%8s\n%s\n%s\n%s\n%42s\n\n", 
            cards[i]->name, 
            cards[i]->cost, 
            cards[i]->type, 
            rarity_to_string(cards[i]->rarity),
            separator,
            cards[i]->text,
            separator,
            cards[i]->stats);
    }
    fclose(output_file);
}

/**
 * parse a file for MTG card info and store it in a array of structs
 */
void parse_file(FILE *data_file, CARD *cards, CARD **sorted_cards, int *sorted_size) {
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
                (*sorted_size)--;
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

/**
 * parse a data field and store it in its coresponding variable in a card struct
 */
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

/**
 * parse a string and store it in a destination
 */
void parse_string(parser_args args) {
    *(args.dest) = malloc(sizeof(char)*(args.buf_length+1));
    strcpy(*(args.dest), args.buf);
    args.dest[args.buf_length] = '\0';
}

/**
 * parse an integer and store it in a destination
 */
void parse_int(parser_args args) {
    *(unsigned int *)args.dest = atoi(args.buf);
}

/**
 * parse a rarity enum and store it in a destination
 */
void parse_rarity(parser_args args) {
    if (strcmp(args.buf, "uncommon") == 0) *(char *)args.dest = Uncommon;
    else if (strcmp(args.buf, "rare") == 0) *(char *)args.dest = Rare;
    else if (strcmp(args.buf, "mythic") == 0) *(char *)args.dest = Mythic;
    else *(char *)args.dest = Common;
}

/**
 * convert a rarity enum to a string
 */
char * rarity_to_string(enum Rarity rarity) {
    if (rarity == Uncommon) return "uncommon";
    else if (rarity == Rare) return "rare";
    else if (rarity == Mythic) return "mythic";
    else return "common";
}

/**
 * insert a card into a sorted array of card pointers
 */
void insert_sorted_card(CARD **cards, CARD *card, int size) {
    int i;
    for (i=size-1; i>=0 && strcmp(cards[i]->name, card->name) > 0; i--) {
        cards[i+1] = cards[i];
    }
    cards[i+1] = card;
}

/**
 * search an array of card pointers for a specific card, based on name
 */
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