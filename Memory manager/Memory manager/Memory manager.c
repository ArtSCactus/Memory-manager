#include <iostream> 
#include <stdbool.h>
struct RAM {
	int total_memory;
	struct segment *segments[];
};
struct hard_drive {
	int total_memory;
};
struct process{
	int porcess_id;
	int process_memory_size;
	bool terminated;
};
struct segment {
	int size;
	int data_address[];
};
struct virtual_address {
	int segment_number;
	int segment_shift;
	//Физический адрес получается путем сложения базового адреса сегмента, который определяется по номеру сегмента g из таблицы сегментов и смещения s
};
int _malloc(struct RAM ram, struct hard_drive hdd,struct process new_process) {
	

}
int _init(struct RAM ram, struct hard_drive hdd, int hard_drive_memory, int RAM_memory) {
	if (hard_drive_memory <= 0  || RAM_memory<=0) return;
	ram.total_memory = RAM_memory;
	hdd.total_memory = hard_drive_memory;
}
int main()
{ 
    
}