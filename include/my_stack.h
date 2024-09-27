#ifndef MY_STACK_H_
#define MY_STACK_H_

#include <stdlib.h>

enum stack_errors
{
    SUCCESS                     = 0,
    ERROR_SIZE_MORE_CAPACITY    = 1,
    ERROR_ALLOC_FAILED          = 2,
    ERROR_STACK_NULL_PTR        = 3,
};

enum stack_pos
{
    NULL_POS = 0,
};

typedef double stack_elem_t;

struct my_stack_t
{
    size_t              size;
    size_t          capacity;
    stack_elem_t       *data;
};

enum stack_errors stack_dump(my_stack_t *stack);
enum stack_errors stack_ctor(my_stack_t *stack, size_t capacity, size_t el_size);
enum stack_errors stack_dtor(my_stack_t *stack);
enum stack_errors stack_pop(my_stack_t *stack, stack_elem_t *el_to_pop);
enum stack_errors stack_push(my_stack_t *stack, stack_elem_t el_to_push);
enum stack_errors stack_verify(my_stack_t *stack);
enum stack_errors stack_alloc(my_stack_t *stack);
enum stack_errors test_stack();

#endif // MY_STACK_H_
