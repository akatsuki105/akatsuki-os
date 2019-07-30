#ifndef _FS_H_
#define _FS_H_

#include "memory.h"

#define FILEMAN_ADDR 0x00300000
#define FILE_NO_CHILD -1
#define IFDIR 040000
#define FILE_MAX_NAME 16

struct FILEMAN {
    struct FILEINFO *info_top;
    int allocated_id;
};

struct FILEINFO {
    int id, parent, child;
    char name[FILE_MAX_NAME];
    int type;
    unsigned int timestamp;
    unsigned int *addr;
    unsigned int size;
    struct FILEINFO *next;
};

void init_fs();
struct FILEINFO *file_getinfo(int id);
struct FILEINFO *file_search(char *name);
void file_remove(struct FILEINFO *file, char recursive);
void file_unlink(struct FILEINFO *file);
void file_link(struct FILEINFO *file, struct FILEINFO *directory);
struct FILEINFO *file_write(char *name, char *data, char is_directory);
struct FILEINFO *file_update(char *name, char *data);
struct FILEINFO *file_read(char *name);
void chdir(char *name);
void lsdir(void);

extern int cd;

#endif _FS_H_