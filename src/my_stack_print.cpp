#include <assert.h>
#include <stdio.h>

#include "my_stack.h"
#include "my_stack_protection.h"
#include "my_macros.h"

const char * const BEGIN_DUMP = "[----------------BEGIN STACK_DUMP----------------]";
const char * const END_DUMP   = "[-----------------END STACK_DUMP-----------------]";

stack_errors stack_dump(my_stack_t *stack DEBUG_ON(, const char *filename, const char *funcname, int codeline))
{
    //STACK_VERIFY(stack);
    DUMP_PRINT("\n%s\n", BEGIN_DUMP);

    if (stack == NULL)
    {
        DUMP_PRINT("Stack_ptr [%p]\n", stack);
        DUMP_PRINT("%s\n\n", END_DUMP);

        return ERROR_STACK_NULL_PTR;
    }

    DUMP_PRINT("Stack_t[%p] " DEBUG_ON("\"%s\" born at %s:%lu function:[%s()] ") "called from "
                                     DEBUG_ON("%s:%d function:[%s()]") "\n", stack,
                                     DEBUG_ON(stack->stack_name,)
                                     DEBUG_ON(stack->filename, stack->linenum, stack->funcname,)
                                     DEBUG_ON(filename, codeline, funcname));
    CANARY_PROT(DUMP_PRINT("Canary #1 = %llx\n", stack->canary_left);)
    DUMP_PRINT("Stack capacity: %lu\n", stack->capacity);
    DUMP_PRINT("Stack size    : %lu\n\n", stack->size);

    HASH_PROT(DUMP_PRINT("CalcSt hash = %llx\n", calc_struct_hash(stack));)
    HASH_PROT(DUMP_PRINT("Struct hash = %llx\n\n", stack->struct_hash);)

    DUMP_PRINT("Stack->data [%p]\n", stack->data);

    if(stack->data == NULL)
    {
        CANARY_PROT(DUMP_PRINT("Canary #2 = %llx\n", stack->canary_right);)
        DUMP_PRINT("%s\n\n", END_DUMP);

        if (stack->capacity == 0)
        {
            return SUCCESS;
        }

        return ERROR_STACK_DATA_NULL;
    }

    HASH_PROT(DUMP_PRINT("CalcBu hash = %llx\n", calc_buffer_hash(stack));)
    HASH_PROT(DUMP_PRINT("Buffer hash = %llx\n\n", stack->buffer_hash);)

    CANARY_PROT(DUMP_PRINT("data canary #1 = %llx\n{\n", *(canary_t *)((char *)stack->data - 1 * stack->elem_size));)

    // void *delta_ptr = stack->data;
    for (size_t i = 0; i < stack->capacity; i++)
    {
        if (i < stack->size)
        {
            DUMP_PRINT("   *[i] = %lu elem = ", i);
        }
        else
        {
            DUMP_PRINT("    [i] = %lu elem = ", i);
        }
        stack->print_func((char *)stack->data + stack->elem_size * i);
    }

    CANARY_PROT(DUMP_PRINT("}\ndata canary #2 = %llx\n", *(canary_t*)((char *)stack->data + stack->capacity * stack->elem_size));)

    CANARY_PROT(DUMP_PRINT("Canary #2 = %llx\n", stack->canary_right);)
    DUMP_PRINT("%s\n\n", END_DUMP);

    return SUCCESS;
}

void *print_doubles(void *void_begin_ptr)
{
    assert(void_begin_ptr != NULL);

    char *begin_ptr = (char *)void_begin_ptr;
    DUMP_PRINT(" %lf (%lx)\n", *(double *)(begin_ptr), *(size_t *)(begin_ptr));

    return NULL;
}

void *print_longs(void *void_begin_ptr)
{
    assert(void_begin_ptr != NULL);

    char *begin_ptr = (char *)void_begin_ptr;
    DUMP_PRINT(" %lld (%lx)\n", *(long long *)(begin_ptr), *(size_t *)(begin_ptr));

    return NULL;
}
