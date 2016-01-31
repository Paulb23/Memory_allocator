/*-------------------------------------------------------------------------*/
/**
   @file    memory_manager.h
   @author  P. Batty
   @brief   Manages the memory
   This module implements the memory manager.
*/
/*--------------------------------------------------------------------------*/

#ifndef _MEMORY_MANAGER_H_
#define _MEMORY_MANAGER_H_ 

#include "begin_code.h"
#include <stdint.h>

#ifdef __cplusplus
#define extern "C" {
#endif

/*------------------------*/
//	Structures			  //
/*------------------------*/   

/* Represents a page of memory. */
typedef struct memory_page {
	struct memory_page *next;		/* the next page */
	struct memory_page *previous;	/* the previous page */
	void *memory;					/* the block of memory */
} memory_page; // 12 bytes


/* Represents a bucket */
typedef struct memory_bucket {
	uint32_t count;					/* number of memory pages within this bucket */
	uint32_t memoryPageSize;		/* size of the Memory pages within this bucket */
	memory_page *freeList;			/* freeList of memory pages */
	memory_page *pages;				/* used list of memory pages */
} memory_bucket;	//


/*------------------------*/
//	Methods				  //
/*------------------------*/   

extern DLL_EXPORT void CALL memory_manager_init();		/* initilises the module */
extern DLL_EXPORT void CALL memory_manager_shutdown();  /* Shutdown the module */


#ifndef DEBUG

extern DLL_EXPORT void CALL *memory_manager_allocate( const uint32_t size );	/* allocate memory of given size */
extern DLL_EXPORT void CALL  memory_manager_free( void *memory );				/* free the memory */

#else

extern DLL_EXPORT void CALL *memory_manager_allocate( const uint32_t size, const char *filename, const int lineNumber ); /* allocate memory of given size */
extern DLL_EXPORT void CALL  memory_manager_free( void *memory, const char *filename, const int lineNumber );			 /* free the memory */
extern DLL_EXPORT void CALL  memory_manager_dump( void *memory, const char *filename, const int lineNumber );			 /* dump the current page memory */


#define memory_manager_allocate( size ) memory_manager_allocate( size, __FILE__, __LINE__ )	/* some defines to auto fill debug calls*/
#define memory_manager_dump( memory )   memory_manager_dump( memory, __FILE__, __LINE__ )		
#define memory_manager_free( memory )   memory_manager_free( memory, __FILE__, __LINE__ )

#endif

#ifdef __cplusplus
}
#endif
#endif
