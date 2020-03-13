#include "memdebug.h"
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


void *alloc_stack;
void *free_stack;

void *malloc_mem_debug(size_t size, int line, const char *file, const char *func)
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

	prepend(&alloc_stack, data);

	return ptr;
}

void free_mem_debug(void *ptr, int line, const char *file, const char *func)
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

        prepend(&free_stack, data);
}

void *calloc_mem_debug(size_t nmemb, size_t size, int line, const char *file,
		       const char *func)
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

	prepend(&alloc_stack, data);
	return ptr;
}

void *realloc_mem_debug(void *sptr, size_t size, int line, const char *file,
			const char *func)
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
	prepend(&alloc_stack, data);
	return ptr;
}

int fprint_mem_debug_malloc(FILE *stream, data_malloc *data)
{
	return fprintf(stream,
                       "malloc():"
                       "\n\tSIZE: %zu"
                       "\n\tPADDING: %zu"
                       "\n\tLINE: %d"
                       "\n\tFILE: %s"
                       "\n\tFUNCTION: %s"
                       "\n\tPTR: %p"
                       "\n\n",
                       data->size, data->padding, data->debug.line, data->debug.file,
                       data->debug.func, data->debug.ptr);
}

int fprint_mem_debug_free(FILE *stream, data_free *data)
{
        int free = fprintf(stream,
                           "free():"
                           "\n\tLINE: %d"
                           "\n\tFILE: %s"
                           "\n\tFUNCTION: %s"
                           "\n\tPTR: %p"
                           "\n    ORIGINAL:\n",
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
        }

        return free && fprintf(stream, "\n");
}

int fprint_mem_debug_calloc(FILE *stream, data_calloc *data)
{
	return fprintf(stream,
		       "calloc():"
		       "\n\tNMEMB: %zu"
		       "\n\tPADDING: %zu"
		       "\n\tSIZE: %zu"
		       "\n\tLINE: %d"
		       "\n\tFILE: %s"
		       "\n\tFUNCTION: %s"
		       "\n\tPTR: %p"
		       "\n\n",
		       data->nmemb, data->padding, data->size, data->debug.line,
		       data->debug.file, data->debug.func, data->debug.ptr);
}

int fprint_mem_debug_realloc(FILE *stream, data_realloc *data)
{
        return fprintf(stream,
                       "realloc():"
                       "\n\tSIZE: %zu"
                       "\n\tPADDING: %zu"
                       "\n\tLINE: %d"
                       "\n\tFILE: %s"
                       "\n\tFUNCTION: %s"
                       "\n\tPTR: %p"
                       "\n\n",
                       data->size, data->padding, data->debug.line, data->debug.file,
                       data->debug.func, data->debug.ptr);
}

int fprint_mem_debug(FILE *stream)
{
        data_debug **data;

        data = (data_debug **)get_all(&alloc_stack);

        if (data != NULL) {
                for (int i = num_elems(&alloc_stack) - 1; i >= 0; i--) {
                        switch (data[i]->type) {
                        case TYPE_MALLOC:
                                return fprint_mem_debug_malloc(stream, (data_malloc *)data[i]);
                                break;
                        case TYPE_FREE:
                                return 0;
                                break;
                        case TYPE_CALLOC:
                                return fprint_mem_debug_calloc(stream, (data_calloc *)data[i]);
                                break;
                        case TYPE_REALLOC:
                                return fprint_mem_debug_realloc(stream, (data_realloc *)data[i]);
                                break;
                        }
                }
        }

        free(data);
        
        data = (data_debug **)get_all(&free_stack);
        if (data != NULL) {
                for (int i = num_elems(&free_stack) - 1; i >= 0; i--) {
                        switch (data[i]->type) {
                        case TYPE_CALLOC:
                        case TYPE_REALLOC:
                        case TYPE_MALLOC:
                                return 0;
                                break;
                        case TYPE_FREE:
                                return fprint_mem_debug_free(stream, (data_free *)data[i]);
                                break;
                        }
                }
        }

        free(data);

        fflush(stream);
        return 0;
}

int log_mem_debug(const char *fn, int8_t append)
{
        FILE *fp;

        if ((fp = fopen(fn, append ? "a" : "w")) == NULL)
                internal_file_error(__LINE__, __FILE__, __func__, fn);

        int ret = fprint_mem_debug(fp);

        fclose(fp);
        return ret;
}
