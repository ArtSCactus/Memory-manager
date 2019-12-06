#include <stdio.h>
typedef char* VA;

typedef struct SegmentLine { // ��������� ��� ����������� ������ � ����� ������ ��� �������� �� ������ findSpace()
	int id_begining;
	int id_end;
}SegmentLine;

typedef struct block {
	void* value;
	int free;
	int segment_id;
}Byte;

typedef struct Segment {
	int id_begining;
	int id_end;
	int segment_id;
}Segment;

typedef struct RAM {
	int total_memory;
	int free_memory;
	Byte* Blocks;
	Segment* segments;
	Segment last_requested_segment;
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
	if (szBlock <= 0) return -1;
	ram.free_memory -= szBlock;
	SegmentLine spaceForSegment;
	int isBlockLineFree = 1;
	for (int startIndex = 0; startIndex + (int)szBlock < ram.total_memory; startIndex++) {
		spaceForSegment.id_begining = startIndex;
		spaceForSegment.id_end = (startIndex + szBlock) - 1;
		isBlockLineFree = 1;
		for (int currentBlockIndex = startIndex; currentBlockIndex <= spaceForSegment.id_end; currentBlockIndex++) {
			if (ram.Blocks[currentBlockIndex].free == 0 || ram.Blocks[currentBlockIndex].segment_id != -1) {
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
	return addSegment(ptr, spaceForSegment.id_begining, spaceForSegment.id_end, findSegmentID());
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
	if (ptr == NULL ) {
		return -1;
}
	if (ram.total_memory < (int) ptr[0] ||   0 > (int) ptr[0]) {
		return -1;
	}
	if (ram.total_memory < (int) ptr[1] || 0 >(int) ptr[1]) {
		return -1;
	}
	deleteSegment(ptr);
	free(ptr);
	return 0;
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
	if (ptr == NULL) {
		return -1;
	}
	if (szBuffer <= 0) {
		return -1;
	}
	if (ram.total_memory < (int) ptr[0] || 0 >(int) ptr[0]) {
		return -2;
	}
	if (ram.total_memory < (int) ptr[1] || 0 >(int) ptr[1]) {
		return -2;
	}
	Segment allocatedSegment;

	if (ram.segment_amount == 1) {
		ram.last_requested_segment = ram.segments[0];
	}

	if (ram.last_requested_segment.id_begining == ptr[0] && ram.last_requested_segment.id_end == ptr[1]) {
		allocatedSegment = ram.last_requested_segment;
	}
	else {
		printf("%i %i\n", ram.segments[0].id_begining, ram.segments[0].id_end);
		printf("%i %i\n", ram.segments[1].id_begining, ram.segments[1].id_end);
		allocatedSegment = ram.segments[getSegmentArrayIndex(ram.Blocks[ptr[0]].segment_id)];
	}
	/*if (allocatedSegment.id_begining + (szBuffer-1) > allocatedSegment.id_end) {
		return -2;
	}*/
	ram.last_requested_segment = allocatedSegment;
	//printf("ID END STATUS: %i\n", allocatedSegment.id_end);
	if (allocatedSegment.id_begining + (int) szBuffer > allocatedSegment.id_end+1) {
		printf("OUT OF SEGMENT POINT: %i\n", allocatedSegment.id_begining + (int)szBuffer);
		printf("ID END STATUS: %i\n", allocatedSegment.id_end);
	
		//return -2;
	}
	char* temp = (char*) malloc(szBuffer);

	int tempArrayIndex = 0;
	//printf("READ DEBUG: \n");
	for (int index = allocatedSegment.id_begining; index <= (int) allocatedSegment.id_end; index++, tempArrayIndex++) {
		temp[tempArrayIndex] = (char) ram.Blocks[index].value;
		//printf("Byte[%i] value: %c\n", index, ram.Blocks[index].value);
		if (index > allocatedSegment.id_end) {
			return -2;
		}
	}
	memcpy(pBuffer, temp, szBuffer);
	return 0;
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
	if ( (int) szBuffer <= 0) {
		return -1;
	}
	Segment allocatedSegment; //= ram.segments[getSegmentArrayIndex(ram.Blocks[ptr[0]].segment_id)];
	if (ram.segment_amount == 1) {
		ram.last_requested_segment = ram.segments[0];
	}

	if (ram.last_requested_segment.id_begining == ptr[0] && ram.last_requested_segment.id_end == ptr[1]) {
		allocatedSegment = ram.last_requested_segment;
	}
	else {
		allocatedSegment = ram.segments[getSegmentArrayIndex(ram.Blocks[ (int) ptr[0]].segment_id)];
	}
	/*if (allocatedSegment.id_begining + (szBuffer - 1) > allocatedSegment.id_end) {
		return -2;
	}*/
	if (allocatedSegment.id_begining + (szBuffer - 1) > allocatedSegment.id_end) {
		return -2;
	}
	ram.last_requested_segment = allocatedSegment;
	char* temp = (char*) pBuffer;
	int bufferIndex = 0;
	for (int index = allocatedSegment.id_begining; index < allocatedSegment.id_begining + (int)szBuffer; index++, bufferIndex++) {
		ram.Blocks[index].free = 0;
		ram.Blocks[index].value = temp[bufferIndex];//&pBuffer;
	//	printf("Byte[%i] value: %c   Segment{id=%i, id_beg=%i, id_end=%i, id_beg+szBuffer=%i}\n", index, temp[bufferIndex], allocatedSegment.segment_id, allocatedSegment.id_begining, (int)(allocatedSegment.id_begining + szBuffer));
	}
	printf("\n");
	return 0;
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
	ram.total_memory = n * szPage;
	ram.segment_amount = 0;
	ram.free_memory = ram.total_memory;
	ram.Blocks = (Byte*) calloc(ram.total_memory, sizeof(Byte));
	ram.segments = (Segment*) calloc(1, sizeof(Segment));
	for (int index = 0; index < ram.total_memory; index++) {
		ram.Blocks[index] = (Byte){ 0,1,-1 };
	}
	return 0;
};

addSegment(VA* ptr, int seg_start, int seg_end, int seg_id)
{
     *ptr =(VA) malloc(2 * sizeof(VA));
	// ����� �������
	Segment newSegment = { seg_start, seg_end, seg_id };
	Segment* newSegmentList;
	ram.segment_amount++;
	//����������� ������ ��� ����������� ������ ���������
	newSegmentList = (Segment*) calloc(ram.segment_amount, sizeof(Segment));

	memcpy(newSegmentList, ram.segments, ram.segment_amount * sizeof(Segment));

	// ��������� �����
	newSegmentList[ram.segment_amount - 1] = newSegment;
	// ������������� �������������� ������ � ��������
	for (int index = seg_start; index <= seg_end; index++) {
		ram.Blocks[index].segment_id = newSegment.segment_id;
		ram.Blocks[index].free = 1;
	}
	//��������� ������
	ram.segments = newSegmentList;
	memcpy(ram.segments, newSegmentList, ram.segment_amount * sizeof(Segment));
	//ram.segment_amount += 1;
	(*ptr)[0] = (int) newSegment.id_begining;// (49*(newSegment.id_end - newSegment.id_begining))*ram.total_memory;
	(*ptr)[1] = (int) newSegment.id_end;
	return 0;
}

int deleteSegment(VA ptr)
{
	Segment currentSegment; //= ram.segments[0];
	for (int index = 0; index < ram.segment_amount; index++) {
		if (ram.segments[index].segment_id == ram.Blocks[ptr[0]].segment_id) {
			currentSegment = ram.segments[index];
		}
	}

	for (int index = currentSegment.id_begining; index <= currentSegment.id_end; index++) {
		ram.Blocks[index].segment_id = -1;
		ram.Blocks[index].free = 1;
		ram.Blocks[index].value = NULL;
	}

	int memoryCounter = currentSegment.id_end - currentSegment.id_begining;
	//��������� ����� ������ ���������
	Segment* newList;
	if (ram.segment_amount == 1) {
		newList = ram.segments;
	}
	else {
		newList = (Segment*)calloc(ram.segment_amount - 1);
	}
	//� ���� ������ ��� ��������, ����� ����, ��� ����� ������ id
	for (int index = 0; index < ram.segment_amount; index++) {
		//ram.Blocks[index].segment_id = -1;
		if (ram.segments[index].segment_id != ram.Blocks[ptr[0]].segment_id) {
			newList[index] = ram.segments[index];
		}
	}
	//��������� ���������� ���������
	ram.segment_amount--;
	ram.free_memory += memoryCounter;
	//����������� ������ �� ������� ������
	memcpy(ram.segments, newList, ram.segment_amount * sizeof(Segment));
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
		printf("Byte[%i] seg_id:%i status:%i value:%i\n", index, ram.Blocks[index].segment_id, ram.Blocks[index].free, ram.Blocks[index].value);
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

int getSegmentArrayIndex(int ID) {
	for (int index = 0; index < ram.segment_amount; index++) {
		if (ram.segments[index].segment_id == ID) {
			return index;
		}
	}
	return -1;
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
// _malloc ������ ������� � ���������� ������ �� ����
 // write ���������� �� ���� ������ ���������� ��������
 // ���� �������� �� ���� ������, �� ���������� ������ ������ � ��������� ����
