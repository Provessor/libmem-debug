#include <stdlib.h>
#include <stdio.h>

#include "stack.h"
#include "types.h" // FIXME: would like to not include

typedef struct stack {
	data_debug *ptr;
	struct stack *next;
} Stack;

void internal_mem_error(const char *file, const int line, const char *err)
{
	fprintf(stderr, "%s: internal memory error at %d - %s", file, line,
		err);
	exit(1);
}

void push(void *stack, void *ptr)
{
        Stack *new = (Stack *)malloc(sizeof(Stack));
        if (new == NULL)
                internal_mem_error(__FILE__, __LINE__, "Failed to allocate memory");
        
        new->ptr = ptr;

        new->next = (Stack *)stack;
        stack = new;
}

void *pop(void *stack, void *mptr)
{
        Stack *prev = NULL, *pin = (Stack *)stack;
        
        while (pin != NULL) {
                if (pin->ptr->ptr == mptr) { // FEXME: all types.h is for
                        if (prev != NULL)
                                prev->next = pin->next;
                        else
                                stack = pin->next;
                } else {
                        prev = pin;
                        pin = pin->next;
                }
        }

        return (void *)pin;
}
