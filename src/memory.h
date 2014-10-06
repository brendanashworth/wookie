// memory.h
#ifndef _MEMORY_H
#define _MEMORY_H

/**
 * Allocates space for a pointer.
 * 	This function also sets the memory of the pointer
 *  to continuous \0 bytes.
 * @param  size Size for the pointer.
 * @return      A poitner to the allocated space.
 */
void *w_malloc(size_t size);

/**
 * Reallocates space for a pointer.
 * @param  ptr  The pointer to reallocate.
 * @param  size The space with which to reallocate the pointer.
 * @return      A pointer to the allocated space.
 */
void *w_realloc(void *ptr, size_t size);

/**
 * Contiguously allocates enough space for count objects, each of size size.
 * @param  count Amount of objects.
 * @param  size  Size (in bytes) of each object.
 * @return       Pointer to the allocated space.
 */
void *w_calloc(size_t count, size_t size);

/**
 * Frees the space allocated for a pointer.
 * @param ptr The pointer to free.
 */
void w_free(void *ptr);

#include "memory.c"
#endif