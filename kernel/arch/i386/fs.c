#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <kernel/fs.h>
#include <kernel/memory.h>

int cd;

void init_fs(memory_manager *memman)
{
    struct FILE *filelist = (struct FILE *)FILE_ADDR;
    cd = 0;

    filelist[0].f_count = 0;
    filelist[0].f_mode = IFDIR;
    strcpy(filelist[0].f_name, "root");
    filelist[0].f_size = 0;
    filelist[0].f_data = "/";

    for (int i = 1; i < NFILE; i++) {
        filelist[i].f_count = 0;
        filelist[i].f_mode = 0;
        filelist[i].f_name[0] = 0;
        filelist[i].f_size = 0;
    }
    return;
}

void create_file(char *file_name)
{
    char *file_no;
    int i;
    struct FILE *filelist = (struct FILE *)FILE_ADDR;
    int empty = 100;
    for (i = 0; i < NFILE; i++) {
        if (filelist[i].f_name[0] == 0 && i < empty) {
            empty = i;
        }
    }
    if (empty == 100) {
        printf("\nmax file.");
        return;
    }

    // カレントディレクトリのファイル番号取得
    int j, index = 0;
    char s[3];
    int files[100];
    for (i = 0; filelist[cd].f_data[i] != 0; i++) {
        // ファイル番号の初期化かつファイル番号の格納
        if (filelist[cd].f_data[i] == '/') {
            files[index++] = atoi(s);
            for (j = 0; j < 3; j++) {
                s[j] = 0;
            }
            j = 0;
        } else { // ファイル番号をセット
            s[j++] = filelist[cd].f_data[i];
        }
    }

    // カレントディレクトリにすでに同じファイルが存在するか確認
    for (i = 0; i < index; i++) {
        if (strcmp(filelist[files[i]].f_name, file_name) == 0) {
            printf("\nthis name is already used in this directory.");
            return;
        }
    }

    sprintf(file_no, "%d/", empty);
    strcat(filelist[cd].f_data, file_no);
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
    int i, j, index = 0;
    char s[3];
    int files[100];
    struct FILE *filelist = (struct FILE *)FILE_ADDR;
    printf("\n");

    for (i = 0; filelist[cd].f_data[i] != 0; i++) {
        // ファイル番号の初期化かつファイル番号の格納
        if (filelist[cd].f_data[i] == '/') {
            files[index++] = atoi(s);
            for (j = 0; j < 3; j++) {
                s[j] = 0;
            }
            j = 0;
        } else { // ファイル番号をセット
            s[j++] = filelist[cd].f_data[i];
        }
    }

    // ファイル名の表示
    for (i = 0; i < index; i++) {
        printf("%s ", filelist[i].f_name);
    }

    return;
}
