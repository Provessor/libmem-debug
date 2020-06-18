#include <stdlib.h>
#include <stdio.h>
#include "../libmem-debug.h"

/* A simple program that allocates and then deallocates some memory */
int main(int argc, char **argv)
{
	char *string = malloc(sizeof(char) * 10);
	char *list = malloc(sizeof(int) * 10);

	print_mem_debug();

	free(list);
	free(string);

	print_mem_debug();

	return 0;
}
