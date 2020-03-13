#ifndef MEMDEBUG_H

#define MEMDEBUG_H

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

#endif
