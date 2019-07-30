#include <kernel/fs.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

int cd;

void init_fs()
{
    memory_manager *memman = (memory_manager *)MEMMAN_ADDR;

    // file managerの初期化
    struct FILEMAN *fileman = (struct FILEMAN *)FILEMAN_ADDR;
    fileman->info_top = (struct FILEINFO *)memman_alloc_4k(memman, sizeof(struct FILEINFO));
    fileman->allocated_id = 0;
    
    // rootファイルの初期化
    struct FILEINFO *rootfile = (struct FILEINFO *)fileman->info_top;
    rootfile->id = fileman->allocated_id++;
    rootfile->parent = 0;
    rootfile->child = FILE_NO_CHILD;
    strncpy(rootfile->name, "root", FILE_MAX_NAME);
    rootfile->type = IFDIR;
    rootfile->next = rootfile;

    cd = 0;
}

// IDで指定したファイル構造体を取得する
struct FILEINFO *file_getinfo(int id)
{
    struct FILEMAN *fileman = (struct FILEMAN *)FILEMAN_ADDR;
    return (struct FILEINFO *)(fileman->info_top + sizeof(struct FILEINFO) * id);
}

// 指定された名前のファイル構造体を探す
struct FILEINFO *file_search(char *name)
{
    struct FILEINFO *current_dir = file_getinfo(cd); // カレントディレクトリのFILEINFO
    struct FILEINFO *first_child, *child;

    // カレントディレクトリ
    if (strlen(name) == 0) {
        return current_dir;
    }
    // 親ディレクトリ
    if (name[0] == '.' && name[1] == '.') {
        return file_getinfo(current_dir->parent);
    }

    // カレントディレクトリに子ファイルが存在する
    if (current_dir->child != FILE_NO_CHILD) {
        first_child = file_getinfo(current_dir->child);
        child = first_child;
        do {
            if (strcmp(child->name, name) == 0) {
                return child;
            }
            child = child->next;
        } while (child != first_child);
    }
    return (struct FILEINFO *)-1;
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

// 指定した場所へデータを書き込む。
/*
 * name file名
 * data ファイルデータ
 * parent 親ディレクトリ名
 * is_directory ファイルを作るかディレクトリを作るか
 */
struct FILEINFO *file_write(char *name, char *data, char is_directory)
{
    struct FILEMAN *fileman = (struct FILEMAN *)FILEMAN_ADDR;
    struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
    struct FILEINFO *info;
    int id;
    struct FILEINFO *parent_info;

    // ファイルがnameであるものを探す。見つからなかったら新しく作成する
    // 見つかればそのファイルをいったん削除し、同じidにファイルを作成する
    info = file_search(name);
    // ファイルが存在しない
    if (info == -1) {
        id = fileman->allocated_id++;
        info = file_getinfo(id);
    } else {
        // ファイルが存在
        id = info->id;
        file_remove(info, 0);
    }

    // ファイルを作るとき
    if (is_directory == 0) {
        int *to = memman_alloc_4k(memman, strlen(data));
        int i = 0, size = strlen(data);
        // データのコピー
        for (i = 0; i < size; i++) {
            to[i] = data[i];
        }
        info->addr = to;
        info->size = size;
        info->type = 0;
    } else {
        info->type = IFDIR;
    }

    // FILEINFOに書き込む
    strncpy(info->name, name, FILE_MAX_NAME);
    info->id = id;
    info->parent = cd;
    info->child = FILE_NO_CHILD;

    // 親のデータを取得してリンク作成
    parent_info = file_getinfo(cd);
    file_link(info, parent_info);

    return info;
}

// ファイルデータを更新する関数
struct FILEINFO *file_update(char *name, char *data)
{
    struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
    struct FILEINFO *info;

    // ファイルがnameであるものを探す。見つからなかったら新しく作成する
    // 見つかればそのファイルをいったん削除し、同じidにファイルを作成する
    info = file_search(name);
    // ファイルが存在しない
    if (info == -1) {
        printf("\ntarget is not found.");
        return -1;
    }

    // ファイルを作るとき
    if ((info->type & IFDIR) == 0) {
        int *to = memman_alloc_4k(memman, strlen(data));
        int i = 0, size = strlen(data);
        // データのコピー
        for (i = 0; i < size; i++) {
            to[i] = data[i];
        }
        info->addr = to;
        info->size = size;
    } else {
        printf("\ntarget is directory.");
        return -1;
    }
    return info;
}

// ファイルデータを読み取る関数
struct FILEINFO *file_read(char *name)
{
    struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
    struct FILEINFO *info;

    // ファイルがnameであるものを探す。見つからなかったら新しく作成する
    // 見つかればそのファイルをいったん削除し、同じidにファイルを作成する
    info = file_search(name);
    // ファイルが存在しない
    if (info == -1) {
        printf("\ntarget is not found.");
        return -1;
    }

    // ファイル内容を表示する
    if ((info->type & IFDIR) == 0) {
        printf("\n");
        for (int i = 0; info->addr[i] != 0; i++) {
            printf("%c", info->addr[i]);
        }
    } else {
        printf("\ntarget is directory.");
        return -1;
    }
    return info;
}

void chdir(char *name)
{
    struct FILEINFO *target = file_search(name);
    if (target == -1) {
        printf("\ntarget is not found.");
        return;
    } else {
        if ((target->type & IFDIR) == 0) {
            printf("\ntarget is not directory.");
            return;
        }
        cd = target->id;
    }
}

void lsdir(void)
{
    struct FILEINFO *chdir = file_getinfo(cd);
    if (chdir->child == FILE_NO_CHILD) {
        return;
    }
    printf("\n");
    struct FILEINFO *firstchild = file_getinfo(chdir->child);
    struct FILEINFO *child = file_getinfo(chdir->child);
    do {
        printf("%s ", child->name);
        child = child->next;
    } while (child != firstchild);
    
}


