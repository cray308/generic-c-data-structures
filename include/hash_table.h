#ifndef DS_HASH_TABLE_H
#define DS_HASH_TABLE_H

#include "hash.h"
#include "iterator.h"
#include <time.h>
#include <limits.h>

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

#define __gen_hash_table_headers(id, kt, TableType, DataType, EntryType)                                     \
                                                                                                             \
struct EntryType {                                                                                           \
    struct EntryType *next;                                                                                  \
    DataType data;                                                                                           \
};                                                                                                           \
                                                                                                             \
typedef struct {                                                                                             \
    unsigned size;                                                                                           \
    unsigned cap;                                                                                            \
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
void __htable_rehash_##id(TableType *this, unsigned nbuckets);                                               \
DataType *__htable_insert_##id(TableType *this, DataType data, int *inserted);                               \
void __htable_insert_fromArray_##id(TableType *this, DataType* arr, unsigned n);                             \
TableType *__htable_new_fromArray_##id(DataType *arr, unsigned n);                                           \
TableType *__htable_createCopy_##id(TableType *other);                                                       \
unsigned char __htable_erase_##id(TableType *this, const kt key);                                            \
void __htable_clear_##id(TableType *this);                                                                   \
DataType *__htable_find_##id(TableType *this, const kt key);                                                 \
void __htable_set_load_factor_##id(TableType *this, double lf);

#define __gen_hash_table_source(id, kt, cmp_eq, TableType, DataType, EntryType, entry_get_key, data_get_key, addrOfKey, sizeOfKey, copyKey, deleteKey, copyValue, deleteValue) \
                                                                                                             \
void __htable_rehash_##id(TableType *this, unsigned nbuckets) {                                              \
    unsigned n, i;                                                                                           \
    struct EntryType **new;                                                                                  \
    if (nbuckets <= this->cap) return;                                                                       \
                                                                                                             \
    n = this->cap; /* for consistency, use a multiple of 2 of the capacity */                                \
    while (n < nbuckets) {                                                                                   \
        n <<= 1;                                                                                             \
    }                                                                                                        \
                                                                                                             \
    __ds_calloc(new, n, sizeof(struct EntryType *))                                                          \
    for (i = 0; i < this->cap; ++i) {                                                                        \
        struct EntryType *e = this->buckets[i];                                                              \
        while (e) {                                                                                          \
            struct EntryType *temp = e->next;                                                                \
            unsigned index = murmurhash(addrOfKey(entry_get_key(e)), (int) sizeOfKey(entry_get_key(e)), this->seed) % n; \
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
DataType *__htable_insert_##id(TableType *this, DataType data, int *inserted) {                              \
    unsigned index; struct EntryType *e = NULL;                                                              \
    double lf = (double) this->size / (double) this->cap;                                                    \
    if (lf > this->lf) {                                                                                     \
        __htable_rehash_##id(this, this->cap << 1);                                                          \
    }                                                                                                        \
                                                                                                             \
    /* get index and entry at that index */                                                                  \
    index = murmurhash(addrOfKey(data_get_key(data)), (int) sizeOfKey(data_get_key(data)), this->seed) % this->cap; \
    __htable_find_index(struct EntryType, this, entry_get_key, cmp_eq, e, index, data_get_key(data))         \
                                                                                                             \
    if (e) {                                                                                                 \
        if (inserted) *inserted = 0;                                                                         \
        deleteValue(e->data.second);                                                                         \
        copyValue(e->data.second, data.second);                                                              \
    } else {                                                                                                 \
        if (inserted) *inserted = 1;                                                                         \
        __ds_calloc(e, 1, sizeof(struct EntryType))                                                          \
        copyKey(entry_get_key(e), data_get_key(data));                                                       \
        e->next = this->buckets[index];                                                                      \
        this->buckets[index] = e;                                                                            \
        this->size++;                                                                                        \
        copyValue(e->data.second, data.second);                                                              \
    }                                                                                                        \
    return &(e->data);                                                                                       \
}                                                                                                            \
                                                                                                             \
void __htable_insert_fromArray_##id(TableType *this, DataType* arr, unsigned n) {                            \
    unsigned i;                                                                                              \
    if (!(arr && n)) return;                                                                                 \
    for (i = 0; i < n; ++i) {                                                                                \
        __htable_insert_##id(this, arr[i], NULL);                                                            \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
TableType *__htable_new_fromArray_##id(DataType *arr, unsigned n) {                                          \
    TableType *ht;                                                                                           \
    __ds_calloc(ht, 1, sizeof(TableType))                                                                    \
    srand((unsigned) time(NULL));                                                                            \
    ht->cap = 32;                                                                                            \
    ht->lf = 0.75;                                                                                           \
    ht->seed = ((uint32_t) rand()) % UINT32_MAX;                                                             \
    __ds_calloc(ht->buckets, ht->cap, sizeof(struct EntryType *))                                            \
    __htable_insert_fromArray_##id(ht, arr, n);                                                              \
    return ht;                                                                                               \
}                                                                                                            \
                                                                                                             \
TableType *__htable_createCopy_##id(TableType *other) {                                                      \
    TableType *ht;                                                                                           \
    unsigned i;                                                                                              \
    __ds_malloc(ht, sizeof(TableType))                                                                       \
    ht->size = other->size;                                                                                  \
    ht->cap = other->cap;                                                                                    \
    ht->seed = other->seed;                                                                                  \
    ht->lf = other->lf;                                                                                      \
    __ds_calloc(ht->buckets, ht->cap, sizeof(struct EntryType *))                                            \
    for (i = 0; i < other->cap; ++i) {                                                                       \
        struct EntryType *e = other->buckets[i];                                                             \
        while (e) {                                                                                          \
            struct EntryType *new;                                                                           \
            __ds_calloc(new, 1, sizeof(struct EntryType))                                                    \
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
unsigned char __htable_erase_##id(TableType *this, const kt key) {                                           \
    struct EntryType *prev, *curr;                                                                           \
    unsigned index = murmurhash(addrOfKey(key), (int) sizeOfKey(key), this->seed) % this->cap;               \
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
void __htable_clear_##id(TableType *this) {                                                                  \
    unsigned i;                                                                                              \
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
DataType *__htable_find_##id(TableType *this, const kt key) {                                                \
    unsigned index = murmurhash(addrOfKey(key), (int) sizeOfKey(key), this->seed) % this->cap;               \
    struct EntryType *e = NULL;                                                                              \
    __htable_find_index(struct EntryType, this, entry_get_key, cmp_eq, e, index, key)                        \
    return e ? &(e->data) : NULL;                                                                            \
}                                                                                                            \
                                                                                                             \
void __htable_set_load_factor_##id(TableType *this, double lf) {                                             \
    double curr_lf;                                                                                          \
    if (lf < 0.500 || lf > 1.000) return;                                                                    \
    this->lf = lf;                                                                                           \
    curr_lf = (double) this->size / (double) this->cap;                                                      \
    if (curr_lf > this->lf) {                                                                                \
        __htable_rehash_##id(this, this->cap << 1);                                                          \
    }                                                                                                        \
}                                                                                                            \

#endif
