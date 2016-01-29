#include<stdio.h>
#include "memory_allocator/memory_manager.h"

int main(void) {
	memory_manager_init();
    	void *memory = memory_manager_allocate(32);     
    	memory_manager_free(memory); 
    memory_manager_shutdown();
   return 0;
}
