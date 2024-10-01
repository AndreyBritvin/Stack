#ifndef MY_STACK_PROTECTION_H_
#define MY_STACK_PROTECTION_H_

#ifdef CANARY_PROTECTION
#define CANARY_DBG(...) __VA_ARGS__
#else
<<<<<<< Updated upstream
#define CANARY_DBG(...)
=======
#define CANARY_HEXSPEAK
#define CANARY_PROT(...)
#define DATA_CANARY
>>>>>>> Stashed changes
#endif

#ifdef HASH_PROTECTION
#define HASH_DBG(...) __VA_ARGS__
#else
#define HASH_DBG(...)
#endif

#endif // MY_STACK_PROTECTION_H_
