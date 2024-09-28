#ifndef MY_STACK_PROTECTION_H_
#define MY_STACK_PROTECTION_H_

#ifdef CANARY_PROTECTION
#define CANARY_HEXSPEAK 0xDEADBABE
#define CANARY_PROT(...) __VA_ARGS__
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

#endif // MY_STACK_PROTECTION_H_
