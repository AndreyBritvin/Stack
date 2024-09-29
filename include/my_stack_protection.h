#ifndef MY_STACK_PROTECTION_H_
#define MY_STACK_PROTECTION_H_

#define GREEN_COLOR "\033[0;32m"
#define RED_COLOR   "\033[0;31m"

#ifdef CANARY_PROTECTION
#define CANARY_HEXSPEAK 0xDEADBABE
#define CANARY_PROT(...) __VA_ARGS__
#define DATA_CANARY (stack_elem_t) ((canary_t) CANARY_HEXSPEAK ^ (canary_t) stack->data)
#else
#define CANARY_PROT(...)
#endif

#ifdef HASH_PROTECTION
#define HASH_PROT(...) __VA_ARGS__
#else
#define HASH_PROT(...)
#endif

#include <stdlib.h>

typedef unsigned long long hash_t;
typedef unsigned long long canary_t;
typedef char byte;

hash_t hash(void *void_arr_to_hash, size_t arr_size);

#endif // MY_STACK_PROTECTION_H_
