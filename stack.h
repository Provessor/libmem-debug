#ifndef STACK_H

#define STACK_H

void append(void **stack, void *ptr);
void prepend(void **stack, void *ptr);
void *get(void **stack, void *ptr);
void *pop(void **stack);
int num_elems(void **stack);
void **get_all(void **stack);

#endif
