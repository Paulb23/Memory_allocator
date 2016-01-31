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
//	Private varables	  //
/*------------------------*/   

static uint32_t bucketSizes[] = {									/* 42 Diffrent bucket sizes must be multiple of two*/
	8,		16,		32,		48,		64,		80,		96,		112,
	128,	160,	192,	224,	256,	288,	320,	384,
	448,	512,	576,	640,	704,	768,	896,	1024,
	1168,	1360,	1632,	2048,	2336,	2720,	3264,	4096,
	4672,	5456,	6544,	8192,	9360,	10912,	13104,	16384,
	21840,	327684
};

static uint32_t numberOfBuckets = sizeof(bucketSizes);			/* number of buckets */

static memory_bucket buckets[sizeof(bucketSizes)];				/* buckets */


/*------------------------*/
//	Private methods		  //
/*------------------------*/   

static memory_page *allocate_os_page( const uint32_t size );	/* allocate a page from OS */
static void free_os_page( memory_page *page ); 					/* free's a page from the OS */

static void *allocate_entry( const uint32_t size ); 			/* alloctes a small entry */

static void free_entry( memory_page *entry , const uint32_t size);		/* frees a entry */   

/*------------------------*/
//	Interface methods	  //
/*------------------------*/ 

/*
 Initilises the module.
*/
void memory_manager_init() {
	for (uint32_t i = numberOfBuckets + 1; i > 0; --i) {		/* initilise starting buckets*/
		buckets[i].count = 1;
		buckets[i].memoryPageSize = bucketSizes[i];

			memory_page *firstPage = allocate_os_page(bucketSizes[i]);;
			memory_page *currentPage = firstPage;
			for(uint32_t j = 0; j < i / 14; ++j) {
				buckets[i].count += 1;

				memory_page *page = allocate_os_page(bucketSizes[i]);

				currentPage->next = page;
				page->previous = currentPage;

				currentPage = page;
			}

		buckets[i].freeList = firstPage;	
		buckets[i].pages = firstPage;		
	}
}

/*
 Shutdown the module
*/
void memory_manager_shutdown() {
	for (uint32_t i = numberOfBuckets + 1; i > 0; --i) {		/* free all buckets */

		memory_page *currentPage = buckets[i].pages;			/* free used list */
		while(currentPage != NULL) {
			memory_page *nextPage = currentPage->next;
			free_os_page(currentPage);
			currentPage = nextPage;
		}		
	}
}

#ifndef DEBUG

/*
 Allocates a entry
 size = size of entry
*/
void *memory_manager_allocate(const uint32_t size) {
	if (!size) {						/* check it#s greater then 0 */
		return NULL;
	}
	if (!(size & ~327684)) {			/* if we have a bucket big enough */
		return allocate_entry(size);
	}
	return malloc(size);				/* else collect from the os */
}

/*
 Frees a entry
 entry = entry to free
*/
void memory_manager_free(void *memory) {
	if (!memory) {											/* check it's not null */
		return;
	}
	uint32_t size = sizeof(memory) + sizeof(memory_page);	/* get the real size of the page */
	if (!(size & ~327684)) {								/* if we have a bucket big enough */					
		free_entry(memory - sizeof(memory_page), size);
	} else {
		free(memory);										/* else its an os page */
	}
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
  returns memory_page
*/
static memory_page *allocate_os_page( const uint32_t size ) {
	memory_page *page = malloc( size + sizeof(memory_page) );
	if (!page) {
		exit(0);
	}

	page->memory = (void *)(int32_t)((int8_t)(page) + sizeof(memory_page));
	page->next = NULL;
	page->previous = NULL;
	return page;
}

/*
 free's a page from the OS
  page = memory_page
*/
static void free_os_page( memory_page *page ) {
	if (!page) {
		return;
	}
	free(page);
}

/*
 Allocates a entry.
  size = 1 - 327684
  returns pointer to entry.
*/
static void *allocate_entry( uint32_t size ) {
	int low = 0, high = numberOfBuckets + 1;

	while (low != high) {
		int mid = (low + high) / 2;
		if (bucketSizes[mid] <= size) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	memory_page *page = buckets[low].freeList;
	if (!page) {
		buckets[low].count += 1;
		page = allocate_os_page(bucketSizes[low]);
		buckets[low].pages->previous = page;
		page->next = buckets[low].pages;
		buckets[low].pages = page;
	}
	buckets[low].freeList = page->next;
	return page->memory;
}

/* 
 frees a memory entry 
*/ 
void free_entry( memory_page *entry, const uint32_t size ) {
	int low = 0, high = numberOfBuckets + 1;

	while (low != high) {
		int mid = (low + high) / 2;
		if (bucketSizes[mid] <= size) {
			low = mid + 1;
		} else {
			high = mid;
		}
	}
	memory_page *page = buckets[low].freeList;
	page->previous = entry;
	entry->next = page;
	buckets[low].freeList = entry;
}