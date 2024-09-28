#ifndef MY_MACROS_H_
#define MY_MACROS_H_

#ifndef NDEBUG
#define DEBUG_ON(...) __VA_ARGS__
#define DEBUG_PRINT(...) fprintf(stdout, __VA_ARGS__);
#define DUMP_PRINT(text,...) fprintf(stdout, text, __VA_ARGS__);
#define STACK_DUMP(stack) stack_dump(stack, __FILE__, __func__, __LINE__)
#define PRINT_ERROR(ERROR) fprintf(stderr, "Error #%d ocured in file %s func %s line %d: " #ERROR "\n",\
                                            ERROR, __FILE__, __func__, __LINE__);
#else
#define DEBUG_ON(...)
#define DEBUG_PRINT(...)
#define DUMP_PRINT(text,...)
#define STACK_DUMP(stack) stack_dump(stack)
#endif

#endif //MY_MACROS_H_
