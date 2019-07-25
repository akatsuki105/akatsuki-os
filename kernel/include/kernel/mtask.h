#ifndef _MTASK_H_
#define _MTASK_H_

#include "timer.h"
#include "memory.h"

#define MAX_TASKS		1000
#define TASK_GDT0		3
#define MAX_TASKS_LV	100
#define MAX_TASKLEVELS	10

/* time status segment 
 * マルチプロセス(タスク)で各プロセスの状態を保存しておく構造体
*/
struct TSS32 {
	int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
	int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
	int es, cs, ss, ds, fs, gs;
	int ldtr, iomap;
};

struct TASK {
	int sel;    /* GDTの番号 */
    int flags;  /* タスクの状態 0:未割当 1:sleep 2:run */
	int level;  /* タスクのレベル */
    int priority;   /* 同レベルタスク内のCPU優先度 */
	struct TSS32 tss; /* 対応するTSS */
};

struct TASKLEVEL {
	int running;    /* このレベルで動作しているタスクの数 */
	int now;    /* 現在動いているタスクのtasksのインデックス */
	struct TASK *tasks[MAX_TASKS_LV];   /* このレベルのタスクのリスト */
};

struct TASKCTL {
	int now_lv; /* 現在動作中のレベル */
	char lv_change; /* 次回タスクスイッチの時にレベルも変えたほうがいいかどうか */
	struct TASKLEVEL level[MAX_TASKLEVELS]; /* TASKLEVEL構造体の全体リスト */
	struct TASK tasks0[MAX_TASKS];  /* TASKの全体リスト */
};

extern struct TIMER *task_timer;
struct TASK *init_multitask(memory_manager *memman);
struct TASK *task_alloc(void);
void task_run(struct TASK *task, int level, int priority);
void task_switch(void);
void task_sleep(struct TASK *task);

void mt_init(void);
void mt_taskswitch(void);

#endif _MTASK_H_
