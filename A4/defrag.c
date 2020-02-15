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

void process_dir(char *dir_name, char **bin_store) {
    struct stat stat_struct;
    stat(dir_name, &stat_struct);
    if (S_ISDIR(stat_struct.st_mode)) {
        char child_dir[256];
        DIR *cur_dir = opendir(dir_name);
        struct dirent *cur_entries;
        while((cur_entries = readdir(cur_dir)) != NULL) {
            if (strcmp((*cur_entries).d_name, ".") && strcmp((*cur_entries).d_name, "..")) {
                sprintf(child_dir, "%s/%s", dir_name, (*cur_entries).d_name);
                process_dir(child_dir, bin_store);
            }
            cur_entries++;
        }
        closedir(cur_dir);
    }
    else if (S_ISREG(stat_struct.st_mode)) {
        printf("%s\n", dir_name);
    }
}

int main(int argc, char *argv[]) {
    char *bin_array[65];

    // get directory name from command line argument
    char *dir_name = get_directory_name(argc, argv);

    process_dir(dir_name, &(bin_array[0]));
    
    return 0;
}