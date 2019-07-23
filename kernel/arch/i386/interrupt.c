#include "kernel/asm.h"
#include "kernel/gdtidt.h"
#include "kernel/interrupt.h"
#include "kernel/keyboard.h"
#include "kernel/timer.h"

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
  	struct TIMER *t;
	outb(MASTER_PIC_CMD_STAT, irq0);
	timerctl.count++;
	// 最初のタイマーだけチェック
	if (timerctl.next > timerctl.count) {
		return;
	}
	// 期限がすぎたタイマーを実行する
	t = timerctl.t0;
	for (;;) {
		if (t->timeout > timerctl.count) {
			break;
		}
		t->flags = TIMER_FLAGS_ALLOC;
		fifo32_put(t->fifo, t->data);
		t = t->next;
	}
	timerctl.t0 = t;
	timerctl.next = t->timeout;
	return;
}
