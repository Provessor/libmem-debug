#ifndef LIBMEM_DEBUG_H

#define LIBMEM_DEBUG_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

void *malloc_mem_debug(size_t size, int line, const char *file, const char *func);
void free_mem_debug(void *ptr, int line, const char *file, const char *func);
void *calloc_mem_debug(size_t nmemb, size_t size, int line, const char *file, const char *func);
void *realloc_mem_debug(void *ptr, size_t size, int line, const char *file, const char *func);

#define print_mem_debug() fprint_mem_debug(stderr)
int fprint_mem_debug(FILE *stream);
int log_mem_debug(const char *fn, int8_t append);

#define malloc(size) malloc_mem_debug(size, __LINE__, __FILE__, __func__)
#define free(ptr) free_mem_debug(ptr, __LINE__, __FILE__, __func__)
#define calloc(nmemb, size) calloc_mem_debug(nmemb, size, __LINE__, __FILE__, __func__)
#define realloc(ptr, size) realloc_mem_debug(ptr, size, __LINE__, __FILE__, __func__)

#endif
