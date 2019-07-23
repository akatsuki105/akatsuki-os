#include <kernel/timer.h>
#include <kernel/asm.h>

timercontroller timerctl;

void init_pit(void)
{
    int i;
    struct TIMER *t;
    outb(PIT_CTRL, 0x34);
    outb(PIT_CNT0, 0x9c);
    outb(PIT_CNT0, 0x2e);
    timerctl.count = 0;
    for (i = 0; i < MAX_TIMER; i++) {
        timerctl.timers0[i].flags = 0;
    }
    // 番兵
    t = timer_alloc();
    t->timeout = 0xffffffff;
    t->flags = TIMER_FLAGS_USING;
    t->next = 0;
    timerctl.t0 = t;
    timerctl.next = 0xffffffff;
    return;
}

struct TIMER *timer_alloc(void)
{
    int i;
	for (i = 0; i < MAX_TIMER; i++) {
		if (timerctl.timers0[i].flags == 0) {
			timerctl.timers0[i].flags = TIMER_FLAGS_ALLOC;
			return &timerctl.timers0[i];
		}
	}
	return 0;
}

void timer_free(struct TIMER *timer)
{
    timer->flags = 0;
    return;
}

void init_timer(struct TIMER *timer, struct FIFO32 *fifo, int data)
{
    timer->fifo = fifo;
    timer->data = data;
    return;
}

void timer_settime(struct TIMER *timer, uint32_t timeout)
{
    int e;
    struct TIMER *t, *s;
    timer->timeout = timeout + timerctl.count;
    timer->flags = TIMER_FLAGS_USING;
    e = load_eflags();
    io_cli();
    t = timerctl.t0;
    // 設定するタイマーが最初のタイマーとなるとき
    if (timer->timeout <= t->timeout) {
        timerctl.t0 = timer;
        timer->next = t;
        timerctl.next = timer->timeout;
        store_eflags(e);
        return;
    }
    // タイマーを差し込む
    for (;;) {
        s = t;
        t = t->next;
        if (timer->timeout <= t->timeout) {
            s->next = timer;
            timer->next = t;
            store_eflags(e);
            return;
        }
    }
}
