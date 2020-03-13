#include <stdio.h>
#include <stdlib.h>

#define EXIT_STATUS_MEMORY 1
#define EXIT_STATUS_FILE 2

void internal_mem_error(const int line, const char *file, const char *func, const char *err)
{
	fprintf(stderr, "%s: internal memory error at %s:%d - %s\n", file, func, line,
		err);
	exit(EXIT_STATUS_MEMORY);
}

void internal_mem_error_size(const int line, const char *file, const char *func, const unsigned long size)
{
        char err[30];
        sprintf(err, "Failed to allocate %lu bytes\n", size);
        internal_mem_error(line, file, func, err);
}

void internal_file_error(const int line, const char *file, const char *func, const char *fn)
{
        fprintf(stderr, "%s: failed to open file \"%s\" at %s:%d\n", file, fn, func, line);
        exit(EXIT_STATUS_FILE);
}
