/**
* Tyler Thompson
* WMU CS3240 - A2
* Jan 29, 2020
* Dup Test: Wind-Scarred Crag 
*/

#include <stdlib.h>
#include "apue.h"
#include "parser_utils.h"

#define INPUT_FILE_NAME "cards.csv"
#define OUTPUT_FILE_NAME "cards.txt"


void free_cards(CARD *cards, int size);
FILE * open_file(const char *file_name, int *line_count);
void write_cards(CARD **cards, int size, const char *file_name);

int main(int argc, char *argv[]) {
    int line_count, sorted_size;
    FILE *data_file = open_file(INPUT_FILE_NAME, &line_count);

    /* throw out first line */
    char buf[128];
    fgets(buf, 128, data_file); 
    line_count--;
    sorted_size = line_count;

    CARD *cards = malloc(sizeof(CARD)*line_count);
    CARD **sorted_cards = malloc(sizeof(CARD*)*sorted_size);

    parse_file(data_file, cards, &sorted_size, sorted_cards);

    fclose(data_file);
    write_cards(sorted_cards, sorted_size, OUTPUT_FILE_NAME);

    free_cards(cards, line_count);
    free(sorted_cards);

    printf("Done.\n");
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
    FILE *data_file = fopen(file_name, "r");
    if (!data_file) err_quit("Failed to open data file %s\n", file_name);
    printf("Opening data source file %s\n", file_name);
    while(!feof(data_file)) if (fgetc(data_file) == '\n') lines++;
    rewind(data_file);
    *line_count = lines;
    return data_file;
}

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
