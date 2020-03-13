#ifndef LIBMEM_DEBUG_H

#define LIBMEM_DEBUG_H

#include "memdebug.h"

#define malloc(size) malloc_mem_debug(size, __LINE__, __FILE__, __func__)
#define free(ptr) free_mem_debug(ptr, __LINE__, __FILE__, __func__)
#define calloc(nmemb, size) calloc_mem_debug(nmemb, size, __LINE__, __FILE__, __func__)
#define realloc(ptr, size) realloc_mem_debug(ptr, size, __LINE__, __FILE__, __func__)

#endif
