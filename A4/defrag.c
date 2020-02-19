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
#include "apue.h"

char * get_directory_name(int argc, char *argv[]) {
    struct stat stat_struct;
    if (argc != 2) err_quit("Incorrect number of arguments.");
    if (stat(argv[1], &stat_struct)) err_quit("Failed to stat directory %s", argv[1]);
    if (!S_ISDIR(stat_struct.st_mode)) err_quit("%s is not a directory.", argv[1]);
    return argv[1];
}

void process_dir(char *dir_name, unsigned char *bin_store[66], int bin_sizes[66]) {
    int str_len, file_index;
    struct stat stat_struct;
    struct dirent *cur_entries;
    char child_dir[256], file_index_str[2];
    FILE *bin_file = NULL;

    lstat(dir_name, &stat_struct);
    if (!S_ISLNK(stat_struct.st_mode)) { // don't follow symbolic links
        if (S_ISDIR(stat_struct.st_mode)) {
            
            DIR *cur_dir = opendir(dir_name);
            
            while((cur_entries = readdir(cur_dir)) != NULL) {
                if (strcmp((*cur_entries).d_name, ".") && strcmp((*cur_entries).d_name, "..")) {
                    sprintf(child_dir, "%s/%s", dir_name, (*cur_entries).d_name);
                    process_dir(child_dir, bin_store, bin_sizes);
                }
                cur_entries++;
            }
            closedir(cur_dir);
        }
        else if (S_ISREG(stat_struct.st_mode)) {
            
            for (str_len=0; dir_name[str_len] != '\0'; str_len++);
            sprintf(file_index_str, "%.2s", dir_name+(str_len-6));
            file_index = atoi(file_index_str);
            printf("File found %s File index %d\n", dir_name, file_index);
            bin_file = fopen(dir_name, "rb");

            // fseek(bin_file, 0, SEEK_END);
            // file_len = ftell(bin_file);
            // rewind(bin_file);
            bin_store[file_index] = malloc(sizeof(unsigned char) * stat_struct.st_size);
            bin_sizes[file_index] = stat_struct.st_size;

            fread(bin_store[file_index], sizeof(unsigned char) * stat_struct.st_size, 1, bin_file);
            fclose(bin_file);
        }
    }
}

int main(int argc, char *argv[]) {
    unsigned char *bin_array[66];
    int bin_sizes[66];

    // get directory name from command line argument
    char *dir_name = get_directory_name(argc, argv);

    process_dir(dir_name, bin_array, bin_sizes);
    
    FILE *out_file = fopen("music.mp3", "wb");

    int i;
    for (i=0; i<66; i++) {
        fwrite(bin_array[i], sizeof(unsigned char) * bin_sizes[i], 1, out_file);
    }
    fclose(out_file);

    return 0;
}