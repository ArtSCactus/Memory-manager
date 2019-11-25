#include "mmemory.h"

int main() {
	_init(5, 5);
	VA ptr = (VA)calloc(2);
	_malloc(ptr, 5);
	_write(ptr, "some", 4);
	void* Buf = (void*) malloc(4);
	// Buf  =
		 _read(ptr, &Buf, 4);
		// char value[4] = {'s','o','m','e'};
		 //Buf = &value;
	char* charBuff = (char*) Buf;
	memory_status();
	printf("\n--------READING IN BUFFER TEST---------\n");
	printf("Value: ");
	for (int index = 0; index < 4; index++)
		printf("%c ", charBuff[index]);
	printf("\n-------------------------\n");
}

int malloc_test() {
	printf("=========================MALLOC TEST========================\n");
	_init(5, 5);
	memory_status();
	VA ptr = (VA) calloc(10);
	_malloc(&ptr, 3);
	memory_status();
	if (ptr[0] != 0 | ptr[1] != 2) {
		printf("Malloc test: FAILED;  VALUE: %i %i\n", ptr[0],ptr[1]);
	}
	else {
		printf("Malloc test: PASSED\n");
	}
}

int free_test() {
	printf("=========================FREE TEST========================\n");
	_init(5, 5);
	memory_status();
	VA ptr = _malloc(&ptr, 3);
	memory_status();
	_free(&ptr);
	memory_status();
}



int  initErrorTest() {
	if (_init(0, 0)<0) {
		printf("Illegal arguments test: PASSED\n");
	}
	else {
		printf("Illegal arguments test: FAILED\n");
	}
}

int stress_test() {
	_init(5, 5);
	memory_status();
	VA ptr = _malloc(&ptr, 5);
	_write(ptr, "Somee", 5);
	VA ptr1 = _malloc(&ptr1, "Somee", 5);
	_write(&ptr1, "Somee", 5);
	VA ptr2 = _malloc(&ptr2, "Somee", 5);
	_write(&ptr2, "Somee", 5);
	VA ptr3 = _malloc(&ptr3, "Somee", 5);
	_write(&ptr3, "Somee", 5);
	VA ptr4 = _malloc(&ptr4, "Somee", 4);
	_write(&ptr4, "Somee", 5);
	printf("=========================STRESS TEST=======================\n");
	memory_status();
}
