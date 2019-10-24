#include <stdbool.h>
#include "Header.h"

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
	Block new_block;
	new_block.free = 0;
	ram.free_memory -= szBlock;
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
	ram.Blocks = (Block*) malloc(n * szPage);
	Block defaultBlock = {0,0,0};
	for (int index = 0; index < ram.total_memory; index++) {
		ram.Blocks[index] = defaultBlock;
	}
	memory_status();
	return 0;
};

int memory_status() {
	for (int index = 0; index < ram.total_memory; index++) {
		printf("Block[%i] seg_id:%i status:%i\n",index,ram.Blocks[index].segment_id, ram.Blocks[index].free);
	}
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