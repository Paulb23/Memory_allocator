#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "memory_allocator/memory_manager.h"

void *remalloc(uint32_t size) {
	return malloc(size);
}

int main(int argc, char *argv[]) {
	clock_t begin, end;
	float time_spent;

	int items = 1000;
	double size = 64;

	if ( argc != 2 ) {
		return 0;
	} else if (atoi(argv[1]) == 1) {

		memory_manager_init();
		begin = clock();
	  	 	for (int i = 0; i < items; ++i) {
	  	 	  	void *l  = memory_manager_allocate(size);  
	  	 	  	memory_manager_free(l);
	  	 	  	size++;
	  	 	}  
	  	end = clock();
	  	time_spent = ((float)(begin - end)) / CLOCKS_PER_SEC;
	  	printf("custom time: %lf \n", time_spent);
	    memory_manager_shutdown();

	} else if (atoi(argv[1]) == 2) {
	    begin = clock();
	  	 for (int i = 0; i < items; ++i) {
	  	 	  	void *l = remalloc(size);  
	  	 	  	free(l);
	  	 	  	size++;
	  	  }  
	  	end = clock();
	  	time_spent = ((float)(begin - end)) / CLOCKS_PER_SEC;
	  	 printf("malloc time: %lf \n", time_spent);
	}
   return 0;
}
