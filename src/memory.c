// memory.c
#include <errno.h>
#include <stdio.h>
#include <string.h>

void *w_malloc(size_t size) {
	void *ptr = NULL;

	// Allocate the memory
	ptr = malloc(size);

	// Ensure the allocation was successful
	if (ptr == NULL) {
		printf("w_malloc(%zu): errno %d, report to wookie developers\n", size, errno);
		exit(-1);
	}

	// Clear the memory
	memset(ptr, '\0', size);
	
	return ptr;
}

void *w_realloc(void *ptr, size_t size) {
	// If the pointer is null, just w_malloc,
	// because that is what realloc() does
	if (ptr == NULL) {
		return w_malloc(size);
	}

	// Reallocate
	return realloc(ptr, size);
}

void *w_calloc(size_t count, size_t size) {
	// Contiguously allocate
	return calloc(count, size);
}

void w_free(void *ptr) {
	// Ensure the pointer was allocated
	if (ptr == NULL)
		return;

	// Free the memory
	free(ptr);
}