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

typedef long long stack_elem_t;
typedef void *(*dump_print_t)(void *void_begin_ptr, size_t iteration, size_t size, size_t el_size);

struct my_stack_t
{
    CANARY_PROT(canary_t canary_left;)
    size_t              size;
    size_t          capacity;
    size_t         elem_size;
    stack_elem_t       *data;
    HASH_PROT(hash_t buffer_hash;)
    HASH_PROT(hash_t struct_hash;) // no hash after this var
    DEBUG_ON(const char *filename; const char *funcname; size_t linenum; const char *stack_name;)
    CANARY_PROT(canary_t canary_right;)
};

static const int ALLOC_CONST = 2;
static const int STACK_POISON_VALUE = 'DEDNELOH';

stack_errors stack_dump(my_stack_t *stack, dump_print_t dump_func DEBUG_ON(, const char *filename, const char *funcname, int codeline));
stack_errors stack_ctor(my_stack_t *stack, size_t capacity, size_t el_size);
stack_errors stack_dtor(my_stack_t *stack);

stack_errors stack_pop(my_stack_t *stack, stack_elem_t *el_to_pop);
stack_errors stack_push(my_stack_t *stack, stack_elem_t el_to_push);

stack_errors stack_verify(my_stack_t *stack);
stack_errors test_stack();

void *print_doubles(void *void_begin_ptr, size_t iteration, size_t size, size_t el_size);
void *print_longs  (void *void_begin_ptr, size_t iteration, size_t size, size_t el_size);

#endif // MY_STACK_H_
