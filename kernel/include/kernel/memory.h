#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>
#include <stddef.h>

#define MEMMAN_FREES		4090
#define MEMMAN_ADDR			0x003c0000

typedef struct {
    uint32_t addr;
    uint32_t size;
} __attribute__((packed)) freeinfo;

typedef struct
{
	uint32_t frees;
	uint32_t maxfrees;
	uint32_t lostsize;
	uint32_t losts;
	freeinfo free[MEMMAN_FREES];
} __attribute__((packed)) memman;

void init_memman(memman *man);
uint32_t memman_total(memman *man);
uint32_t memman_alloc(memman *man, size_t size);
int memman_free(memman *man, uint32_t addr, size_t size);
uint32_t memman_alloc_4k(memman *man, size_t size);
int memman_free_4k(memman *man, uint32_t addr, uint32_t size);

#endif _MEMORY_H_
