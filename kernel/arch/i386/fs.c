#include <kernel/fs.h>
#include <stddef.h>
#include <string.h>

void init_fs(memory_manager *memman)
{
    // file managerの初期化
    struct FILEMAN *fileman = (struct FILEMAN *)FILEMAN_ADDR;
    fileman->info_top = (struct FILEINFO *)memman_alloc_4k(memman, sizeof(struct FILEINFO));
    
    // rootファイルの初期化
    struct FILEINFO *rootfile = (struct FILEINFO *)fileman->info_top;
    rootfile->id = 0;
    rootfile->parent = 0;
    rootfile->child = FILE_NO_CHILD;
    strcpy(rootfile->name, "root");
    rootfile->type = IFDIR;
    rootfile->next = rootfile;
}

// IDで指定したファイル構造体を取得する
struct FILEINFO *file_getinfo(int id)
{
    struct FILEMAN *fileman = (struct FILEMAN *)FILEMAN_ADDR;
    return (struct FILEINFO *)(fileman->info_top + sizeof(struct FILEINFO) * id);
}

// 指定したファイルを消す
void file_remove(struct FILEINFO *file, char recursive)
{
    memory_manager *memman = (memory_manager *)MEMMAN_ADDR;
    struct FILEINFO *child, *first;
    // ファイルだったらデータとして利用しているメモリを開放する
    if ((file->type & IFDIR) == 0) {
        memman_free_4k(memman, (int)file->addr, file->size);
    } else if (file->child != FILE_NO_CHILD && recursive) 
    {   // ディレクトリなら再帰的に削除する
        first = child = file_getinfo(file->child);
        do {
            file_remove(child, recursive);
            child = child->next;
        } while (first != child);
    }
    file_unlink(file);
}

// 指定したファイルのリンクを解除する（中身・確保したメモリは消さない）
void file_unlink(struct FILEINFO *file)
{
    struct FILEINFO *parent, *brother;
    brother = file->next;
    parent = file_getinfo(file->parent);
    
    if (file == brother)
    { // 同階層に自分しかいない
        parent->child = FILE_NO_CHILD;
    }
    else {
        // brotherがfileの1つ前になるまで移動
        while (brother->next != file) {
            brother = brother->next;
        }
        // 1つ前のファイルの先をfileの先にする
        brother->next = file->next;
        parent->child = file->next;
    }
}

// 指定したファイルを特定の場所にリンクする
void file_link(struct FILEINFO *file, struct FILEINFO *directory)
{
    struct FILEINFO *brother, *sister;
    if (directory->child == FILE_NO_CHILD) {
        // 子供がいないなら、自分で自分を指してループを作る
        file->next = file;
        directory->child = file->id;
    }
    else {
        // 子供がいる場合は、間に挿入する
        brother = file_getinfo(directory->child);
        sister = brother->next;
        file->next = sister;
        brother->next = file;
    }
}

