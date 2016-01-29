/*-------------------------------------------------------------------------*/
/**
   @file    memory_manager.c
   @author  P. Batty
   @brief   Manages the memory
   This module implements the memory manager
*/
/*--------------------------------------------------------------------------*/

#include "memory_allocator/memory_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*------------------------*/
//	defines varables	  //
/*------------------------*/  

#define ALIGN_SIZE( size )		( ((size) + ALIGN - 1) & ~(ALIGN - 1) )


/*------------------------*/
//	Private varables	  //
/*------------------------*/   

enum {
	ALIGN = 8
};

static uint32_t pageSize;									/* the page size */

static void 			 *smallFirstFree[256 / ALIGN + 1 ];	/* small heap allocator (for allocs of 1-255 bytes) */
static small_memory_page *lastSmallUsedPage;  				/* last used page */
static small_memory_page *currentSmallPage; 				/* the current page we are using */
static uint32_t 		  currentSmallPageOffset;   		/* the offset on the current page */


/*------------------------*/
//	Private methods		  //
/*------------------------*/   

static small_memory_page *allocate_os_page( const uint32_t size );	/* allocate a page from OS */
static void free_os_page( small_memory_page *page ); 				/* free's a page from the OS */

static void *allocate_small_entry( const uint32_t size ); 			/* alloctes a small memory */

static void free_small_entry( void *entry );						/* frees a memory page */   
	

/*------------------------*/
//	Interface methods	  //
/*------------------------*/ 

/*
 Initilises the module.
*/
void memory_manager_init() {
	pageSize = 65536 - sizeof( small_memory_page ); /* default page size,  16 bit max = 65536*/

	memset( smallFirstFree, 0, sizeof( smallFirstFree ) );

	lastSmallUsedPage = NULL;
	currentSmallPageOffset = ALIGN_SIZE( 0 );
	currentSmallPage = allocate_os_page( pageSize );
}

/*
 Shutdown the module
*/
void memory_manager_shutdown() {
	if (currentSmallPage) {
		free_os_page(currentSmallPage);
	}

	small_memory_page *page = lastSmallUsedPage;
	while(page) {
		small_memory_page *next = page->next;
		free_os_page(page);
		page = next;
	}
}

#ifndef DEBUG

void *memory_manager_allocate(const uint32_t size) {
	if (!size) {
		return NULL;
	}
	if (!(size & ~255)) {
		return allocate_small_entry(size);
	}
	return NULL;
}

void memory_manager_free(void *memory) {
	if (!memory) {
		return;
	}
	free_small_entry(memory);
}

#else

#undef memory_manager_allocate
#undef memory_manager_dump
#undef memory_manager_free

void *memory_manager_allocate(const uint32_t size, const char *filename, const int lineNumber) {
	printf("Allocating: %lu from: %s %i \n", size, filename, lineNumber);
}

void memory_manager_free(void *memory, const char *filename, const int lineNumber) {
	printf("Free from: %s %i \n", filename, lineNumber);
	
}

void memory_manager_dump(void *memory, const char *filename, const int lineNumber) {

}

#endif


/*------------------------*/
//	private methods	 	  //
/*------------------------*/ 

/*
 Allocates a OS page
  size = size in bytes
  returns small_memory_page
*/
static small_memory_page *allocate_os_page( const uint32_t size ) {
	small_memory_page *page = malloc( (size + sizeof( small_memory_page )) + ALIGN - 1 ); /* create the page, plus struct size */
	
	if (!page) {
		exit(0);
	}

	page->memory = (void *) ALIGN_SIZE( (int32_t)((int8_t)(page) + sizeof( small_memory_page ) ));					
	page->size = size;				
	page->next = NULL;
	page->previous = NULL;
	page->largestFreeEntry = 0;
	page->firstFreeEntry = NULL;

	return page;
}

/*
 free's a page from the OS
  page = small_memory_page
*/
static void free_os_page( small_memory_page *page ) {
	if (!page) {
		return;
	}
	free(page);
}

/*
 Allocates a small entry.
  size = 1 - 255.
  returns pointer to entry.
*/
static void *allocate_small_entry( uint32_t size ) {

	if (size < sizeof( uint32_t ) ) {
		size = sizeof( uint32_t );
	}

	size = ALIGN_SIZE( size );

	int8_t *smallEntry = (int8_t *)(smallFirstFree[size / ALIGN]);	/* allocate */
	if ( smallEntry ) {
		smallFirstFree[size / ALIGN] = (void *)(*smallEntry);
		return smallEntry;
	}

	uint32_t bytesLeft = pageSize - currentSmallPageOffset;	/* Check we have enough room */
	if (size >= bytesLeft) {								/* else allocate new OS pages */
		currentSmallPage->next = lastSmallUsedPage;
		lastSmallUsedPage = currentSmallPage;
		currentSmallPage = allocate_os_page( pageSize );
		if (!currentSmallPage) {
			return NULL;
		}
		currentSmallPageOffset = ALIGN_SIZE ( 0 );
	}

	smallEntry = ((int8_t *)currentSmallPage->memory) + currentSmallPageOffset;
	currentSmallPageOffset += size;
	return (smallEntry);
}

/* 
 frees a memory entry 
*/ 
void free_small_entry( void *entry ) {
//	int8_t *d = ( (int8_t *)entry );
//	uint32_t *dt = (uint32_t *)entry;

//	uint32_t ix = *d;
//	if (ix > (256 / ALIGN)) {
//		exit(0);
//	}
//
//	*dt = (uint32_t)smallFirstFree[ix];
//	smallFirstFree[ix] = (void *)d;
}			  