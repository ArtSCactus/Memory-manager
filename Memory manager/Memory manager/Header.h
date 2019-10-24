#pragma once
#include "mmemory.h"
typedef char* VA;

typedef struct block {
	VA value;
	int free;
	int segment_id;
}Block;

typedef struct RAM {
	int total_memory;
	int free_memory;
	Block* Blocks;
}RAM;

int memory_status();