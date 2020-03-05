#include "stack.h"
#include "types.h"

#define PADDING_RATIO 1
#define PADDING_INCRE 0
#define PADDING_FILLC (int8_t)0

void *alloc_stack;
void *free_stack;

void *malloc_mem_debug(size_t size, int line, char *file, char *func)
{
	data_malloc *data = (data_malloc *)malloc(sizeof(data_malloc));
	size_t padding = size * PADDING_RATIO + PADDING_INCRE;
	void *ptr = malloc(size + padding);

	for (int8_t *pin = (int8_t *)ptr + size;
	     pin <= (int8_t *)ptr + size + padding; pin++)
		*pin = PADDING_FILLC;

	data->debug.type = TYPE_MALLOC;
	data->debug.line = line;
	data->debug.file = file;
	data->debug.func = func;
	data->debug.ptr = ptr;

	data->size = size;
	data->padding = padding;

	push(alloc_stack, data);

	return ptr;
}

void free_mem_debug(void *ptr, int line, char *file, char *func)
{
        data_debug *orig = (data_debug *)pop(alloc_stack, ptr);

        data_free *data = (data_free *)malloc(sizeof(data_free));

        data->debug = orig;
        data->free_line = line;
        data->free_file = file;
        data->free_func = func;
}

void *calloc_mem_debug(size_t nmemb, size_t size, int line, char *file,
		       char *func)
{
	data_calloc *data = (data_calloc *)malloc(sizeof(data_calloc));
	size_t padding = nmemb * PADDING_RATIO + PADDING_INCRE;
	void *ptr = calloc(nmemb + padding, size);

	for (int8_t *pin = (int8_t *)ptr + (nmemb * size);
	     pin <= (int8_t *)ptr + ((nmemb + padding) * size); pin++)
		*pin = PADDING_FILLC;

	data->debug.type = TYPE_CALLOC;
	data->debug.line = line;
	data->debug.file = file;
	data->debug.func = func;
	data->debug.ptr = ptr;

	data->nmemb = nmemb;
	data->size = size;
	data->padding = padding;

	push(alloc_stack, data);
	return ptr;
}

void *realloc_mem_debug(void *sptr, size_t size, int line, char *file,
			char *func)
{
	data_realloc *data = (data_realloc *)malloc(sizeof(data_realloc));
	size_t padding = size * PADDING_RATIO + PADDING_INCRE;
	void *ptr = realloc(sptr, size + padding);

	for (int8_t *pin = (int8_t *)ptr + size;
	     pin <= (int8_t *)ptr + size + padding; pin++)
		*pin = PADDING_FILLC;

	data->debug.type = TYPE_REALLOC;
	data->debug.line = line;
	data->debug.file = file;
	data->debug.func = func;
        data->debug.ptr = ptr;

	data->size = size;
	data->padding = padding;
	data->sptr = sptr;

	push(alloc_stack, data);
	return ptr;
}

void *reallocarray_mem_debug(void *sptr, size_t nmemb, size_t size, int line,
			     char *file, char *func)
{
	data_reallocarray *data =
		(data_reallocarray *)malloc(sizeof(data_reallocarray));
	size_t padding = nmemb * PADDING_RATIO + PADDING_INCRE;
	void *ptr = reallocarray(sptr, nmemb + padding, size);

	for (int8_t *pin = (int8_t *)ptr + (nmemb * size);
	     pin <= (int8_t *)ptr + ((nmemb + padding) * size); pin++)
		*pin = PADDING_FILLC;

	data->debug.type = TYPE_REALLOCARRAY;
	data->debug.line = line;
	data->debug.file = file;
	data->debug.func = func;
        data->debug.ptr = ptr;

	data->size = size;
	data->padding = padding;
	data->sptr = sptr;

	push(alloc_stack, data);
	return ptr;
}
