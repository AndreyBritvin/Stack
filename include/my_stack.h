#ifndef MY_STACK_H_
#define MY_STACK_H_

#define CANARY_PROTECTION
#define HASH_PROTECTION

#include <stdlib.h>
#include "my_stack_protection.h"
#include "my_macros.h"
#include "my_errors.h"

enum stack_state
{
    PUSHING,
    POPING ,
};

enum stack_pos
{
    NULL_POS = 0,
};

typedef long long stack_elem_t;
typedef void *(*dump_print_t)(void *void_begin_ptr);

struct my_stack_t
{
    CANARY_PROT(canary_t canary_left;)
    size_t              size;
    size_t          capacity;
    size_t         elem_size;
    void *              data;
    dump_print_t  print_func;
    HASH_PROT(hash_t buffer_hash;)
    HASH_PROT(hash_t struct_hash;) // no hash after this var
    DEBUG_ON(const char *filename; const char *funcname; size_t linenum; const char *stack_name;)
    CANARY_PROT(canary_t canary_right;)
};

static const int ALLOC_CONST = 2;
static const int STACK_POISON_VALUE = 'DEDNELOH';

stack_errors stack_dump(my_stack_t *stack DEBUG_ON(, const char *filename, const char *funcname, int codeline));
stack_errors stack_ctor(my_stack_t *stack, size_t capacity, size_t el_size, dump_print_t dump_func);
stack_errors stack_dtor(my_stack_t *stack);

stack_errors stack_pop (my_stack_t *stack, void *el_to_pop);
stack_errors stack_push(my_stack_t *stack, void *el_to_push);

stack_errors stack_verify(my_stack_t *stack);
stack_errors unit_test_stack();

void *print_doubles(void *void_begin_ptr);
void *print_longs  (void *void_begin_ptr);

#ifdef HASH_PROTECTION
    stack_errors recalc_hash(my_stack_t *stack);
    hash_t calc_buffer_hash(my_stack_t *stack);
    hash_t calc_struct_hash(my_stack_t *stack);
#endif // HASH_PROTECTION

#endif // MY_STACK_H_
