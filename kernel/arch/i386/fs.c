#include <string.h>
#include <stdio.h>
#include <kernel/fs.h>
#include <kernel/memory.h>

void init_fs(memory_manager *memman)
{
    struct FILE *filelist = (struct FILE *)FILE_ADDR;
    for (int i = 0; i < NFILE; i++) {
        filelist[i].f_count = 0;
        filelist[i].f_mode = 0;
        filelist[i].f_name[0] = 0;
        filelist[i].f_size = 0;
    }
    return;
}

void create_file(char *file_name)
{
    struct FILE *filelist = (struct FILE *)FILE_ADDR;
    int empty = 100;
    for (int i = 0; i < NFILE; i++) {
        if (filelist[i].f_name[0] == 0 && i < empty) {
            empty = i;
        }
        if (strcmp(filelist[i].f_name, file_name) == 0) {
            printf("\nthis name is already used.");
            return;
        }
    }
    if (empty == 100) {
        printf("\nmax file.");
        return;
    }
    strcpy(filelist[empty].f_name, file_name);
}

void remove_file(char *file_name)
{
    struct FILE *filelist = (struct FILE *) FILE_ADDR;
    for (int i = 0; i < NFILE; i++) {
        if (strcmp(filelist[i].f_name, file_name) == 0) {
            filelist[i].f_name[0] = 0;
            return;
        }
    }
    printf("\nfile does not exist.");
}

void ls(void)
{
    struct FILE *filelist = (struct FILE *)FILE_ADDR;
    printf("\n");
    for (int i = 0; i < NFILE; i++) {
        if (filelist[i].f_name[0] != 0) {
            printf("%s ", filelist[i].f_name);
        }
    }
    return;
}
