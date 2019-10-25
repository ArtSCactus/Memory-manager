#include "List.h"
/*
struct List create() // а- значение первого узла
{
	List list;
	// выделение памяти под корень списка
	//list = (List*) сalloc(1, sizeof (List));
	list.size = 0;
	return(list);
}

Segment add(List list, int seg_start, int seg_end, int seg_id)
{
	 List temp;
	 Segment newSegment = {seg_start, seg_end, seg_id};
	temp.size = temp.size + 1;
	//temp = (List) malloc(temp.size);
	for (int index = 0; index < temp.size; index++) {
		temp.segments[index] = list.segments[index];
	}
	temp.segments[temp.size - 1] = newSegment;
	free(list);
	//return(temp);
}

struct List deleteSegment(List list, int segment_id)
{
	struct List temp;
	temp.size = temp.size + 1;
	//temp = (struct List) malloc(temp.size);
	for (int index = 0; index < temp.size; index++) {
		if (list.segments[index].segment_id != segment_id) {
			temp.segments[index] = list.segments[index];
		}
	}
	free(list);
	return(temp);
}

void deleteList(List list) 
{
	free(list);
}

void printList(List list)
{
	List segment_list = list;
	for (int index=0; index<list.size; index++)
		printf("Segment[%i]:  begins from block: %i  ends on block: %i\n", 
			segment_list.segments[index].segment_id, segment_list.segments[index].id_begining, segment_list.segments[index].id_end); // вывод значения элемента p
}

Segment getSegment(List list, int segment_id) 
{
	for (int index = 0; index < list.size; index++) {
		if (list.segments[index].segment_id == segment_id) {
			return list.segments[index];
		}
	}
}
*/