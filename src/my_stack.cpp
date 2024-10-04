#include "my_stack.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

static stack_errors stack_realloc(my_stack_t *stack, stack_state state);

#ifdef HASH_PROTECTION
    static stack_errors recalc_hash(my_stack_t *stack);
    static hash_t calc_buffer_hash(my_stack_t *stack);
    static hash_t calc_struct_hash(my_stack_t *stack);
#endif // HASH_PROTECTION

const char * const BEGIN_DUMP = "[----------------BEGIN STACK_DUMP----------------]";
const char * const END_DUMP   = "[-----------------END STACK_DUMP-----------------]";

stack_errors stack_dump(my_stack_t *stack, dump_print_t dump_print_func DEBUG_ON(, const char *filename, const char *funcname, int codeline))
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

    CANARY_PROT(DUMP_PRINT("data canary #1 = %llx\n{\n", (canary_t)stack->data[-1]);)

    // void *delta_ptr = stack->data;
    for (size_t i = 0; i < stack->capacity; i++)
    {
        dump_print_func(stack->data, i, stack->size, stack->elem_size);
    }

    CANARY_PROT(DUMP_PRINT("}\ndata canary #2 = %llx\n", (canary_t)stack->data[stack->capacity]);)

    CANARY_PROT(DUMP_PRINT("Canary #2 = %llx\n", stack->canary_right);)
    DUMP_PRINT("%s\n\n", END_DUMP);

    return SUCCESS;
}

stack_errors stack_ctor(my_stack_t *stack, size_t capacity, size_t el_size)
{
    BEGIN_VERIFY
    if (stack == NULL)
    {
        PRINT_ERROR(ERROR_STACK_NULL_PTR);

        return ERROR_STACK_NULL_PTR;
    }

    stack->capacity  = capacity;
    stack->elem_size = el_size;
    stack->size      = NULL_POS;
    stack->data = (stack_elem_t *) calloc(el_size * capacity CANARY_PROT(+ sizeof(canary_t) * 2), sizeof(char));

    CANARY_PROT(stack->data++;)
    // CANARY_PROT(stack->data[-1]       = (stack_elem_t) DATA_CANARY;)
    // CANARY_PROT(stack->data[capacity] = (stack_elem_t) DATA_CANARY;)

    CANARY_PROT(canary_t temp_can_val = DATA_CANARY;)
    CANARY_PROT(memcpy(&stack->data[-1],              &temp_can_val, sizeof(canary_t));)
    CANARY_PROT(memcpy(&stack->data[stack->capacity], &temp_can_val, sizeof(canary_t));)

    CANARY_PROT(stack->canary_left  = STACK_CANARY_VALUE;)
    CANARY_PROT(stack->canary_right = STACK_CANARY_VALUE;)

    HASH_PROT(
    recalc_hash(stack);

    DEBUG_PRINT("Hash struct val = %llx\n", stack->struct_hash);
    DEBUG_PRINT("Hash buffer val = %llx\n", stack->buffer_hash);
    )

    STACK_VERIFY(stack);

    return SUCCESS;
}

stack_errors stack_dtor(my_stack_t *stack)
{
    if (stack == NULL)
    {
        PRINT_ERROR(ERROR_STACK_NULL_PTR);

        return ERROR_STACK_NULL_PTR;
    }

    if (stack->data != NULL)
    {
        memset(stack->data, 0, sizeof(stack->data[0]) * stack->capacity); // REDO canary?

        free(stack->data CANARY_PROT(-1)); stack->data = NULL;
    }

    stack->size = NULL_POS;
    stack->capacity = 0;

    return SUCCESS;
}

stack_errors stack_pop(my_stack_t *stack, stack_elem_t *el_to_pop) // TODO:
{
    assert(el_to_pop != NULL);
    BEGIN_VERIFY
    DEBUG_PRINT("Entered stack_pop\n");
    STACK_VERIFY(stack);

    if (stack->size == 0)
    {
        PRINT_ERROR(ERROR_STACK_UNDERFLOW);

        return      ERROR_STACK_UNDERFLOW;
    }

    *el_to_pop = stack->data[stack->size - 1];
    stack->data[stack->size - 1] = STACK_POISON_VALUE;
    stack->size--;

    recalc_hash(stack);

    if (stack->size <= stack->capacity / (2 * ALLOC_CONST))
    {
        stack_realloc(stack, POPING);
    }

    STACK_VERIFY(stack);
    DEBUG_PRINT("Quit stack_pop\n");

    return SUCCESS;
}

stack_errors stack_push(my_stack_t *stack, stack_elem_t el_to_push)
{
    BEGIN_VERIFY
    DEBUG_PRINT("Entered stack_push\n");
    STACK_VERIFY(stack);
    stack->data[stack->size] = el_to_push;
    stack->size++;

    recalc_hash(stack);

    if (stack->size >= stack->capacity)
    {
        stack_realloc(stack, PUSHING);
    }

    STACK_VERIFY(stack);

    DEBUG_PRINT("Quit stack_push\n");

    return SUCCESS;
}

static stack_errors stack_realloc(my_stack_t *stack, stack_state state)
{
    BEGIN_VERIFY
    STACK_VERIFY(stack);

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

    if (stack->data == NULL)
    {
        PRINT_ERROR(ERROR_STACK_DATA_NULL);

        return ERROR_STACK_DATA_NULL;
    }

    // stack->data[-1]              = (stack_elem_t) DATA_CANARY;
    // stack->data[stack->capacity] = (stack_elem_t) DATA_CANARY;
    CANARY_PROT(canary_t temp_can_val = DATA_CANARY;)
    CANARY_PROT(memcpy(&stack->data[-1],              &temp_can_val, sizeof(canary_t));)
    CANARY_PROT(memcpy(&stack->data[stack->capacity], &temp_can_val, sizeof(canary_t));)

    recalc_hash(stack);
    STACK_VERIFY(stack);
    STACK_DUMP(stack);

    return SUCCESS;
}

stack_errors stack_verify(my_stack_t *stack)
{
    if (stack == NULL)
    {
        PRINT_ERROR(ERROR_STACK_NULL_PTR);

        return      ERROR_STACK_NULL_PTR;
    }

    HASH_PROT(
    if (stack->struct_hash != calc_struct_hash(stack))
    {
        PRINT_ERROR(ERROR_HASH_IN_STRUCT);

        return      ERROR_HASH_IN_STRUCT;
    }
    )

    CANARY_PROT(
    if (stack->canary_left != STACK_CANARY_VALUE || stack->canary_right != STACK_CANARY_VALUE)
    {
        PRINT_ERROR(ERROR_STRUCT_CANARY_DIED);

        return      ERROR_STRUCT_CANARY_DIED;
    }
    )

    if (stack->size > stack->capacity)
    {
        PRINT_ERROR(ERROR_SIZE_MORE_CAPACITY);

        return      ERROR_SIZE_MORE_CAPACITY;
    }

    if (stack->data == NULL && stack->capacity == 0)
    {
        return SUCCESS;
    }

    if (stack->data == NULL)
    {
        PRINT_ERROR(ERROR_ALLOC_FAILED);

        return      ERROR_ALLOC_FAILED;
    }

    HASH_PROT(
    if (stack->buffer_hash != calc_buffer_hash(stack))
    {
        PRINT_ERROR(ERROR_HASH_IN_BUFFER);

        return      ERROR_HASH_IN_BUFFER;
    }
    )

    CANARY_PROT(
    canary_t temp_can_value = DATA_CANARY;

    // printf("Canary_left_data  = %llx\n"
        //    "Canary_temp_data  = %llx\n", (canary_t) stack->data[-1], temp_can_value);
    // printf("Canary_right_data = %llx\n"
        //    "Canary_temp_data  = %llx\n", (canary_t) stack->data[stack->capacity], temp_can_value);

    if (   memcmp(&stack->data[-1],              &temp_can_value, sizeof(canary_t))
        || memcmp(&stack->data[stack->capacity], &temp_can_value, sizeof(canary_t)))
    {
        PRINT_ERROR(ERROR_BUFFER_CANARY_DIED);
        // printf("Memcmp value: %d, \n", memcmp(&stack->data[-1], &temp_can_value, sizeof(canary_t)));
        return      ERROR_BUFFER_CANARY_DIED;
    }
    )

    return SUCCESS;
}

#ifdef HASH_PROTECTION

static stack_errors recalc_hash(my_stack_t *stack)
{
    stack->buffer_hash = calc_buffer_hash(stack);
    stack->struct_hash = calc_struct_hash(stack);

    return SUCCESS;
}

static hash_t calc_struct_hash(my_stack_t *stack)
{
    return hash(stack, 0 + CANARY_PROT(+ sizeof(canary_t)) + 2 * sizeof(size_t) + sizeof(stack_elem_t*) +
                       sizeof(hash_t));
}

static hash_t calc_buffer_hash(my_stack_t *stack)
{
    return hash(stack->data - 1, stack->capacity * sizeof(stack_elem_t) CANARY_PROT(+ 2 * sizeof(canary_t)));
}

#endif // HASH_PROTECTION

#define BEGIN_STACK_CHECKING enum stack_errors STACK_ERR = SUCCESS;
#define CHECK_STACK if(!STACK_ERR) STACK_ERR =
#define END_STACK_CHECKING(macro_my_stack) if(STACK_ERR) {                                    \
                                printf("During stack checking error #%d occured", STACK_ERR); \
                                STACK_DUMP(macro_my_stack);                                   \
                                stack_dtor(macro_my_stack);                                   \
                                return STACK_ERR;                                             \
                                }

void *print_doubles(void *void_begin_ptr, size_t iteration, size_t size, size_t el_size)
{
    assert(void_begin_ptr != NULL);

    char *begin_ptr = (char *)void_begin_ptr;
    char *cur_ptr = begin_ptr + iteration * el_size;

    if (iteration < size)
    {
        DUMP_PRINT("   *[i] = %lu elem = %lf (%lx)\n", iteration, *(double *)(cur_ptr), *(size_t *)(cur_ptr));
    }
    else
    {
        DUMP_PRINT("    [i] = %lu elem = %lf (%lx)\n", iteration, *(double *)(cur_ptr), *(size_t *)(cur_ptr));
    }

    return cur_ptr;
}

void *print_longs(void *void_begin_ptr, size_t iteration, size_t size, size_t el_size)
{
    assert(void_begin_ptr != NULL);

    char *begin_ptr = (char *)void_begin_ptr;
    char *cur_ptr = begin_ptr + iteration * el_size;

    if (iteration < size)
    {
        DUMP_PRINT("   *[i] = %lu elem = %lld (%lx)\n", iteration, *(long long *)(cur_ptr), *(size_t *)(cur_ptr));
    }
    else
    {
        DUMP_PRINT("    [i] = %lu elem = %lld (%lx)\n", iteration, *(long long *)(cur_ptr), *(size_t *)(cur_ptr));
    }

    return cur_ptr;
}

enum stack_errors test_stack()
{
    my_stack_t my_stack = {};
    INIT_STACK(my_stack);
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

    // my_stack.struct_hash += 1;
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
