#include "my_stack_protection.h"
#include <assert.h>

hash_t hash(void *void_arr_to_hash, size_t arr_size)
{
    assert(void_arr_to_hash != NULL);

    hash_t hash = 8765;
    byte *arr_to_hash = (byte *) void_arr_to_hash;

    for(size_t i = 0; i < arr_size; i++)
    {
        hash = ((hash << 5) + hash) + (hash_t) arr_to_hash[i]; /* hash * 33 + c */
    }

    return hash;
}
