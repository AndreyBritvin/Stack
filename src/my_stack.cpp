#include "my_stack.h"
#include "my_macros.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>


stack_errors stack_dump(my_stack_t *stack)
{
    int stack_error = stack_verify(stack);

    printf("\n[----------------BEGIN STACK_DUMP----------------]\n");

    if (!stack)
    {
        printf("Stack_ptr = NULL\n");
        printf("[-----------------END STACK_DUMP-----------------]\n\n");

        return ERROR_STACK_NULL_PTR;
    }

    printf("Stack capacity: %lu\n", stack->capacity);
    printf("Stack size    : %lu\n", stack->size);

    if(!stack->data)
    {
        printf("Stack->data = NULL\n");
        printf("[-----------------END STACK_DUMP-----------------]\n\n");

        return ERROR_STACK_DATA_NULL;
    }

    printf("Stack Data:\n");
    for (size_t i = 0; i < stack->size; i++)
    {
        printf("i = %lu elem = %lf \n", i, stack->data[i]);
    }

    printf("[-----------------END STACK_DUMP-----------------]\n\n");

    return SUCCESS;
}

stack_errors stack_ctor(my_stack_t *stack, size_t capacity, size_t el_size)
{
    stack_verify(stack);

    stack->capacity = capacity;
    stack->size = NULL_POS;
    stack->data = (stack_elem_t *) calloc(capacity, el_size);

    stack_verify(stack);

    return SUCCESS;
}

stack_errors stack_dtor(my_stack_t *stack)
{
    stack_verify(stack);

    memset(stack->data, 0, sizeof(stack->data[0]) * stack->capacity);

    free(stack->data); stack->data = NULL;
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

<<<<<<< Updated upstream
=======
    if(stack->size == STACK_POISON_VALUE)
    {
        printf("Here you are\n");
        stack->data = (stack_elem_t *) STACK_POISON_VALUE;
    }

    HASH_PROT(recalc_hash(stack));

>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
=======
    HASH_PROT(recalc_hash(stack));

>>>>>>> Stashed changes
    if (stack->size >= stack->capacity)
    {
        stack_realloc(stack, PUSHING);
    }

    DEBUG_PRINT("Quit stack_push\n");

    return SUCCESS;
}

stack_errors stack_realloc(my_stack_t *stack, stack_state state)
{
    stack_verify(stack);

    switch (state)
    {
        case PUSHING:
            DEBUG_PRINT("Increasing memory!!!\n");
            stack->data = (stack_elem_t *) realloc(stack->data, (stack->capacity *= ALLOC_CONST) * sizeof(stack_elem_t));
            break;
        case POPING:
            DEBUG_PRINT("Decreasing memory :( ):\n");
            stack->data = (stack_elem_t *) realloc(stack->data, (stack->capacity /= (2 * ALLOC_CONST)) * sizeof(stack_elem_t));
            break;
        default:
            PRINT_ERROR(ERROR_STACK_STATE_NOT_EXIST);

            return ERROR_STACK_STATE_NOT_EXIST;
    }

<<<<<<< Updated upstream
    stack_verify(stack);
    stack_dump(stack);
=======
    if (stack->data == NULL)
    {
        PRINT_ERROR(ERROR_STACK_DATA_NULL);

        return ERROR_STACK_DATA_NULL;
    }

    CANARY_PROT(stack->data[-1]              = DATA_CANARY);
    CANARY_PROT(stack->data[stack->capacity] = DATA_CANARY);

    HASH_PROT(recalc_hash(stack));
    STACK_VERIFY(stack);
    STACK_DUMP(stack);
>>>>>>> Stashed changes

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
                                stack_dump(macro_my_stack);                                   \
                                }

enum stack_errors test_stack()
{
    my_stack_t my_stack = {};
    stack_elem_t to_pop = 0;

    BEGIN_STACK_CHECKING
    CHECK_STACK stack_ctor(&my_stack, 3, sizeof(stack_elem_t));
    // my_stack.data = (stack_elem_t *) realloc(my_stack.data, );
<<<<<<< Updated upstream
    CHECK_STACK stack_dump(&my_stack);
    for (int i = 1; i < 11; i++)
=======
    CHECK_STACK STACK_DUMP(&my_stack);
    for (int i = 1; i < STACK_POISON_VALUE + 10; i++)
>>>>>>> Stashed changes
    {
        CHECK_STACK stack_push(&my_stack, i);
        CHECK_STACK stack_dump(&my_stack);
    }
<<<<<<< Updated upstream
    for (int i = 1; i < 11; i++)
=======

    // my_stack.struct_hash += 1;
    for (int i = 1; i < STACK_POISON_VALUE + 10; i++)
>>>>>>> Stashed changes
    {
        CHECK_STACK stack_pop(&my_stack, &to_pop);
        CHECK_STACK stack_dump(&my_stack);
    }
    CHECK_STACK stack_dtor(&my_stack);
    CHECK_STACK stack_dump(&my_stack);
    END_STACK_CHECKING(&my_stack)

    return SUCCESS;
}
#undef BEGIN_STACK_CHECKING
#undef CHECK_STACK
#undef END_STACK_CHECKING
