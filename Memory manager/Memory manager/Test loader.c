#include "mmemory.h"

int main() {
	//VA ptr;
	//void* Buf = (char*)malloc(4);
	//VA wrongPtr = (VA)calloc(2, sizeof(VA));
	//wrongPtr[0] = -1;
	//wrongPtr[1] = 26;
	//assertEquals(-1, _init(0, 0), "Wrong argument init test");
	//assertEquals(0, _init(5, 5), "Init test");
	//assertEquals(-2, _malloc(&ptr, 26), "Out of memory");
	//assertEquals(-1, _malloc(&ptr, 0), "Wrong arguments malloc test");
	//assertEquals(0, _malloc(&ptr, 5), "Malloc test");
	//assertEquals(-2, _write(ptr, "some", 6), "Out of bounds write test");
	//assertEquals(-1, _write(ptr, "some", 0), "Wrong argument write test");
	//assertEquals(0, _write(ptr, "some", 4), "Write test");
	//assertEquals(-1, _read(ptr, Buf, 0), "Wrong argument read test");
	//assertEquals(-2, _read(wrongPtr, Buf, 4), "Out of bounds read test");
	//assertEquals(0, _read(ptr, Buf, 4), "Read test");
	//assertEquals(-1, _free(wrongPtr), "Free wrong argument test");
	//assertEquals(0, _free(ptr), "Free test");
	////memory_status();
	//test_run();
	defrag_test();
}

int test_run() {
	VA ptr;
	VA ptr1;
	_init(5, 5);
//	_malloc(&ptr, 1);
	_malloc(&ptr, 4);
	_malloc(&ptr1, 10);
	_write(ptr, "some", 4);
	_write(ptr1, "hello", 6);
	void* Buf = (char*) malloc(4);
	_read(ptr, Buf, 4);
	void* Buf1 = (char*)malloc(4);
	_read(ptr1, Buf1, 4);
	
	char* charBuff = (char*)Buf;
	memory_status();
	printf("\n--------READING IN BUFFER TEST---------\n");
	printf("Value: ");
	for (int index = 0; index < 4; index++)
		printf("%c ", charBuff[index]);
	printf("\n-------------------------\n");
	charBuff = (char*)Buf1;
	printf("\n--------READING IN BUFFER TEST---------\n");
	printf("Value: ");
	for (int index = 0; index < 4; index++)
		printf("%c ", charBuff[index]);
	printf("\n-------------------------\n");
}

int defrag_test() {
	VA ptr;
	VA ptr1;
	_init(5, 10);
	_malloc(&ptr, 4);
	_write(ptr, "aaaa", 4);
	_malloc(&ptr1, 5);
	_write(ptr1, "bbbbb", 5);
	_free(ptr);
	VA ptr2;
	_malloc(&ptr2, 6);
	_write(ptr2, "cccccccccc", 6);
	VA ptr3;
	_malloc(&ptr3, 2);
	_write(ptr3, "dd", 2);
	memory_status();
}

int assertEquals(int correct, int methodResult, char* testName) 
{
	if (correct == methodResult) {
		printf("%s: PASSED\n", testName);
	}
	else {
		printf("%s: FAILED\n", testName);
	}
}
