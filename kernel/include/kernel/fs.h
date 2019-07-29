#ifndef _FS_H_
#define _FS_H_

#define FILE_ADDR 0x00300000
#define NFILE 100
#define IFDIR 040000

struct FILE {
    char f_count;   /* 参照カウンタ */
    int f_mode; /* 状態,制御情報 */
    int f_size; /* ファイルサイズ */
    char f_name[16]; /* ファイル名 */
    char *f_data; /* ファイルデータ本体へのポインタ */
};

void init_fs();
void create_file(char *file_name);
void remove_file(char *file_name);
void create_dir(char *dir_name);
void ls(void);

extern int cd;

#endif _FS_H_