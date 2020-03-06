#include <stdio.h>

#include "stack.h"
#include "types.h"
#include "hand_err.h"

#define PADDING_RATIO 1
#define PADDING_INCRE 0
#define PADDING_FILLC (int8_t)0

int fprint_mem_debug_malloc(FILE *stream, data_malloc *data);
int fprint_mem_debug_free(FILE *stream, data_free *data);
int fprint_mem_debug_calloc(FILE *stream, data_calloc *data);
int fprint_mem_debug_realloc(FILE *stream, data_realloc *data);
#if __GLIBC_MINOR__ > 28
int fprint_mem_debug_reallocarray(FILE *stream, data_reallocarray *data);
#endif


void *alloc_stack;
void *free_stack;

void *malloc_mem_debug(size_t size, int line, char *file, char *func)
{
	data_malloc *data = (data_malloc *)malloc(sizeof(data_malloc));
        if (data == NULL)
                internal_mem_error_size(__LINE__, __FILE__, __func__, sizeof(data_malloc));

	size_t padding = size * PADDING_RATIO + PADDING_INCRE;
	void *ptr = malloc(size + padding);

	for (int8_t *pin = (int8_t *)ptr + size;
	     pin < (int8_t *)ptr + size + padding; pin++)
		*pin = PADDING_FILLC;

	data->debug.type = TYPE_MALLOC;
	data->debug.line = line;
	data->debug.file = file;
	data->debug.func = func;
	data->debug.ptr = ptr;

	data->size = size;
	data->padding = padding;

	push(&alloc_stack, data);

	return ptr;
}

void free_mem_debug(void *ptr, int line, char *file, char *func)
{
	data_debug *orig = (data_debug *)get(&alloc_stack, ptr);
        if (orig == NULL)
                internal_mem_error(__LINE__, __FILE__, __func__, "Tried to remove a element from stack which was not on stack");

	data_free *data = (data_free *)malloc(sizeof(data_free));
        if (data == NULL)
                internal_mem_error_size(__LINE__, __FILE__, __func__, sizeof(data_free));

	data->orig = orig;
        data->debug.type = TYPE_FREE;
        data->debug.line = line;
        data->debug.file = file;
        data->debug.func = func;
        data->debug.ptr = ptr;

        push(&free_stack, data);
}

void *calloc_mem_debug(size_t nmemb, size_t size, int line, char *file,
		       char *func)
{
	data_calloc *data = (data_calloc *)malloc(sizeof(data_calloc));
        if (data == NULL)
                internal_mem_error_size(__LINE__, __FILE__, __func__, sizeof(data_calloc));

	size_t padding = nmemb * PADDING_RATIO + PADDING_INCRE;
	void *ptr = calloc(nmemb + padding, size);

	for (int8_t *pin = (int8_t *)ptr + (nmemb * size);
	     pin < (int8_t *)ptr + ((nmemb + padding) * size); pin++)
		*pin = PADDING_FILLC;

	data->debug.type = TYPE_CALLOC;
	data->debug.line = line;
	data->debug.file = file;
	data->debug.func = func;
	data->debug.ptr = ptr;

	data->nmemb = nmemb;
	data->size = size;
	data->padding = padding;

	push(&alloc_stack, data);
	return ptr;
}

void *realloc_mem_debug(void *sptr, size_t size, int line, char *file,
			char *func)
{
	data_realloc *data = (data_realloc *)malloc(sizeof(data_realloc));
        if (data == NULL)
                internal_mem_error_size(__LINE__, __FILE__, __func__, sizeof(data_realloc));

	size_t padding = size * PADDING_RATIO + PADDING_INCRE;
	void *ptr = realloc(sptr, size + padding);

	for (int8_t *pin = (int8_t *)ptr + size;
	     pin < (int8_t *)ptr + size + padding; pin++)
		*pin = PADDING_FILLC;

	data->debug.type = TYPE_REALLOC;
	data->debug.line = line;
	data->debug.file = file;
	data->debug.func = func;
	data->debug.ptr = ptr;

	data->size = size;
	data->padding = padding;

        if (get(&alloc_stack, sptr) == NULL)
                internal_mem_error(__LINE__, __FILE__, __func__, "Tried to remove a element from stack which was not on stack");
	push(&alloc_stack, data);
	return ptr;
}

#if __GLIBC_MINOR__ > 28
void *reallocarray_mem_debug(void *sptr, size_t nmemb, size_t size, int line,
			     char *file, char *func)
{
	data_reallocarray *data =
		(data_reallocarray *)malloc(sizeof(data_reallocarray));
        if (data == NULL)
                internal_mem_error_size(__LINE__, __FILE__, __func__, sizeof(data_reallocarray));

	size_t padding = nmemb * PADDING_RATIO + PADDING_INCRE;
	void *ptr = reallocarray(sptr, nmemb + padding, size);

	for (int8_t *pin = (int8_t *)ptr + (nmemb * size);
	     pin < (int8_t *)ptr + ((nmemb + padding) * size); pin++)
		*pin = PADDING_FILLC;

	data->debug.type = TYPE_REALLOCARRAY;
	data->debug.line = line;
	data->debug.file = file;
	data->debug.func = func;
	data->debug.ptr = ptr;

	data->size = size;
	data->padding = padding;

        if (get(&alloc_stack, sptr) == NULL)
                        internal_mem_error(__LINE__, __FILE__, __func__, "Tried to remove a element from stack which was not on stack");
	push(&alloc_stack, data);
	return ptr;
}
#endif

int fprint_mem_debug_malloc(FILE *stream, data_malloc *data)
{
	return fprintf(stream,
                       "malloc():"
                       "\t\nSIZE: %zu"
                       "\t\nPADDING: %zu"
                       "\t\nLINE: %d"
                       "\t\nFILE: %s"
                       "\t\nFUNCTION: %s"
                       "\t\nPTR: %p"
                       "\n\n",
                       data->size, data->padding, data->debug.line, data->debug.file,
                       data->debug.func, data->debug.ptr);
}

int fprint_mem_debug_free(FILE *stream, data_free *data)
{
        int free = fprintf(stream,
                           "free():"
                           "\t\nLINE: %d"
                           "\t\nFILE: %s"
                           "\t\nFUNCTION: %s"
                           "\t\nPTR: %p"
                           "\t\tORIGINAL:\n",
                           data->debug.line, data->debug.file, data->debug.func,
                           data->debug.ptr);
        switch (data->orig->type) {
        case TYPE_MALLOC:
                free = free && fprint_mem_debug_malloc(stream, (data_malloc *)data->orig);
                break;
        case TYPE_FREE:
                free = 0;
                break;
        case TYPE_CALLOC:
                free = free && fprint_mem_debug_calloc(stream, (data_calloc *)data->orig);
                break;
        case TYPE_REALLOC:
                free = free && fprint_mem_debug_realloc(stream, (data_realloc *)data->orig);
                break;
#if __GLIBC_MINOR__ > 28
        case TYPE_REALLOCARRAY:
                free = free && fprint_mem_debug_reallocarray(stream, (data_reallocarray *)data->orig);
                break;
#endif
        }

        return free && fprintf(stderr, "\n");
}

int fprint_mem_debug_calloc(FILE *stream, data_calloc *data)
{
	return fprintf(stream,
		       "calloc():"
		       "\t\nNMEMB: %zu"
		       "\t\nPADDING: %zu"
		       "\t\nSIZE: %zu"
		       "\t\nLINE: %d"
		       "\t\nFILE: %s"
		       "\t\nFUNCTION: %s"
		       "\t\nPTR: %p"
		       "\n\n",
		       data->nmemb, data->padding, data->size, data->debug.line,
		       data->debug.file, data->debug.func, data->debug.ptr);
}

int fprint_mem_debug_realloc(FILE *stream, data_realloc *data)
{
        return fprintf(stream,
                       "realloc():"
                       "\t\nSIZE: %zu"
                       "\t\nPADDING: %zu"
                       "\t\nLINE: %d"
                       "\t\nFILE: %s"
                       "\t\nFUNCTION: %s"
                       "\t\nPTR: %p"
                       "\n\n",
                       data->size, data->padding, data->debug.line, data->debug.file,
                       data->debug.func, data->debug.ptr);
}

#if __GLIBC_MINOR__ > 28
int fprint_mem_debug_reallocarray(FILE *stream, data_reallocarray *data)
{
        return fprintf(stream,
                       "reallocarray():"
                       "\t\nNMEMB: %zu"
                       "\t\nPADDING: %zu"
                       "\t\nSIZE: %zu"
                       "\t\nLINE: %d"
                       "\t\nFILE: %s"
                       "\t\nFUNCTION: %s"
                       "\t\nPTR: %p"
                       "\n\n",
                       data->nmemb, data->padding, data->size, data->debug.line, data->debug.file,
                       data->debug.func, data->debug.ptr);
}
#endif

int fprint_mem_debug(FILE *stream)
{
        data_debug *data;

        while ((data = pop(&alloc_stack)) != NULL) {
                switch (data->type) {
                case TYPE_MALLOC:
                        return fprint_mem_debug_malloc(stream, (data_malloc *)data);
                        break;
                case TYPE_FREE:
                        return 0;
                        break;
                case TYPE_CALLOC:
                        return fprint_mem_debug_calloc(stream, (data_calloc *)data);
                        break;
                case TYPE_REALLOC:
                        return fprint_mem_debug_realloc(stream, (data_realloc *)data);
                        break;
#if __GLIBC_MINOR__ > 28
                case TYPE_REALLOCARRAY:
                        return fprint_mem_debug_reallocarray(stream, (data_reallocarray *)data);
                        break;
#endif
                }
        }

        while ((data = pop(&free_stack)) != NULL) {
                switch (data->type) {
                case TYPE_CALLOC:
                case TYPE_REALLOC:
#if __GLIBC_MINOR__ > 28
                case TYPE_REALLOCARRAY:
#endif
                case TYPE_MALLOC:
                        return 0;
                        break;
                case TYPE_FREE:
                        return fprint_mem_debug_free(stream, (data_free *)data);
                        break;
                }
        }

        return 0;
}

// Could also be done with a macro
int print_mem_debug(void)
{
        return fprint_mem_debug(stderr);
}

int log_mem_debug(char *fn, int8_t append)
{
        FILE *fp;

        if ((fp = fopen(fn, append ? "a" : "w")) == NULL)
                internal_file_error(__LINE__, __FILE__, __func__, fn);

        return fprint_mem_debug(fp);
}
