#include <conio.h>
#include <stdio.h>
#include <stdbool.h>
#include "mmemory.h"
/*struct virtual_address {
	int segment_number;
	int segment_shift;
	//Физический адрес получается путем сложения базового адреса сегмента, который определяется по номеру сегмента g из таблицы сегментов и смещения s
};*/
int main()
{ 

	printf("%i",_init(2,2));
	int arr[] = {0,1,2};
	printf("%i", _length(arr));
	_getch();

}