#ifndef MY_ERRORS_H_
#define MY_ERRORS_H_

enum stack_errors
{
    SUCCESS                     =  0,
    ERROR_SIZE_MORE_CAPACITY    =  1,
    ERROR_ALLOC_FAILED          =  2,
    ERROR_STACK_NULL_PTR        =  3,
    ERROR_STACK_DATA_NULL       =  4,
    ERROR_STACK_STATE_NOT_EXIST =  5,
    ERROR_STRUCT_CANARY_DIED    =  6,
    ERROR_BUFFER_CANARY_DIED    =  7,
    ERROR_HASH_IN_STRUCT        =  8,
    ERROR_HASH_IN_BUFFER        =  9,
    ERROR_STACK_UNDERFLOW       = 10,
    ERROR_PRINT_FUNC_IS_NULL    = 11,
    ERROR_POISON_VALUE_PTR_IS_NULL = 12,
};

/*
struct stack_errors
{
    stack_errors_code           code;
    const char const *    text_error;
};
*/

#endif // MY_ERRORS_H_
