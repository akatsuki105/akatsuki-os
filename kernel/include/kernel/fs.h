#ifndef _FS_H_
#define _FS_H_

#include "memory.h"

#define FILEMAN_ADDR 0x00300000
#define FILE_NO_CHILD -1
#define IFDIR 040000

struct FILEMAN {
    struct FILEINFO *info_top;
};

struct FILEINFO {
    int id, parent, child;
    char name[16];
    unsigned char type;
    unsigned int timestamp;
    unsigned int *addr;
    unsigned int size;
    struct FILEINFO *next;
};

void init_fs(memory_manager *memman);
struct FILEINFO *file_getinfo(int id);

extern int cd;

#endif _FS_H_