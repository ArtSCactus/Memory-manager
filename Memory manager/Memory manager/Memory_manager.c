#include <stddef.h>
#include <stdbool.h>
#include <malloc.h>
#include "mmemory.h"
#include "Memory_manager.h"

typedef char* VA;

typedef struct block {
	size_t size;
	VA address;
	bool free;
}Block;

typedef struct RAM {
	int total_memory;
	int free_memory;
	Block Blocks[];
}RAM;

RAM ram;

int length(int Array[]) {
	return sizeof(Array)/sizeof(Array[0]);
}

/**
@func	_malloc
@brief	�������� ���� ������ ������������� �������

@param	[out] ptr		������ �����
@param	[in]  szBlock	������ �����

@return	��� ������
@retval	0	�������� ����������
@retval	-1	�������� ���������
@retval	-2	�������� ������
@retval	1	����������� ������
**/
_malloc(VA* ptr, size_t szBlock)
{
	if (ram.free_memory < szBlock) return -2;
	if (szBlock <= 0 || length(ptr) == 0) return -1;
	Block new_block;
	new_block.address = ptr;
	new_block.size = szBlock;
	new_block.free = false;
	ram.free_memory -= szBlock;
	return 0;
};



/**
@func	_free
@brief	�������� ����� ������

@param	[in] ptr		������ �����

@return	��� ������
@retval	0	�������� ����������
@retval	-1	�������� ���������
@retval	1	����������� ������
**/
_free(VA ptr) 
{
	for (int index = 0; index < length(ram.Blocks); index++) {
		Block temp = ram.Blocks[index];
		if (temp.address == ptr) {
			ram.Blocks[index].free = true;
			ram.free_memory += ram.Blocks[index].size;
			return 0;
		}
	}
	return -1;
};



/**
@func	_read
@brief	������ ���������� �� ����� ������

@param	[in] ptr		������ �����
@param	[in] pBuffer	������ ������ ���� ���������� ���������
@param	[in] szBuffer	������ ������

@return	��� ������
@retval	0	�������� ����������
@retval	-1	�������� ���������
@retval	-2	������ �� ������� �����
@retval	1	����������� ������
**/
_read(VA ptr, void* pBuffer, size_t szBuffer) {

};



/**
@func	_write
@brief	������ ���������� � ���� ������

@param	[in] ptr		������ �����
@param	[in] pBuffer	������ ������ ���� ���������� ���������
@param	[in] szBuffer	������ ������

@return	��� ������
@retval	0	�������� ����������
@retval	-1	�������� ���������
@retval	-2	������ �� ������� �����
@retval	1	����������� ������
**/
_write(VA ptr, void* pBuffer, size_t szBuffer) {

};



/**
@func	_init
@brief	������������� ������ ��������� ������

@param	[in] n ���������� �������
@param	[in] szPage	������ ��������

� �������� 1 � 2 ����� ����� ������ ������������� ��� n*szPage

@return	��� ������
@retval	0	�������� ����������
@retval	-1	�������� ���������
@retval	1	����������� ������
**/
_init(int n, int szPage) {
	if (n <= 0 || szPage <= 0) return -1;
	ram.total_memory = n*szPage;
	Block *a; //��������� �� ������
	Block i, n;
	a = (Block*)malloc(n * sizeof(Block));

	return 0;
};


//����� - ��� ���� ������!!! 
//������ ����� malloc - ��� ����� �������. ������ ����� free - ��� �������� ��������. 
//������ ������� - ��� ����� ������, ������ �� ������� ������� �� 1 �����. char* VA ��� ������, ������� ������������� � long � ������������  ������ ��� �������� �����������.
//������ �� ��������� ��������� �� ������ �� 1 �����. ������ ����� �������� � �������� ������. 
//��� ������ write �� ��������� char-->int--> �������� ���, ����� ����������� ������������������ ������ � 4 ����� (���������) � ���������� ����. read ������ �������� ���������� ��������. 


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