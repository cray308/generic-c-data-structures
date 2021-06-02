#ifndef DS_HASH_TABLE_H
#define DS_HASH_TABLE_H

#include "ds.h"
#include "iterator.h"
#include "pair.h"
#include <time.h>
#include <limits.h>
#include <stdint.h>

uint32_t murmurhash(const void *key, int len, uint32_t seed) {
    const uint8_t *data = (const uint8_t *) key;
    const int nblocks = len >> 2; int i;
    uint32_t h1 = seed, k = 0;
    uint32_t const *blocks;
    uint8_t const *tail;
    blocks = (const uint32_t *) (data + (nblocks * 4));
    tail = (const uint8_t *) (data + (nblocks * 4));

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
        k ^= tail[2] << 16;
        /* fall through */
    case 2:
        k ^= tail[1] << 8;
        /* fall through */
    case 1:
        k ^= tail[0];
        k *= 0xcc9e2d51;
        k = (k << 15) | (k >> 17);
        k *= 0x1b873593;
        h1 ^= k;
    };

    h1 ^= len;
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;
    return h1;
}

#define __htable_find_index(EntryType, ht, entry_get_key, cmp_eq, result, index, key) {                      \
    EntryType *ptr = (ht)->buckets[(index)];                                                                 \
    while (ptr) {                                                                                            \
        if (cmp_eq(entry_get_key(ptr), key)) {                                                               \
            (result) = ptr; break;                                                                           \
        }                                                                                                    \
        ptr = ptr->next;                                                                                     \
    }                                                                                                        \
}

/* --------------------------------------------------------------------------
 * Hash table iterators
 * -------------------------------------------------------------------------- */

#define iter_begin_HTABLE(id, ht)  __htable_iter_begin_##id(ht)
#define iter_end_HTABLE(id, ht)    NULL
#define iter_next_HTABLE(id, ht, p)   ((p) = __htable_iter_next_##id(ht))

#define __gen_hash_table(id, kt, cmp_eq, TableType, DataType, EntryType, entry_get_key, data_get_key, addrOfKey, sizeOfKey, copyKey, deleteKey, copyValue, deleteValue) \
                                                                                                             \
struct EntryType {                                                                                           \
    struct EntryType *next;                                                                                  \
    DataType data;                                                                                           \
};                                                                                                           \
                                                                                                             \
typedef struct {                                                                                             \
    size_t size;                                                                                             \
    size_t cap;                                                                                              \
    uint32_t seed;                                                                                           \
    double lf;                                                                                               \
    struct {                                                                                                 \
        struct EntryType *curr;                                                                              \
        unsigned idx;                                                                                        \
    } it;                                                                                                    \
    struct EntryType **buckets;                                                                              \
} TableType;                                                                                                 \
                                                                                                             \
__DS_FUNC_PREFIX_INL DataType* __htable_iter_begin_##id(TableType *this) {                                   \
    if (!(this->size)) {                                                                                     \
        this->it.curr = NULL;                                                                                \
        this->it.idx = this->cap;                                                                            \
    } else {                                                                                                 \
        unsigned idx = 0;                                                                                    \
        for (; (idx < this->cap) && (this->buckets[idx] == NULL); ++idx);                                    \
        this->it.idx = idx;                                                                                  \
        this->it.curr = this->buckets[this->it.idx];                                                         \
    }                                                                                                        \
    return this->it.curr ? &(this->it.curr->data) : NULL;                                                    \
}                                                                                                            \
__DS_FUNC_PREFIX_INL DataType* __htable_iter_next_##id(TableType *this) {                                    \
    if (this->it.curr->next) {                                                                               \
        this->it.curr = this->it.curr->next;                                                                 \
    } else {                                                                                                 \
        unsigned idx = this->it.idx + 1;                                                                     \
        for (; (idx < this->cap) && (this->buckets[idx] == NULL); ++idx);                                    \
        this->it.idx = idx;                                                                                  \
        this->it.curr = (this->it.idx >= this->cap) ? NULL : this->buckets[this->it.idx];                    \
    }                                                                                                        \
    return this->it.curr ? &(this->it.curr->data) : NULL;                                                    \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __htable_rehash_##id(TableType *this, size_t nbuckets) {                               \
    size_t n; uint32_t i;                                                                                    \
    struct EntryType **new;                                                                                  \
    if (nbuckets <= this->cap) return;                                                                       \
                                                                                                             \
    n = this->cap; /* for consistency, use a multiple of 2 of the capacity */                                \
    while (n < nbuckets) {                                                                                   \
        n <<= 1;                                                                                             \
    }                                                                                                        \
                                                                                                             \
    new = __ds_calloc(n, sizeof(struct EntryType *));                                                        \
    for (i = 0; i < this->cap; ++i) {                                                                        \
        struct EntryType *e = this->buckets[i];                                                              \
        while (e) {                                                                                          \
            struct EntryType *temp = e->next;                                                                \
            uint32_t index = murmurhash(addrOfKey(entry_get_key(e)), sizeOfKey(entry_get_key(e)), this->seed) % n; \
            e->next = new[index];                                                                            \
            new[index] = e;                                                                                  \
            e = temp;                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
                                                                                                             \
    free(this->buckets);                                                                                     \
    this->buckets = new;                                                                                     \
    this->cap = n;                                                                                           \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX DataType *__htable_insert_##id(TableType *this, DataType data, int *inserted) {             \
    uint32_t index; struct EntryType *e = NULL;                                                              \
    double lf = (double) this->size / this->cap;                                                             \
    if (lf > this->lf) {                                                                                     \
        __htable_rehash_##id(this, this->cap << 1);                                                          \
    }                                                                                                        \
                                                                                                             \
    /* get index and entry at that index */                                                                  \
    index = murmurhash(addrOfKey(data_get_key(data)), sizeOfKey(data_get_key(data)), this->seed) % this->cap; \
    __htable_find_index(struct EntryType, this, entry_get_key, cmp_eq, e, index, data_get_key(data))         \
                                                                                                             \
    if (e) {                                                                                                 \
        if (inserted) *inserted = 0;                                                                         \
        deleteValue(e->data.second);                                                                         \
        copyValue(e->data.second, data.second);                                                              \
    } else {                                                                                                 \
        if (inserted) *inserted = 1;                                                                         \
        e = __ds_calloc(1, sizeof(struct EntryType));                                                        \
        copyKey(entry_get_key(e), data_get_key(data));                                                       \
        e->next = this->buckets[index];                                                                      \
        this->buckets[index] = e;                                                                            \
        this->size++;                                                                                        \
        copyValue(e->data.second, data.second);                                                              \
    }                                                                                                        \
    return &(e->data);                                                                                       \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __htable_insert_fromArray_##id(TableType *this, DataType* arr, size_t n) {             \
    size_t i;                                                                                                \
    if (!(arr && n)) return;                                                                                 \
    for (i = 0; i < n; ++i) {                                                                                \
        __htable_insert_##id(this, arr[i], NULL);                                                            \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX TableType *__htable_new_##id(void) {                                                        \
    TableType *ht = __ds_calloc(1, sizeof(TableType));                                                       \
    srand(time(NULL));                                                                                       \
    ht->cap = 32;                                                                                            \
    ht->lf = 0.75;                                                                                           \
    ht->seed = rand() % UINT32_MAX;                                                                          \
    ht->buckets = __ds_calloc(ht->cap, sizeof(struct EntryType *));                                          \
    return ht;                                                                                               \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX TableType *__htable_new_fromArray_##id(DataType *arr, size_t n) {                           \
    TableType *ht = __htable_new_##id();                                                                     \
    __htable_insert_fromArray_##id(ht, arr, n);                                                              \
    return ht;                                                                                               \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX TableType *__htable_createCopy_##id(TableType *other) {                                     \
    TableType *ht = __ds_malloc(sizeof(TableType));                                                          \
    size_t i;                                                                                                \
    ht->size = other->size;                                                                                  \
    ht->cap = other->cap;                                                                                    \
    ht->seed = other->seed;                                                                                  \
    ht->lf = other->lf;                                                                                      \
    ht->buckets = __ds_calloc(ht->cap, sizeof(struct EntryType *));                                          \
    for (i = 0; i < other->cap; ++i) {                                                                       \
        struct EntryType *e = other->buckets[i];                                                             \
        while (e) {                                                                                          \
            struct EntryType *new = __ds_calloc(1, sizeof(struct EntryType));                                \
            copyKey(entry_get_key(new), entry_get_key(e));                                                   \
            new->next = ht->buckets[i];                                                                      \
            ht->buckets[i] = new;                                                                            \
            copyValue(new->data.second, e->data.second);                                                     \
            e = e->next;                                                                                     \
        }                                                                                                    \
    }                                                                                                        \
    return ht;                                                                                               \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX unsigned char __htable_erase_##id(TableType *this, const kt key) {                          \
    struct EntryType *prev, *curr;                                                                           \
    uint32_t index = murmurhash(addrOfKey(key), sizeOfKey(key), this->seed) % this->cap;                     \
    if (!(this->buckets[index])) return 0; /* this entry does not exist */                                   \
                                                                                                             \
    prev = this->buckets[index], curr = prev->next;                                                          \
    if (cmp_eq(entry_get_key(prev), key)) { /* special case when it's the first bucket entry */              \
        this->buckets[index] = curr;                                                                         \
        deleteKey(entry_get_key(prev));                                                                      \
        deleteValue(prev->data.second);                                                                      \
        free(prev);                                                                                          \
        this->size--;                                                                                        \
        return 1;                                                                                            \
    }                                                                                                        \
                                                                                                             \
    while (curr) {                                                                                           \
        if (cmp_eq(entry_get_key(curr), key)) {                                                              \
            prev->next = curr->next;                                                                         \
            deleteKey(entry_get_key(curr));                                                                  \
            deleteValue(curr->data.second);                                                                  \
            free(curr);                                                                                      \
            this->size--;                                                                                    \
            return 1;                                                                                        \
        }                                                                                                    \
        prev = curr;                                                                                         \
        curr = curr->next;                                                                                   \
    }                                                                                                        \
    return 0;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __htable_clear_##id(TableType *this) {                                                 \
    size_t i;                                                                                                \
    for (i = 0; i < this->cap; ++i) { /* iterate over all buckets */                                         \
        struct EntryType *e = this->buckets[i];                                                              \
                                                                                                             \
        while (e) { /* iterate through linked list */                                                        \
            struct EntryType *temp = e->next;                                                                \
            deleteKey(entry_get_key(e));                                                                     \
            deleteValue(e->data.second);                                                                     \
            free(e);                                                                                         \
            e = temp;                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
    memset(this->buckets, 0, sizeof(struct EntryType *) * this->cap);                                        \
    this->size = 0;                                                                                          \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void __htable_free_##id(TableType *this) {                                              \
    __htable_clear_##id(this);                                                                               \
    free(this->buckets);                                                                                     \
    free(this);                                                                                              \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL DataType *__htable_find_##id(TableType *this, const kt key) {                           \
    uint32_t index = murmurhash(addrOfKey(key), sizeOfKey(key), this->seed) % this->cap;                     \
    struct EntryType *e = NULL;                                                                              \
    __htable_find_index(struct EntryType, this, entry_get_key, cmp_eq, e, index, key)                        \
    return e ? &(e->data) : NULL;                                                                            \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __htable_set_load_factor_##id(TableType *this, double lf) {                            \
    double curr_lf;                                                                                          \
    if (lf < 0.500 || lf > 1.000) return;                                                                    \
    this->lf = lf;                                                                                           \
    curr_lf = (double) this->size / this->cap;                                                               \
    if (curr_lf > this->lf) {                                                                                \
        __htable_rehash_##id(this, this->cap << 1);                                                          \
    }                                                                                                        \
}                                                                                                            \

#endif
