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
    const int nblocks = len / 4;

    uint32_t h1 = seed;
    const uint32_t *blocks = (const uint32_t *) (data + (nblocks * 4));

    for (int i = -nblocks; i; i++) {
        uint32_t k1 = blocks[i];

        k1 *= 0xcc9e2d51;
        k1 = (k1 << 15) | (k1 >> 17);
        k1 *= 0x1b873593;
        
        h1 ^= k1;
        h1 = (h1 << 13) | (h1 >> 19);
        h1 = (h1 * 5) + 0xe6546b64;
    }

    const uint8_t *tail = (const uint8_t *) (data + (nblocks * 4));
    uint32_t k1 = 0;

    switch(len & 3) {
    case 3:
        k1 ^= tail[2] << 16;
        /* fall through */
    case 2:
        k1 ^= tail[1] << 8;
        /* fall through */
    case 1:
        k1 ^= tail[0];
        k1 *= 0xcc9e2d51;
        k1 = (k1 << 15) | (k1 >> 17);
        k1 *= 0x1b873593;
        h1 ^= k1;
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
    const uint32_t seed;                                                                                     \
    double lf;                                                                                               \
    struct {                                                                                                 \
        struct EntryType *curr;                                                                              \
        unsigned idx;                                                                                        \
    } it;                                                                                                    \
    struct EntryType **buckets;                                                                              \
} TableType;                                                                                                 \
                                                                                                             \
__DS_FUNC_PREFIX_INL DataType* __htable_iter_begin_##id(TableType *ht) {                                     \
    if (!(ht->size)) {                                                                                       \
        ht->it.curr = NULL;                                                                                  \
        ht->it.idx = ht->cap;                                                                                \
    } else {                                                                                                 \
        unsigned idx = 0;                                                                                    \
        for (; (idx < ht->cap) && (ht->buckets[idx] == NULL); ++idx);                                        \
        ht->it.idx = idx;                                                                                    \
        ht->it.curr = ht->buckets[ht->it.idx];                                                               \
    }                                                                                                        \
    return ht->it.curr ? &(ht->it.curr->data) : NULL;                                                        \
}                                                                                                            \
__DS_FUNC_PREFIX_INL DataType* __htable_iter_next_##id(TableType *ht) {                                      \
    if (ht->it.curr->next) {                                                                                 \
        ht->it.curr = ht->it.curr->next;                                                                     \
    } else {                                                                                                 \
        unsigned idx = ht->it.idx + 1;                                                                       \
        for (; (idx < ht->cap) && (ht->buckets[idx] == NULL); ++idx);                                        \
        ht->it.idx = idx;                                                                                    \
        ht->it.curr = (ht->it.idx >= ht->cap) ? NULL : ht->buckets[ht->it.idx];                              \
    }                                                                                                        \
    return ht->it.curr ? &(ht->it.curr->data) : NULL;                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __htable_clear_##id(TableType *ht);                                                    \
__DS_FUNC_PREFIX void __htable_rehash_##id(TableType *ht, size_t nbuckets);                                  \
__DS_FUNC_PREFIX DataType* __htable_find_##id(TableType *ht, const kt key);                                  \
__DS_FUNC_PREFIX DataType* __htable_insert_##id(TableType *ht, DataType data, int *inserted);                \
__DS_FUNC_PREFIX void __htable_insert_fromArray_##id(TableType *ht, DataType* arr, size_t n);                \
__DS_FUNC_PREFIX bool __htable_erase_##id(TableType *ht, const kt key);                                      \
                                                                                                             \
__DS_FUNC_PREFIX TableType *__htable_new_##id(void) {                                                        \
    TableType *ht = __ds_malloc(sizeof(TableType));                                                          \
    srand(time(NULL));                                                                                       \
    memcpy(ht, &((TableType){.cap = 32, .seed = rand() % UINT32_MAX, .lf = 0.75}), sizeof(TableType));       \
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
    TableType temp = {.size = other->size, .cap = other->cap, .seed = other->seed, .lf = other->lf};         \
    memcpy(ht, &temp, sizeof(TableType));                                                                    \
    ht->buckets = __ds_calloc(ht->cap, sizeof(struct EntryType *));                                          \
    for (size_t i = 0; i < other->cap; ++i) {                                                                \
        struct EntryType *e = other->buckets[i];                                                             \
                                                                                                             \
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
__DS_FUNC_PREFIX_INL void __htable_free_##id(TableType *ht) {                                                \
    __htable_clear_##id(ht);                                                                                 \
    free(ht->buckets);                                                                                       \
    free(ht);                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL DataType *__htable_find_##id(TableType *ht, const kt key) {                             \
    uint32_t index = murmurhash(addrOfKey(key), sizeOfKey(key), ht->seed) % ht->cap;                         \
    struct EntryType *e = NULL;                                                                              \
    __htable_find_index(struct EntryType, ht, entry_get_key, cmp_eq, e, index, key)                          \
    return e ? &(e->data) : NULL;                                                                            \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __htable_rehash_##id(TableType *ht, size_t nbuckets) {                                 \
    if (nbuckets <= ht->cap) return;                                                                         \
                                                                                                             \
    size_t n = ht->cap; /* for consistency, use a multiple of 2 of the capacity */                           \
    while (n < nbuckets) {                                                                                   \
        n <<= 1;                                                                                             \
    }                                                                                                        \
                                                                                                             \
    struct EntryType **new = __ds_calloc(n, sizeof(struct EntryType *));                                     \
    struct EntryType *e = NULL, *temp = NULL;                                                                \
    uint32_t index = 0;                                                                                      \
                                                                                                             \
    for (size_t i = 0; i < ht->cap; ++i) {                                                                   \
        e = ht->buckets[i];                                                                                  \
                                                                                                             \
        while (e) {                                                                                          \
            temp = e->next;                                                                                  \
            index = murmurhash(addrOfKey(entry_get_key(e)), sizeOfKey(entry_get_key(e)), ht->seed) % n;      \
            e->next = new[index];                                                                            \
            new[index] = e;                                                                                  \
            e = temp;                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
                                                                                                             \
    free(ht->buckets);                                                                                       \
    ht->buckets = new;                                                                                       \
    ht->cap = n;                                                                                             \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __htable_set_load_factor_##id(TableType *ht, double lf) {                              \
    if (lf < 0.500 || lf > 1.000) return;                                                                    \
    ht->lf = lf;                                                                                             \
    double curr_lf = (double) ht->size / ht->cap;                                                            \
    if (curr_lf > ht->lf) {                                                                                  \
        __htable_rehash_##id(ht, ht->cap << 1);                                                              \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX DataType *__htable_insert_##id(TableType *ht, DataType data, int *inserted) {               \
    double lf = (double) ht->size / ht->cap;                                                                 \
    if (lf > ht->lf) {                                                                                       \
        __htable_rehash_##id(ht, ht->cap << 1);                                                              \
    }                                                                                                        \
                                                                                                             \
    /* get index and entry at that index */                                                                  \
    uint32_t index = murmurhash(addrOfKey(data_get_key(data)), sizeOfKey(data_get_key(data)), ht->seed) % ht->cap; \
    struct EntryType *e = NULL;                                                                              \
    __htable_find_index(struct EntryType, ht, entry_get_key, cmp_eq, e, index, data_get_key(data))           \
    int _inserted = 0;                                                                                       \
                                                                                                             \
    if (e) {                                                                                                 \
        deleteValue(e->data.second);                                                                         \
        copyValue(e->data.second, data.second);                                                              \
    } else {                                                                                                 \
        _inserted = 1;                                                                                       \
        e = __ds_calloc(1, sizeof(struct EntryType));                                                        \
        copyKey(entry_get_key(e), data_get_key(data));                                                       \
        e->next = ht->buckets[index];                                                                        \
        ht->buckets[index] = e;                                                                              \
        ht->size++;                                                                                          \
        copyValue(e->data.second, data.second);                                                              \
    }                                                                                                        \
    if (inserted) *inserted = _inserted;                                                                     \
    return &(e->data);                                                                                       \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __htable_insert_fromArray_##id(TableType *ht, DataType* arr, size_t n) {               \
    if (!(arr && n)) return;                                                                                 \
    for (size_t i = 0; i < n; ++i) {                                                                         \
        __htable_insert_##id(ht, arr[i], NULL);                                                              \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX bool __htable_erase_##id(TableType *ht, const kt key) {                                     \
    uint32_t index = murmurhash(addrOfKey(key), sizeOfKey(key), ht->seed) % ht->cap;                         \
    if (!(ht->buckets[index])) return false; /* this entry does not exist */                                 \
                                                                                                             \
    struct EntryType *prev = ht->buckets[index];                                                             \
    struct EntryType *curr = prev->next;                                                                     \
    if (cmp_eq(entry_get_key(prev), key)) { /* special case when it's the first bucket entry */              \
        ht->buckets[index] = curr;                                                                           \
        deleteKey(entry_get_key(prev));                                                                      \
        deleteValue(prev->data.second);                                                                      \
        free(prev);                                                                                          \
        ht->size--;                                                                                          \
        return true;                                                                                         \
    }                                                                                                        \
                                                                                                             \
    while (curr) {                                                                                           \
        if (cmp_eq(entry_get_key(curr), key)) {                                                              \
            prev->next = curr->next;                                                                         \
            deleteKey(entry_get_key(curr));                                                                  \
            deleteValue(curr->data.second);                                                                  \
            free(curr);                                                                                      \
            ht->size--;                                                                                      \
            return true;                                                                                     \
        }                                                                                                    \
        prev = curr;                                                                                         \
        curr = curr->next;                                                                                   \
    }                                                                                                        \
    return false;                                                                                            \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void __htable_clear_##id(TableType *ht) {                                                   \
    struct EntryType *e = NULL, *temp = NULL;                                                                \
    for (size_t i = 0; i < ht->cap; ++i) { /* iterate over all buckets */                                    \
        e = ht->buckets[i];                                                                                  \
                                                                                                             \
        while (e) { /* iterate through linked list */                                                        \
            temp = e->next;                                                                                  \
            deleteKey(entry_get_key(e));                                                                     \
            deleteValue(e->data.second);                                                                     \
            free(e);                                                                                         \
            e = temp;                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
    memset(ht->buckets, 0, sizeof(struct EntryType *) * ht->cap);                                            \
    ht->size = 0;                                                                                            \
}                                                                                                            \

#endif
