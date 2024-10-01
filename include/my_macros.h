#ifndef MY_MACROS_H_
#define MY_MACROS_H_

#ifndef NDEBUG
#define DEBUG_ON(...) __VA_ARGS__
#define DEBUG_PRINT(...) fprintf(stdout, __VA_ARGS__);
#define PRINT_ERROR(ERROR) fprintf(stdout, "Error #%d ocured in file %s func %s line %d: " #ERROR "\n",\
                                            ERROR, __FILE__, __func__, __LINE__);
#else
<<<<<<< Updated upstream
#define DEBUG_ON(...)
#define DEBUG_PRINT(...)
=======
    #define DEBUG_ON(...)
    #define DEBUG_PRINT(...)
    #define DUMP_PRINT(text,...)
    #define STACK_DUMP(stack) stack_dump(stack)
    #define STACK_VERIFY(stack_)
    #define INIT_STACK(stack)
    #define PRINT_ERROR(ERROR)
    #define BEGIN_VERIFY
>>>>>>> Stashed changes
#endif

#endif //MY_MACROS_H_
