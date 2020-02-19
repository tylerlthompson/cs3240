/**
* Tyler Thompson
* WMU CS3240 - A4
* Feb 19, 2020
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include "apue.h"

#define FRAG_COUNT 66
#define OUTPUT_FILE "music.mp3"

struct process_args {
    char *dir_name;
    unsigned char *bin_store[FRAG_COUNT];
    int bin_sizes[FRAG_COUNT];
} p_args;

char * get_directory_name(int argc, char *argv[]) {
    struct stat stat_struct;
    if (argc != 2) err_quit("Incorrect number of arguments.");
    if (stat(argv[1], &stat_struct)) err_quit("Failed to stat directory %s", argv[1]);
    if (!S_ISDIR(stat_struct.st_mode)) err_quit("%s is not a directory.", argv[1]);
    return argv[1];
}

int read_file(char *src, unsigned char *dest, int file_size) {
    int fread_ret, ret_val = 0;
    FILE *bin_file = NULL;
    if ((bin_file = fopen(src, "rb")) == NULL) {
        ret_val = -1;
    }
    else {
        fread_ret = fread(dest, sizeof(unsigned char) * file_size, 1, bin_file);
        if (!fread_ret) {
            if (!ferror(bin_file)) {
                ret_val = -2;
            }
        }
        fclose(bin_file);
    }
    return ret_val;
}

int write_file(unsigned char *src[FRAG_COUNT], int src_sizes[FRAG_COUNT], char *dest) {
    int i, fwrite_ret, ret_val = 0;
    FILE *out_file = NULL;
    
    if ((out_file = fopen(dest, "wb")) == NULL ){
        ret_val = -1;
    }
    else {
        for (i=0; i<FRAG_COUNT; i++) {
            fwrite_ret = fwrite(src[i], sizeof(unsigned char) * src_sizes[i], 1, out_file);
            if (!fwrite_ret) {
                if (!ferror(out_file)) {
                    ret_val = -2;
                }
            }
        }
        fclose(out_file);
    }
    return ret_val;
}

void process_dir(char *dir_name, unsigned char *bin_store[FRAG_COUNT], int bin_sizes[FRAG_COUNT]) {
    int str_len, file_index;
    struct stat stat_struct;
    struct dirent *cur_entries;
    char child_dir[256], file_index_str[2];
    DIR *cur_dir = NULL;

    lstat(dir_name, &stat_struct);
    if (!S_ISLNK(stat_struct.st_mode)) {  // don't follow symbolic links
        if (S_ISDIR(stat_struct.st_mode)) {  // directory found
            
            if ((cur_dir = opendir(dir_name)) == NULL) err_ret("Failed to open directory %s\n", dir_name);
            
            for (cur_entries=NULL; (cur_entries = readdir(cur_dir)) != NULL; cur_entries++) {
                if (strcmp((*cur_entries).d_name, ".") && strcmp((*cur_entries).d_name, "..")) {
                    sprintf(child_dir, "%s/%s", dir_name, (*cur_entries).d_name);  // append path name
                    process_dir(child_dir, bin_store, bin_sizes);  // process next directory
                }
            }

            if (errno) closedir(cur_dir), err_ret("Error file reading directory %s errno %d\n", dir_name, errno);
            closedir(cur_dir);
        }
        else if (S_ISREG(stat_struct.st_mode)) {  // file found
            
            for (str_len=0; dir_name[str_len] != '\0'; str_len++);  // get length of full path name
            sprintf(file_index_str, "%.2s", dir_name+(str_len-6));  // parse index number from path name
            file_index = atoi(file_index_str);  // convert index number to int

            bin_store[file_index] = malloc(sizeof(unsigned char) * stat_struct.st_size);  // allocate memory for binary data
            bin_sizes[file_index] = stat_struct.st_size;  // store size of file for later

            if (read_file(dir_name, bin_store[file_index], stat_struct.st_size)) err_ret("Failed to read file %s\n", dir_name);

        }
    }
}

void free_bin_store(unsigned char *bin_store[FRAG_COUNT]) {
    int i;
    for (i=0; i<FRAG_COUNT; i++) {
        if (bin_store[i] != NULL) free(bin_store[i]);
    }
}

void create_bin_store(unsigned char *bin_store[FRAG_COUNT]) {
    int i;
    for (i=0; i<FRAG_COUNT; i++) bin_store[i] = NULL;
}

int main(int argc, char *argv[]) {
    unsigned char *bin_store[FRAG_COUNT];
    int bin_sizes[FRAG_COUNT];

    create_bin_store(bin_store);

    // get directory name from command line argument
    char *dir_name = get_directory_name(argc, argv);

    printf("Processing directory and reading files...\n");
    process_dir(dir_name, bin_store, bin_sizes);

    printf("Writing to output file %s...\n", OUTPUT_FILE);
    if (write_file(bin_store, bin_sizes, OUTPUT_FILE)) printf("Failed to write to output file %s\n", OUTPUT_FILE);

    free_bin_store(bin_store);
    printf("Done.\n");
    return 0;
}