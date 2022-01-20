#include "hash.h"
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

__attribute__((constructor)) static void hashInit(void) {
    srand((unsigned) time(NULL));
}

unsigned murmurhash(const void *key, int len, unsigned seed) {
    const uint8_t *data = (const uint8_t *) key;
    const int nblocks = len >> 2; int i;
    uint32_t h1 = seed, k = 0;
    uint32_t const *blocks = (const uint32_t *) (data + (nblocks << 2));
    uint8_t const *tail = (const uint8_t *) (data + (nblocks << 2));

    for (i = -nblocks; i; i++) {
        uint32_t k1 = blocks[i];

        k1 *= 0xcc9e2d51;
        k1 = (k1 << 15) | (k1 >> 17);
        k1 *= 0x1b873593;

        h1 ^= k1;
        h1 = (h1 << 13) | (h1 >> 19);
        h1 = (h1 * 5) + 0xe6546b64;
    }

    switch(len & 3) {
    case 3:
        k ^= (uint32_t)(tail[2] << 16);
        /* fall through */
    case 2:
        k ^= (uint32_t)(tail[1] << 8);
        /* fall through */
    case 1:
        k ^= tail[0];
        k *= 0xcc9e2d51;
        k = (k << 15) | (k >> 17);
        k *= 0x1b873593;
        h1 ^= k;
    };

    h1 ^= (uint32_t) len;
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;
    return h1;
}
