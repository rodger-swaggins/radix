#include <stdint.h>
#include "primitives.h"

#ifndef FCRYPT_HEADER_FILE
#define FCRYPT_HEADER_FILE
    typedef struct cr_state cr_state;

    uint64_t cr_keygen(
        RadixMemoryBlob *key,
        uint64_t seed,
        register uint32_t iterations);

    void cr_init_crypt(cr_state *state, const uint64_t gkey);
    uint8_t cr_operate_byte(uint8_t i, cr_state *state);
    void cr_encrypt_decrypt(RadixMemoryBlob *data, const uint64_t gkey);

    #include "crypt.c"
#endif