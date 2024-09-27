#include "my_stack.h"
#include "my_macros.h"

#include <stdio.h>
#include <assert.h>

enum stack_errors stack_dump(my_stack_t *stack)
{
    int stack_error = stack_verify(stack);

    printf("\n[----------------BEGIN STACK_DUMP----------------]\n");

    if (!stack || !stack->data)
    {
        return ERROR_STACK_NULL_PTR;
    }

    printf("Stack capacity: %lu\n", stack->capacity);
    printf("Stack size    : %lu\n", stack->size);
    printf("Stack Data:\n");
    for (size_t i = 0; i < stack->size; i++)
    {
        printf("i = %lu elem = %lf \n", i, stack->data[i]);
    }

    printf("[-----------------END STACK_DUMP-----------------]\n\n");

    return SUCCESS;
}

enum stack_errors stack_ctor(my_stack_t *stack, size_t capacity, size_t el_size)
{
    stack_verify(stack);

    stack->capacity = capacity;
    stack->size = NULL_POS;
    stack->data = (stack_elem_t *) calloc(capacity, el_size);

    stack_verify(stack);

    return SUCCESS;
}

enum stack_errors stack_dtor(my_stack_t *stack)
{
    return SUCCESS;
}

enum stack_errors stack_pop(my_stack_t *stack, stack_elem_t *el_to_pop)
{
    assert(el_to_pop != NULL);
    DEBUG_PRINT("Entered stack_pop\n");
    stack_verify(stack);

    *el_to_pop = stack->data[stack->size - 1];
    stack->size--;

    DEBUG_PRINT("Quit stack_pop\n");

    return SUCCESS;
}

enum stack_errors stack_push(my_stack_t *stack, stack_elem_t el_to_push)
{
    DEBUG_PRINT("Entered stack_push\n");
    stack_verify(stack);
    stack->data[stack->size] = el_to_push;
    stack->size++;
    DEBUG_PRINT("Quit stack_push\n");

    return SUCCESS;
}

enum stack_errors stack_alloc(my_stack_t *stack)
{
    stack_verify(stack);

    return SUCCESS;
}

enum stack_errors stack_verify(my_stack_t *stack)
{
    if (stack == NULL)
    {
        return ERROR_STACK_NULL_PTR;
    }

    if (stack->size > stack->capacity)
    {
        return ERROR_SIZE_MORE_CAPACITY;
    }

    if (stack->data == NULL && stack->capacity != 0)
    {
        return ERROR_ALLOC_FAILED;
    }

    return SUCCESS;
}

enum stack_errors test_stack()
{
    my_stack_t my_stack = {};

    stack_ctor(&my_stack, 10, sizeof(stack_elem_t));
    stack_dump(&my_stack);
    stack_push(&my_stack, 20);
    stack_dump(&my_stack);
    stack_elem_t to_pop = 0;
    stack_pop(&my_stack, &to_pop);
    stack_dump(&my_stack);

    return SUCCESS;
}
