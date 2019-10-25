//#include "Header.h"
#include <stdio.h>
typedef char* VA;

typedef struct block {
	VA value;
	int free;
	int segment_id;
}Block;

typedef struct Segment {
	int id_begining;
	int id_end;
	int segment_id;
}Segment;

typedef struct RAM {
	int total_memory;
	int free_memory;
	Block* Blocks;
	Segment* segments;
	int segment_amount;
	//List Segment_list;
}RAM;
RAM ram;

/**
@func	_malloc
@brief	Выделяет блок памяти определенного размера

@param	[out] ptr		адресс блока
@param	[in]  szBlock	размер блока

@return	код ошибки
@retval	0	успешное выполнение
@retval	-1	неверные параметры
@retval	-2	нехватка памяти
@retval	1	неизвестная ошибка
**/
_malloc(VA* ptr, size_t szBlock)
{
	if (ram.free_memory < szBlock) return -2;
	if (szBlock <= 0 ) return -1;
	ram.free_memory -= szBlock;
	addSegment(0, szBlock, 1);
	return 0;
};



/**
@func	_free
@brief	Удаление блока памяти

@param	[in] ptr		адресс блока

@return	код ошибки
@retval	0	успешное выполнение
@retval	-1	неверные параметры
@retval	1	неизвестная ошибка
**/
_free(VA ptr) 
{
	for (int index = 0; index < ram.total_memory; index++) {
		Block temp = ram.Blocks[index];
		if (temp.value == ptr) {
			ram.Blocks[index].free = 1;
			ram.free_memory += 1;
			return 0;
		}
	}
	return -1;
};



/**
@func	_read
@brief	Чтение информации из блока памяти

@param	[in] ptr		адресс блока
@param	[in] pBuffer	адресс буфера куда копируется инфомация
@param	[in] szBuffer	размер буфера

@return	код ошибки
@retval	0	успешное выполнение
@retval	-1	неверные параметры
@retval	-2	доступ за пределы блока
@retval	1	неизвестная ошибка
**/
_read(VA ptr, void* pBuffer, size_t szBuffer) {

};



/**
@func	_write
@brief	Запись информации в блок памяти

@param	[in] ptr		адресс блока
@param	[in] pBuffer	адресс буфера куда копируется инфомация
@param	[in] szBuffer	размер буфера

@return	код ошибки
@retval	0	успешное выполнение
@retval	-1	неверные параметры
@retval	-2	доступ за пределы блока
@retval	1	неизвестная ошибка
**/
_write(VA ptr, void* pBuffer, size_t szBuffer) {

};



/**
@func	_init
@brief	Инициализация модели менеджера памяти

@param	[in] n количество страниц
@param	[in] szPage	размер страницы

В варианте 1 и 2 общий объем памяти расчитывается как n*szPage

@return	код ошибки
@retval	0	успешное выполнение
@retval	-1	неверные параметры
@retval	1	неизвестная ошибка
**/
_init(int n, int szPage) {
	if (n <= 0 || szPage <= 0) return -1;
	ram.total_memory = n*szPage;
	ram.segment_amount = 0;
	ram.free_memory = ram.total_memory;
	ram.Blocks = (Block*) calloc(ram.total_memory);
	//ram.Segment_list = create();
	for (int index = 0; index < ram.total_memory; index++) {
		ram.Blocks[index] =(Block) { 0,0,-1 };
	}
		_malloc('a',5);
		memory_status();
		deleteSegment(1);
		memory_status();
	return 0;
};

int addSegment(int seg_start, int seg_end, int seg_id)
{
	// новый сегмент
	Segment newSegment = { seg_start, seg_end, seg_id };
	Segment* newSegmentList;
	//резервируем память под увеличенный список сегментов
	newSegmentList = (Segment*) calloc(ram.segment_amount);
	//переносим сегменты из старого списка в новый
	for (int index = 0; index < ram.segment_amount; index++) {
		newSegmentList[index]=ram.segments[index];
	}
	// добавляем новый
	newSegmentList[ram.segment_amount - 1] = newSegment;
	for (int index = seg_start; index <seg_end; index++) {
		ram.Blocks[index].segment_id = newSegment.segment_id;
	}
	//обновляем список
	//free(ram.segments);
	ram.segments = newSegmentList;
	ram.segment_amount++;

}

int deleteSegment(int segment_id)
{
	Segment currentSegment = ram.segments[0];
	for (int index = 0; index < ram.segment_amount; index++) {
		if (ram.segments[index].segment_id == segment_id) {
			currentSegment=ram.segments[index];
		}
	}

	for (int index = 0; index < 5; index++) {
	ram.Blocks[index].segment_id = -1;
	}
	int memoryCounter = currentSegment.id_end - currentSegment.id_begining;
	//формируем новый список сегментов
	Segment* newList;
	if (ram.segment_amount == 1) {
		 newList = ram.segments;
	}
	else {
		 newList = (Segment*)calloc(ram.segment_amount);
	}
	//в него войдут все сегменты, кроме того, что имеет нужный id
	for (int index = 0; index < ram.segment_amount; index++) {
		if (ram.segments[index].segment_id != segment_id) {
			newList[index] = ram.segments[index];
		}
	}
	//уменьшаем количество сегментов
	ram.segment_amount--;
	ram.free_memory += memoryCounter;
	//освобождает память от старого списка
	//free(ram.segments);
	ram.segments = newList;
}

void deleteList()
{
	free(ram.segments);
}

void printList()
{
	//List segment_list = list;
	for (int index = 0; index < ram.segment_amount; index++)
		printf("Segment[%i]:  begins from block: %i  ends on block: %i\n",
			ram.segments[index].segment_id, ram.segments[index].id_begining, ram.segments[index].id_end); // вывод значения элемента p
}

Segment getSegment(int segment_id)
{
	for (int index = 0; index < ram.segment_amount; index++) {
		if (ram.segments[index].segment_id == segment_id) {
			return ram.segments[index];
		}
	}
}

int memory_status() {
	printf("Total memory{%i}, Free memory{%i}\n", ram.total_memory, ram.free_memory);
	for (int index = 0; index < ram.total_memory; index++) {
		printf("Block[%i] seg_id:%i status:%i value:%i\n",index,ram.Blocks[index].segment_id, ram.Blocks[index].free, ram.Blocks[index].value);
	}
	printf("\n");
}

//Буфер - это сами данные!!! 
//Каждый возов malloc - это новый сегмент. Каждый вызов free - это удаление сегмента. 
//Каждый сегмент - это набор блоков, каждый из которых состоит из 1 байта. char* VA это ссылка, которая преобразуется в long и используется  только как числовое обозначение.
//Память по умолчанию разделена на ячейки по 1 байту. Каждый адрес уникален и является числом. 
//При вызове write мы переводим char-->int--> бинарный код, затем подыскиваем последовательность блоков в 4 байта (свободных) и записываем туда. read должен выводить содержимое сегмента. 


/*
#include<stdio.h>
#include<stdbool.h>

typedef struct __s_block {
	struct __s_block *next;
	bool isfree;
	size_t size;
	void *memoryAddress;
}_SBLOCK;

#define BLOCK_SIZE sizeof(_SBLOCK)

_SBLOCK *allocateMemBlock(size_t size)
{
	_SBLOCK *block = (_SBLOCK*)sbrk(0);
	void *memadr = (void*)sbrk(0);
	void *allocate_mem = (void*)sbrk(BLOCK_SIZE + size);

	if (allocate_mem == (void*)-1) {
		return NULL;
	}
	else {
		block->next = NULL;
		block->isfree = false;
		block->size = size;
		block->memoryAddress = memadr + BLOCK_SIZE;
		return block;
	}
}

//allocate next memory block
void allocateNextMemBlock(size_t size, _SBLOCK **head)
{
	_SBLOCK *current = *head;
	void *allocate_mem = NULL;
	void *memadr = (void*)sbrk(0);

	if (current == NULL) {
		*head = allocateMemBlock(size);
	}
	else {
		while (current->next != NULL) {
			current = current->next;
		}
		_SBLOCK *newblock = sbrk(0);

		allocate_mem = (void*)sbrk(BLOCK_SIZE + size);
		if (allocate_mem == (void*)-1) {}
		else {
			newblock->next = NULL;
			newblock->isfree = false;
			newblock->size = size;
			newblock->memoryAddress = memadr + BLOCK_SIZE;
			current->next = newblock;
		}
	}
}

void freeMemBlock(_SBLOCK **head)
{
	if (*head == NULL) {}
	else {
		(*head)->isfree = true;
	}
}

void printMemBlocks(_SBLOCK *current)
{
	while (current != NULL) {
		printf("isfree = %d, size = %d, memoryAddress = %p, current = %p, next-node = %p\n",
			current->isfree, current->size, current->memoryAddress, current, current->next);
		current = current->next;
	}
}

int main()
{
	_SBLOCK *sMemBlock = NULL;
	allocateNextMemBlock(10, &sMemBlock);
	allocateNextMemBlock(35, &sMemBlock);
	allocateNextMemBlock(62, &sMemBlock);
	printMemBlocks(sMemBlock);

	printf("\nAfter freeing second node\n");
	freeMemBlock(&(sMemBlock->next));
	printMemBlocks(sMemBlock);

	return 0;
}
*/