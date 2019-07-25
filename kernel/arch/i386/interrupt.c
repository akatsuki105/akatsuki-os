#include "kernel/asm.h"
#include "kernel/gdtidt.h"
#include "kernel/interrupt.h"
#include "kernel/keyboard.h"
#include "kernel/timer.h"
#include "kernel/syscall.h"
#include "kernel/mtask.h"

void interrupt_done(void){
  outb(MASTER_PIC_CMD_STAT, PIC_EOI);
  outb(SLAVE_PIC_CMD_STAT, PIC_EOI);
}


void keyboard_interrupt(void){
  outb(MASTER_PIC_CMD_STAT, irq1);
  keyboard_input_int(getchar());
  interrupt_done();
}

void timer_interrupt(void)
{
  	struct TIMER *timer;
	char ts = 0;
	outb(MASTER_PIC_CMD_STAT, irq0);
	timerctl.count++;
	// 最初のタイマーだけチェック
	if (timerctl.next > timerctl.count) {
		return;
	}
	// 期限がすぎたタイマーを実行する
	timer = timerctl.t0;
	for (;;) {
		if (timer->timeout > timerctl.count) {
			break;
		}
		timer->flags = TIMER_FLAGS_ALLOC;
		// タスクスイッチ
		if (timer != task_timer) {
			fifo32_put(timer->fifo, timer->data);
		} else {
			ts = 1;
		}
		timer = timer->next;
	}
	timerctl.t0 = timer;
	timerctl.next = timer->timeout;
	if (ts != 0) {
		mt_taskswitch();
	}
	return;
}

uint32_t software_interrupt(uint32_t syscall_num, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t arg5)
{
	switch (syscall_num) {
	case SYSCALL_WRITE:
		break;
	case SYSCALL_READ:
		break;
	}
	return 0;
}
