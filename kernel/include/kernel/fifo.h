#ifndef _FIFO_H_
#define _FIFO_H_

struct FIFO32 {
	int *buf;
	int p, q, size, free, flags;
};
void init_fifo32(struct FIFO32 *fifo, int size, int *buf);
int fifo32_put(struct FIFO32 *fifo, int data);
int fifo32_get(struct FIFO32 *fifo);
int fifo32_status(struct FIFO32 *fifo);

extern struct FIFO32 keyfifo;
extern int keyfifo_buf[128];

#endif _FIFO_H_