#ifndef MY_MACROS_H_
#define MY_MACROS_H_

#define DEBUG

#define PRINT_ERROR(ERROR) fprintf(stderr, "Error #%d ocured in file %s func %s line %d: " #ERROR "\n",\
                                            ERROR, __FILE__, __func__, __LINE__);

#ifdef DEBUG
    #define DEBUG_ON(...) __VA_ARGS__
    #define DEBUG_PRINT(...) fprintf(stdout, __VA_ARGS__);
    #define DUMP_PRINT(text,...) fprintf(stdout, text, __VA_ARGS__);
    #define STACK_DUMP(stack) stack_dump(stack, __FILE__, __func__, __LINE__)
    #define BEGIN_VERIFY stack_errors ST_ERR = SUCCESS;
    #define STACK_VERIFY(stack_) ST_ERR = SUCCESS; if((ST_ERR = stack_verify(stack_)) != SUCCESS){return ST_ERR;}
    #define INIT_STACK(stack) stack.filename = __FILE__; stack.funcname = __func__; stack.linenum = __LINE__; stack.stack_name = #stack;

#else
    #define DEBUG_ON(...)
    #define DEBUG_PRINT(...)
    #define DUMP_PRINT(text,...)
    #define STACK_DUMP(stack) stack_dump(stack)
    #define BEGIN_VERIFY
    #define STACK_VERIFY(stack_)
    #define INIT_STACK(stack)
#endif

#endif //MY_MACROS_H_
