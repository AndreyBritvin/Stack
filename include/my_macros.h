#ifndef MY_MACROS_H_
#define MY_MACROS_H_

#ifndef NDEBUG
#define DEBUG_ON(...) __VA_ARGS__
#define DEBUG_PRINT(...) fprintf(stdout, __VA_ARGS__);
#else
#define DEBUG_ON(...)
#define DEBUG_PRINT(...)
#endif

#endif //MY_MACROS_H_
