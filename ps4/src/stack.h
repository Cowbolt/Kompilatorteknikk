#include <stdio.h>
#include <stdlib.h>
#include <tlhash.h>

typedef struct stack_elem_ {
    tlhash_t* data;
    struct stack_elem_ *next;
}stack_elem;

typedef struct stack_{
    int size;
    stack_elem *head;
}stack;

stack* create_stack();
void stack_push(stack* s, tlhash_t* val);
tlhash_t* stack_top(stack* s);
void stack_pop(stack* s);
tlhash_t* stack_get_elem (stack* s, int index);