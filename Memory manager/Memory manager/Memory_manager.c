#include <stdio.h>
typedef char* VA;

typedef struct SegmentLine { // ��������� ��� ����������� ������ � ����� ������ ��� �������� �� ������ findSpace()
	int id_begining;
	int id_end;
}SegmentLine;

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
	SegmentLine spaceForSegment;
	int isBlockLineFree = 1;
	for (int startIndex = 0; startIndex + (int) szBlock < ram.total_memory; startIndex++) {
		spaceForSegment.id_begining = startIndex;
		spaceForSegment.id_end = startIndex + szBlock;
		isBlockLineFree = 1;
		for (int currentBlockIndex = startIndex; currentBlockIndex < spaceForSegment.id_end; currentBlockIndex++) {
			if (ram.Blocks[currentBlockIndex].free == 1 || ram.Blocks[currentBlockIndex].segment_id != -1) {
				isBlockLineFree = 0;
				break;
			}
		}
		if (isBlockLineFree == 1) {
			break;
		}
		if (spaceForSegment.id_begining == ram.total_memory - 1 & spaceForSegment.id_end == ram.total_memory - 1) {
			return -2;
		}
	}
	

	if (spaceForSegment.id_begining == -1 & spaceForSegment.id_end == -1) {
		return -2;
	}
	addSegment(spaceForSegment.id_begining, spaceForSegment.id_end, findSegmentID());
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
	deleteSegment(ptr);
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
/*_free(VA* ptr)
{
	deleteSegment(ptr);
	return 0;
};*/



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
	for (int index = 0; index < ram.total_memory; index++) {
		ram.Blocks[index] =(Block) { 0,0,-1 };
	}
		_malloc('a',5);
		memory_status();
		_malloc('a', 6);
		memory_status();
	    _free(1); // ��� ���������� ������ _free ����� �������� _read � _write
		memory_status();
	return 0;
};

int addSegment(int seg_start, int seg_end, int seg_id)
{
	// ����� �������
	Segment newSegment = { seg_start, seg_end, seg_id };
	Segment* newSegmentList;
		//����������� ������ ��� ����������� ������ ���������
		newSegmentList = (Segment*)calloc(ram.segment_amount+1);
	//��������� �������� �� ������� ������ � �����
	for (int index = 0; index < ram.segment_amount; index++) {
		newSegmentList[index]=ram.segments[index];
	}
	// ��������� �����
	newSegmentList[ram.segment_amount - 1] = newSegment;
	// ������������� �������������� ������ � ��������
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
		 newList = (Segment*)calloc(ram.segment_amount-1);
	}
	//� ���� ������ ��� ��������, ����� ����, ��� ����� ������ id
	for (int index = 0; index < ram.segment_amount; index++) {
		ram.Blocks[index].segment_id = -1;
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

int findSegmentID() {
	int isExists = 0; // ��� �������� ����������� �����, �������� ������������� �������� id ����� ��������� 0-false , 1 -true
	for (int currentID = 0; ; currentID++) {
		isExists = 0;
		for (int index = 0; index < ram.segment_amount; index++) {
			if (currentID == ram.segments[index].segment_id) {
				isExists = 1;
				break;
			}
		}
		if (isExists == 0) {
			return currentID;
		}
	}
}

 SegmentLine findSpace(size_t size) {
    SegmentLine spaceForSegment;
	int isBlockLineFree = 1;
	for (int startIndex = 0; startIndex + size < ram.total_memory; startIndex++) {
		spaceForSegment.id_begining = startIndex;
		spaceForSegment.id_end = startIndex + size;
		isBlockLineFree = 1;
		for (int currentBlockIndex = startIndex; currentBlockIndex < spaceForSegment.id_end; currentBlockIndex++) {
			if (ram.Blocks[currentBlockIndex].free == 0 || ram.Blocks[currentBlockIndex].segment_id > -1) {
				isBlockLineFree = 0;
				break;
			}
		}
		if (isBlockLineFree == 1) {
			return spaceForSegment;
		}
	}
	spaceForSegment.id_begining = -1;
	spaceForSegment.id_end = -1;
	return spaceForSegment;
}




//����� - ��� ���� ������!!! 
//������ ����� malloc - ��� ����� �������. ������ ����� free - ��� �������� ��������. 
//������ ������� - ��� ����� ������, ������ �� ������� ������� �� 1 �����. char* VA ��� ������, ������� ������������� � long � ������������  ������ ��� �������� �����������.
//������ �� ��������� ��������� �� ������ �� 1 �����. ������ ����� �������� � �������� ������. 
//��� ������ write �� ��������� char-->int--> �������� ���, ����� ����������� ������������������ ������ � 4 ����� (���������) � ���������� ����. read ������ �������� ���������� ��������. 