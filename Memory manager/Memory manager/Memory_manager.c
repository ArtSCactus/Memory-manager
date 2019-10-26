#include <stdio.h>
typedef char* VA;

typedef struct SegmentLine { // необходим для обозначения начала и конца строки при возврате из метода findSpace()
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
@brief	Удаление блока памяти

@param	[in] ptr		адресс блока

@return	код ошибки
@retval	0	успешное выполнение
@retval	-1	неверные параметры
@retval	1	неизвестная ошибка
**/
_free(VA ptr) 
{
	deleteSegment(ptr);
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
/*_free(VA* ptr)
{
	deleteSegment(ptr);
	return 0;
};*/



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
	for (int index = 0; index < ram.total_memory; index++) {
		ram.Blocks[index] =(Block) { 0,0,-1 };
	}
		_malloc('a',5);
		memory_status();
		_malloc('a', 6);
		memory_status();
	    _free(1); // для нормальной работы _free нужно написать _read и _write
		memory_status();
	return 0;
};

int addSegment(int seg_start, int seg_end, int seg_id)
{
	// новый сегмент
	Segment newSegment = { seg_start, seg_end, seg_id };
	Segment* newSegmentList;
		//резервируем память под увеличенный список сегментов
		newSegmentList = (Segment*)calloc(ram.segment_amount+1);
	//переносим сегменты из старого списка в новый
	for (int index = 0; index < ram.segment_amount; index++) {
		newSegmentList[index]=ram.segments[index];
	}
	// добавляем новый
	newSegmentList[ram.segment_amount - 1] = newSegment;
	// устанавливаем принадлежность блоков к сегменту
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
		 newList = (Segment*)calloc(ram.segment_amount-1);
	}
	//в него войдут все сегменты, кроме того, что имеет нужный id
	for (int index = 0; index < ram.segment_amount; index++) {
		ram.Blocks[index].segment_id = -1;
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

int findSegmentID() {
	int isExists = 0; // для контроля прохождения цикла, означает существование текущего id среди сегментов 0-false , 1 -true
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




//Буфер - это сами данные!!! 
//Каждый возов malloc - это новый сегмент. Каждый вызов free - это удаление сегмента. 
//Каждый сегмент - это набор блоков, каждый из которых состоит из 1 байта. char* VA это ссылка, которая преобразуется в long и используется  только как числовое обозначение.
//Память по умолчанию разделена на ячейки по 1 байту. Каждый адрес уникален и является числом. 
//При вызове write мы переводим char-->int--> бинарный код, затем подыскиваем последовательность блоков в 4 байта (свободных) и записываем туда. read должен выводить содержимое сегмента. 