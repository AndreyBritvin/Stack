#ifndef MY_STACK_H_
#define MY_STACK_H_

#include <stdlib.h>

typedef double stack_elem_t;

struct my_stack_t
{
    size_t              size;
    size_t          capacity;
    stack_elem_t       *data;
};

int stack_dump();

#endif // MY_STACK_H_
