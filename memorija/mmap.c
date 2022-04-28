#include <stdlib.h>
#include <stdio.h>

int main () {

    printf("[before malloc] Look at /proc/%d/maps\n", getpid());
    getchar();
/**	
	int CHUNK_NUM = 200;
	char* addr_small[CHUNK_NUM];
	for (int i = 0; i < CHUNK_NUM; i++){
		addr_small[i] = (char*) malloc(1024);
	}
**/
	char * addr;
	addr = (char*) malloc(2000 * 1024);
	
    printf("[after malloc] Look at /proc/%d/maps\n", getpid());
    getchar();
    
/**
	for (int i = 0; i < CHUNK_NUM; i++){
		free(addr_small[i]);
	}
**/
	free(addr);

    printf("[after free] Look at /proc/%d/maps\n", getpid());
    getchar();

    return 0;

}
