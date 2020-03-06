#ifndef TYPES_H

#define TYPES_H

#include <stdlib.h>

typedef enum {
	TYPE_MALLOC,
	TYPE_FREE,
	TYPE_CALLOC,
	TYPE_REALLOC,
	TYPE_REALLOCARRAY
} data_type;

typedef struct {
	data_type type;
	int line;
	char *file;
	char *func;
        void *ptr;
} data_debug;

typedef struct {
	data_debug debug;
	size_t size;
        size_t padding;
} data_malloc;

typedef struct {
	data_debug debug;
        data_debug *orig;
} data_free;

typedef struct {
	data_debug debug;
	size_t nmemb;
	size_t size;
        size_t padding;
} data_calloc;

typedef struct {
	data_debug debug;
	size_t size;
        size_t padding;
        void *sptr;
} data_realloc;

typedef struct {
	data_debug debug;
	size_t nmemb;
	size_t size;
        size_t padding;
        void *sptr;
} data_reallocarray;

#endif
