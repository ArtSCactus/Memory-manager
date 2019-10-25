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
	if (szBlock <= 0 ) return -1;
	ram.free_memory -= szBlock;
	addSegment(0, szBlock, 1);
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
	// ����� �������
	Segment newSegment = { seg_start, seg_end, seg_id };
	Segment* newSegmentList;
	//����������� ������ ��� ����������� ������ ���������
	newSegmentList = (Segment*) calloc(ram.segment_amount);
	//��������� �������� �� ������� ������ � �����
	for (int index = 0; index < ram.segment_amount; index++) {
		newSegmentList[index]=ram.segments[index];
	}
	// ��������� �����
	newSegmentList[ram.segment_amount - 1] = newSegment;
	for (int index = seg_start; index <seg_end; index++) {
		ram.Blocks[index].segment_id = newSegment.segment_id;
	}
	//��������� ������
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
	//��������� ����� ������ ���������
	Segment* newList;
	if (ram.segment_amount == 1) {
		 newList = ram.segments;
	}
	else {
		 newList = (Segment*)calloc(ram.segment_amount);
	}
	//� ���� ������ ��� ��������, ����� ����, ��� ����� ������ id
	for (int index = 0; index < ram.segment_amount; index++) {
		if (ram.segments[index].segment_id != segment_id) {
			newList[index] = ram.segments[index];
		}
	}
	//��������� ���������� ���������
	ram.segment_amount--;
	ram.free_memory += memoryCounter;
	//����������� ������ �� ������� ������
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
			ram.segments[index].segment_id, ram.segments[index].id_begining, ram.segments[index].id_end); // ����� �������� �������� p
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