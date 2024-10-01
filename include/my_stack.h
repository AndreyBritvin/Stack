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
    ERROR_STRUCT_CANARY_DIED    = 6,
    ERROR_BUFFER_CANARY_DIED    = 7,
    ERROR_HASH_IN_STRUCT        = 8,
    ERROR_HASH_IN_BUFFER        = 9,
    ERROR_STACK_UNDERFLOW       = 10,
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
    HASH_PROT(hash_t struct_hash;) // no hash after this var
    DEBUG_ON(const char *filename; const char *funcname; size_t linenum; const char *stack_name;)
    CANARY_PROT(canary_t canary_right;)
};

static const int ALLOC_CONST = 2;
static const int STACK_POISON_VALUE = 'DEDNELOH' / 1000;

stack_errors stack_dump(my_stack_t *stack DEBUG_ON(, const char *filename, const char *funcname, int codeline));
stack_errors stack_ctor(my_stack_t *stack, size_t capacity, size_t el_size);
stack_errors stack_dtor(my_stack_t *stack);

stack_errors stack_pop(my_stack_t *stack, stack_elem_t *el_to_pop);
stack_errors stack_push(my_stack_t *stack, stack_elem_t el_to_push);

stack_errors stack_verify(my_stack_t *stack);
static stack_errors stack_realloc(my_stack_t *stack, stack_state state);
stack_errors test_stack();

#ifdef HASH_PROTECTION
static stack_errors recalc_hash(my_stack_t *stack);
static hash_t calc_buffer_hash(my_stack_t *stack);
static hash_t calc_struct_hash(my_stack_t *stack);

#endif // HASH_PROTECTION

#endif // MY_STACK_H_
