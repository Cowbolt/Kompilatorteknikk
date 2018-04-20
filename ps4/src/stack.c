#include <stack.h>

stack* create_stack ()
{

    stack *s = malloc(sizeof(stack*));
    s->size = 0;
    s->head = NULL;

    return s;

}

void stack_push ( stack* s, tlhash_t* val )
{

    stack_elem* temp = malloc(sizeof(stack_elem*));

    temp->next = s->head;
    temp->data = val;
    s->head = temp;
    s->size = (s->size) + 1;


}

void stack_pop ( stack* s )
{

    stack_elem* temp;

    temp = s->head;
    s->head = temp->next;
    free(temp);
    s->size = (s->size) - 1; //subtracts from counter

}

tlhash_t* stack_top ( stack* s )
{

    stack_elem* temp = s->head;

    tlhash_t* value = temp->data;

    return value;
}

tlhash_t* stack_get_elem ( stack* s, int index )
{
    stack_elem* temp = s->head;
    for (int i = 0; i < index; i++)
    {
        temp = temp->next;
    }
    return temp->data;

}