#include <stdlib.h>
#include <stdio.h>

#include "stack.h"
#include "types.h" /* FIXME: would like to not include */
#include "hand_err.h"

typedef struct stack {
	data_debug *ptr;
	struct stack *next;
} Stack;

void prepend(void **stack, void *ptr)
{
        Stack *new = (Stack *)malloc(sizeof(Stack));
        if (new == NULL)
                internal_mem_error_size(__LINE__, __FILE__, __func__, sizeof(Stack));

        new->ptr = ptr;
        new->next = NULL;
        *stack = new;

        if (*stack != NULL)
                new->next = ((Stack *)*stack)->next;

        *stack = new;
}

void append(void **stack, void *ptr)
{
        Stack *pin, *new = (Stack *)malloc(sizeof(Stack));
        if (new == NULL)
                internal_mem_error_size(__LINE__, __FILE__, __func__, sizeof(Stack));
        
        new->ptr = ptr;
        new->next = NULL;

        for (pin = (Stack *)*stack; pin->next != NULL; pin = pin->next)
                ;

        pin->next = new;
}

void *get(void **stack, void *mptr)
{
        Stack *prev = NULL, *pin = (Stack *)*stack;
        
        while (pin != NULL) {
                if (pin->ptr->ptr == mptr) { /* FEXME: all types.h is for */
                        if (prev != NULL)
                                prev->next = pin->next;
                        else
                                *stack = pin->next;

                        void *ptr = pin->ptr;
                        free(pin);
                        return ptr;
                }
                
                prev = pin;
                pin = pin->next;
        }

        return NULL;
}

void *pop(void **stack)
{
        void *ptr = ((Stack *)*stack)->ptr;
        *stack = ((Stack *)*stack)->next;
        
        return ptr;
}

int num_elems(void **stack)
{
        Stack *pin;
        int count = 0;
        if (*stack != NULL) {
                count = 1;

                for (pin = (Stack *)*stack; pin->next != NULL; pin = pin->next)
                        count++;
        }

        return count;
}

void **get_all(void **stack)
{
        int count = num_elems(stack);
        void **all;
        Stack *pin = (Stack *)*stack;

        if (count == 0)
                return NULL;

        all = (void **)malloc(sizeof(void **) * count);
        if (all == NULL)
                internal_mem_error_size(__LINE__, __FILE__, __func__, sizeof(void **) * count);

        int i;
        for (i = 0; i < count; i++) {
                all[i] = pin->ptr;
                pin = pin->next;
        }

        return all;
}
