#ifndef MY_STACK_H_
#define MY_STACK_H_

#define CANARY_PROTECTION
#define HASH_PROTECTION

#include <stdlib.h>
#include "my_stack_protection.h"
#include "my_macros.h"

enum stack_state
{
    PUSHING,
    POPING ,
};

enum stack_errors
{
    SUCCESS                     = 0,
    ERROR_SIZE_MORE_CAPACITY    = 1,
    ERROR_ALLOC_FAILED          = 2,
    ERROR_STACK_NULL_PTR        = 3,
    ERROR_STACK_DATA_NULL       = 4,
    ERROR_STACK_STATE_NOT_EXIST = 5,
};

enum stack_pos
{
    NULL_POS = 0,
};

typedef double stack_elem_t;

struct my_stack_t
{
    CANARY_PROT(canary_t canary_left;)
    size_t              size;
    size_t          capacity;
    stack_elem_t       *data;
    HASH_PROT(hash_t buffer_hash;)
    HASH_PROT(hash_t struct_hash;)
    CANARY_PROT(canary_t canary_right;)
};

static const int ALLOC_CONST = 2;

stack_errors stack_dump(my_stack_t *stack DEBUG_ON(, const char *filename, const char *funcname, int codeline));
stack_errors stack_ctor(my_stack_t *stack, size_t capacity, size_t el_size);
stack_errors stack_dtor(my_stack_t *stack);

stack_errors stack_pop(my_stack_t *stack, stack_elem_t *el_to_pop);
stack_errors stack_push(my_stack_t *stack, stack_elem_t el_to_push);

stack_errors stack_verify(my_stack_t *stack);
static stack_errors stack_realloc(my_stack_t *stack, stack_state state);
stack_errors test_stack();

#endif // MY_STACK_H_
