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

struct FILE create_file(char *file_name)
{
    struct FILE *filelist = (struct FILE *)FILE_ADDR;
    int empty = 100;
    for (int i = 0; i < NFILE; i++) {
        if (filelist[i].f_name[0] == 0 && i < empty) {
            empty = i;
        }
        if (strcmp(filelist[i].f_name, file_name) == 0) {
            printf("this name is already used.\n");
            return;
        }
    }
    if (empty == 100) {
        printf("max file.\n");
    }
    strcpy(filelist[empty].f_name, file_name);
    return filelist[empty];
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
