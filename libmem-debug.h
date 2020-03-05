#ifndef LIBMEM_DEBUG_H

#define LIBMEMDEBUG_H

#include <stdlib.h>

#define malloc(size) malloc_mem_debug(size, __LINE__, __FILE__, __func__)
#define free(ptr) free_mem_debug(ptr, __LINE__, __FILE__, __func__)
#define calloc(nmemb, size) calloc_mem_debug(nmemb, size, __LINE__, __FILE__, __func__)
#define realloc(ptr, size) realloc_mem_debug(ptr, size, __LINE__, __FILE__, __func__)
#define reallocarray(ptr, nmemb, size) reallocarray_mem_debug(ptr, nmemb, size, __LINE__, __FILE__, __func__)

void *malloc_mem_debug(size_t size, int line, char *file, char *func);
void free_mem_debug(void *ptr, int line, char *file, char *func);
void *calloc_mem_debug(size_t nmemb, size_t size, int line, char *file, char *func);
void *realloc_mem_debug(void *ptr, size_t size, int line, char *file, char *func);
void *reallocarray_mem_debug(void *ptr, size_t nmemb, size_t size, int line, char *file, char *func);

#endif
