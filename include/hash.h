#ifndef DS_MURMURHASH_H
#define DS_MURMURHASH_H

#include "ds.h"
#include <stdint.h>

unsigned murmurhash(const void *key, int len, uint32_t seed);

#endif
