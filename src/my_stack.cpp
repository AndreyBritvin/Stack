#include "my_stack.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

const char * const BEGIN_DUMP = "[----------------BEGIN STACK_DUMP----------------]";
const char * const END_DUMP   = "[-----------------END STACK_DUMP-----------------]";

stack_errors stack_dump(my_stack_t *stack DEBUG_ON(, const char *filename, const char *funcname, int codeline))
{
    //stack_verify(stack);
    DUMP_PRINT("\n%s\n", BEGIN_DUMP);

    if (stack == NULL)
    {
        DUMP_PRINT("Stack_ptr [%p]\n", stack);
        DUMP_PRINT("%s\n\n", END_DUMP);

        return ERROR_STACK_NULL_PTR;
    }

    DUMP_PRINT("Stack_t[%p] born at" DEBUG_ON("WHERE") "called from "
                                     DEBUG_ON("%s:%d function:[%s()]") "\n", stack,
                                     DEBUG_ON(filename, codeline, funcname)); //TODO
    CANARY_PROT(DUMP_PRINT("Canary #1 = %llx\n", stack->canary_left);)
    DUMP_PRINT("Stack capacity: %lu\n", stack->capacity);
    DUMP_PRINT("Stack size    : %lu\n", stack->size);
    DUMP_PRINT("Stack->data [%p]\n", stack->data);

    if(stack->data == NULL)
    {
        CANARY_PROT(DUMP_PRINT("Canary #2 = %llx\n", stack->canary_right);)
        DUMP_PRINT("%s\n\n", END_DUMP);

        return ERROR_STACK_DATA_NULL;
    }

    CANARY_PROT(DUMP_PRINT("data canary #1 = %llx\n{\n", (canary_t)stack->data[-1]);)

    for (size_t i = 0; i < stack->capacity; i++)
    {
        if (i < stack->size)
        {
            DUMP_PRINT("   *[i] = %lu elem = %lf \n", i, stack->data[i]);
        }
        else
        {
            DUMP_PRINT("    [i] = %lu elem = %lf \n", i, stack->data[i]);
        }
    }

    CANARY_PROT(DUMP_PRINT("}\ndata canary #2 = %llx\n", (canary_t)stack->data[stack->capacity]);)

    CANARY_PROT(DUMP_PRINT("Canary #2 = %llx\n", stack->canary_right);)
    DUMP_PRINT("%s\n\n", END_DUMP);

    return SUCCESS;
}

stack_errors stack_ctor(my_stack_t *stack, size_t capacity, size_t el_size)
{
    if (stack == NULL)
    {
        PRINT_ERROR(ERROR_STACK_NULL_PTR);

        return ERROR_STACK_NULL_PTR;
    }

    stack->capacity = capacity;
    stack->size = NULL_POS;
    stack->data = (stack_elem_t *) calloc(1, el_size * capacity CANARY_PROT(+ sizeof(canary_t) * 2));

    stack_verify(stack);

    CANARY_PROT(stack->data[0]            = (stack_elem_t) ((canary_t) CANARY_HEXSPEAK ^ (canary_t) stack->data);)
    CANARY_PROT(stack->data[capacity + 1] = (stack_elem_t) ((canary_t) CANARY_HEXSPEAK ^ (canary_t) stack->data);)
    CANARY_PROT(stack->data++;)

    CANARY_PROT(stack->canary_left  = (canary_t) CANARY_HEXSPEAK ^ (canary_t) stack;)
    CANARY_PROT(stack->canary_right = (canary_t) CANARY_HEXSPEAK ^ (canary_t) stack;)

    stack_verify(stack);

    return SUCCESS;
}

stack_errors stack_dtor(my_stack_t *stack)
{
    stack_verify(stack);

    memset(stack->data, 0, sizeof(stack->data[0]) * stack->capacity); // REDO canary?

    free(stack->data CANARY_PROT(-1)); stack->data = NULL;
    stack->size = NULL_POS;
    stack->capacity = 0;

    return SUCCESS;
}

stack_errors stack_pop(my_stack_t *stack, stack_elem_t *el_to_pop) // TODO:
{
    assert(el_to_pop != NULL);

    DEBUG_PRINT("Entered stack_pop\n");
    stack_verify(stack);

    *el_to_pop = stack->data[stack->size - 1];
    stack->size--;

    if (stack->size <= stack->capacity / (2 * ALLOC_CONST))
    {
        stack_realloc(stack, POPING);
    }

    DEBUG_PRINT("Quit stack_pop\n");

    return SUCCESS;
}

stack_errors stack_push(my_stack_t *stack, stack_elem_t el_to_push)
{
    DEBUG_PRINT("Entered stack_push\n");
    stack_verify(stack);
    stack->data[stack->size] = el_to_push;
    stack->size++;

    if (stack->size >= stack->capacity)
    {
        stack_realloc(stack, PUSHING);
    }

    DEBUG_PRINT("Quit stack_push\n");

    return SUCCESS;
}

static stack_errors stack_realloc(my_stack_t *stack, stack_state state)
{
    stack_verify(stack);

    switch (state)
    {
        case PUSHING:
            DEBUG_PRINT("Increasing memory!!!\n");
            stack->data = (stack_elem_t *) realloc(stack->data CANARY_PROT(- 1), (stack->capacity *= ALLOC_CONST) * sizeof(stack_elem_t)
                                                                    CANARY_PROT(+ 2 * sizeof(canary_t))) CANARY_PROT(+ 1);
            break;
        case POPING:
            DEBUG_PRINT("Decreasing memory :( ):\n");
            stack->data = (stack_elem_t *) realloc(stack->data CANARY_PROT(- 1), (stack->capacity /= (2 * ALLOC_CONST)) * sizeof(stack_elem_t)
                                                                                 CANARY_PROT(+ 2 * sizeof(canary_t))) CANARY_PROT(+ 1);
            break;
        default:
            PRINT_ERROR(ERROR_STACK_STATE_NOT_EXIST);

            return ERROR_STACK_STATE_NOT_EXIST;
    }

    stack_verify(stack);
    STACK_DUMP(stack);

    return SUCCESS;
}

stack_errors stack_verify(my_stack_t *stack)
{
    if (stack == NULL)
    {
        PRINT_ERROR(ERROR_STACK_NULL_PTR);

        return ERROR_STACK_NULL_PTR;
    }

    if (stack->size > stack->capacity)
    {
        PRINT_ERROR(ERROR_SIZE_MORE_CAPACITY);

        return ERROR_SIZE_MORE_CAPACITY;
    }

    if (stack->data == NULL && stack->capacity != 0)
    {
        PRINT_ERROR(ERROR_ALLOC_FAILED);

        return ERROR_ALLOC_FAILED;
    }

    return SUCCESS;
}

#define BEGIN_STACK_CHECKING enum stack_errors STACK_ERR = SUCCESS;
#define CHECK_STACK if(!STACK_ERR) STACK_ERR =
#define END_STACK_CHECKING(macro_my_stack) if(STACK_ERR) {                                    \
                                printf("During stack checking error #%d occured", STACK_ERR); \
                                STACK_DUMP(macro_my_stack);                                   \
                                }

enum stack_errors test_stack()
{
    my_stack_t my_stack = {};
    stack_elem_t to_pop = 0;

    BEGIN_STACK_CHECKING
    CHECK_STACK stack_ctor(&my_stack, 3, sizeof(stack_elem_t));
    // my_stack.data = (stack_elem_t *) realloc(my_stack.data, );
    CHECK_STACK STACK_DUMP(&my_stack);
    for (int i = 1; i < 11; i++)
    {
        CHECK_STACK stack_push(&my_stack, i);
        CHECK_STACK STACK_DUMP(&my_stack);
    }
    for (int i = 1; i < 11; i++)
    {
        CHECK_STACK stack_pop(&my_stack, &to_pop);
        CHECK_STACK STACK_DUMP(&my_stack);
    }
    CHECK_STACK stack_dtor(&my_stack);
    CHECK_STACK STACK_DUMP(&my_stack);
    END_STACK_CHECKING(&my_stack)

    return SUCCESS;
}
#undef BEGIN_STACK_CHECKING
#undef CHECK_STACK
#undef END_STACK_CHECKING
