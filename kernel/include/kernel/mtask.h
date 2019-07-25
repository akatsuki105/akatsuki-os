#ifndef _MTASK_H_
#define _MTASK_H_

/* time status segment 
 * マルチプロセス(タスク)で各プロセスの状態を保存しておく構造体
*/
struct TSS32
{
    int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
    int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    int es, cs, ss, ds, fs, gs;
    int ldtr, iomap;
};

#endif _MTASK_H_
