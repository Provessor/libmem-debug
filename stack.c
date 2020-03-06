#include <stdlib.h>
#include <stdio.h>

#include "stack.h"
#include "types.h" // FIXME: would like to not include
#include "hand_err.h"

typedef struct stack {
	data_debug *ptr;
	struct stack *next;
} Stack;

void push(void *stack, void *ptr)
{
        Stack *new = (Stack *)malloc(sizeof(Stack));
        if (new == NULL)
                internal_mem_error_size(__LINE__, __FILE__, __func__, sizeof(Stack));
        
        new->ptr = ptr;

        new->next = (Stack *)stack;
        stack = new;
}

void *get(void *stack, void *mptr)
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

        void *ptr = pin->ptr;
        free(pin);
        return ptr;
}

void *pop(void *stack)
{
        Stack *head = (Stack *)stack;
        stack = head->next;

        void *ptr = head->ptr;
        free(head);
        return ptr;
}
