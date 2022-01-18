#ifndef DS_MURMURHASH_H
#define DS_MURMURHASH_H

unsigned murmurhash(const void *key, int len, unsigned seed)
  __attribute__((nonnull (1)));

#endif /* DS_MURMURHASH_H */
