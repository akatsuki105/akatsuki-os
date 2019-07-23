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
} __attribute__((packed)) memory_manager;

void init_memman(memory_manager *man);
uint32_t memman_total(memory_manager *man);
uint32_t memman_alloc(memory_manager *man, size_t size);
int memman_free(memory_manager *man, uint32_t addr, size_t size);
uint32_t memman_alloc_4k(memory_manager *man, size_t size);
int memman_free_4k(memory_manager *man, uint32_t addr, uint32_t size);

#endif _MEMORY_H_
