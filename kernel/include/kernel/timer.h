#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>
#include <stddef.h>
#include "fifo.h"

#define MAX_TIMER		500
#define PIT_CTRL	0x0043
#define PIT_CNT0	0x0040
#define TIMER_FLAGS_ALLOC		1
#define TIMER_FLAGS_USING		2

/*
 * next => 次のtimer
 * timeout => 予定時刻(count)
 * flags => 稼働中か
 */
struct TIMER {
	struct TIMER *next;
	uint32_t timeout, flags;
	struct FIFO32 *fifo;
	int data;
};

typedef struct {
    uint32_t count;
    uint32_t next;
    struct TIMER *t0;
    struct TIMER timers0[MAX_TIMER];
}__attribute__((packed)) timercontroller;

extern timercontroller timerctl;
void init_pit(void);
struct TIMER *timer_alloc(void);
void timer_free(struct TIMER *timer);
void init_timer(struct TIMER *timer, struct FIFO32 *fifo, int data);
void timer_settime(struct TIMER *timer, uint32_t timeout);

#endif _TIMER_H_