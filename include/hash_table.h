#ifndef DS_HASH_TABLE_H
#define DS_HASH_TABLE_H

#include "ds.h"
#include "hash.h"
#include <time.h>

#if UINT_MAX == 0xffffffff
#define DS_HTABLE_MAX_SIZE 42949672
#define DS_HTABLE_SHIFT_THRESHOLD 21474836
#elif UINT_MAX == 0xffff
#define DS_HTABLE_MAX_SIZE 655
#define DS_HTABLE_SHIFT_THRESHOLD 327
#endif

#define __setup_hash_table_headers(id, kt, TableType, DataType, EntryType)                                   \
                                                                                                             \
struct EntryType {                                                                                           \
    struct EntryType *next;                                                                                  \
    DataType data;                                                                                           \
};                                                                                                           \
                                                                                                             \
typedef struct {                                                                                             \
    unsigned size;                                                                                           \
    unsigned cap;                                                                                            \
    unsigned threshold;                                                                                      \
    unsigned lf;                                                                                             \
    unsigned seed;                                                                                           \
    struct {                                                                                                 \
        struct EntryType *curr;                                                                              \
        unsigned idx;                                                                                        \
    } it;                                                                                                    \
    struct EntryType **buckets;                                                                              \
} TableType;                                                                                                 \
                                                                                                             \
DataType* __htable_iter_begin_##id(TableType *this);                                                         \
DataType* __htable_iter_next_##id(TableType *this);                                                          \
                                                                                                             \
unsigned char __htable_rehash_##id(TableType *this, unsigned nbuckets);                                      \
DataType *__htable_insert_##id(TableType *this,                                                              \
                               const DataType data, int *inserted);                                          \
unsigned char __htable_insert_fromArray_##id(TableType *this,                                                \
                                             DataType const *arr, unsigned n);                               \
TableType *__htable_new_fromArray_##id(DataType const *arr, unsigned n);                                     \
TableType *__htable_createCopy_##id(TableType const *other);                                                 \
unsigned char __htable_erase_##id(TableType *this, const kt key);                                            \
void __htable_clear_##id(TableType *this);                                                                   \
DataType *__htable_find_##id(TableType const *this, const kt key);                                           \
unsigned char __htable_set_load_factor_##id(TableType *this, unsigned lf);                                   \

#define __setup_hash_table_source(id, kt, cmp_eq, TableType, DataType,                                       \
                                  EntryType, entry_get_key, data_get_key,                                    \
                                  addrOfKey, sizeOfKey, copyKey, deleteKey,                                  \
                                  copyValue, deleteValue)                                                    \
                                                                                                             \
DataType* __htable_iter_begin_##id(TableType *this) {                                                        \
    if (!this->size) {                                                                                       \
        this->it.curr = NULL;                                                                                \
        this->it.idx = this->cap;                                                                            \
    } else {                                                                                                 \
        unsigned idx;                                                                                        \
        for (idx = 0; idx < this->cap && this->buckets[idx] == NULL; ++idx);                                 \
        this->it.idx = idx;                                                                                  \
        this->it.curr = this->buckets[this->it.idx];                                                         \
    }                                                                                                        \
    return this->it.curr ? &this->it.curr->data : NULL;                                                      \
}                                                                                                            \
                                                                                                             \
DataType* __htable_iter_next_##id(TableType *this) {                                                         \
    if (this->it.curr->next) {                                                                               \
        this->it.curr = this->it.curr->next;                                                                 \
    } else {                                                                                                 \
        unsigned idx = this->it.idx + 1;                                                                     \
        for (; idx < this->cap && this->buckets[idx] == NULL; ++idx);                                        \
        this->it.idx = idx;                                                                                  \
        this->it.curr = (this->it.idx >= this->cap) ?                                                        \
            NULL : this->buckets[this->it.idx];                                                              \
    }                                                                                                        \
    return this->it.curr ? &this->it.curr->data : NULL;                                                      \
}                                                                                                            \
                                                                                                             \
unsigned char __htable_rehash_##id(TableType *this, unsigned nbuckets) {                                     \
    unsigned ncap = this->cap, i;                                                                            \
    struct EntryType **new;                                                                                  \
    struct EntryType *e, *next;                                                                              \
    if (nbuckets <= ncap) return 1;                                                                          \
    else if (ncap == DS_HTABLE_MAX_SIZE) return 0;                                                           \
                                                                                                             \
    if (nbuckets < DS_HTABLE_SHIFT_THRESHOLD) {                                                              \
        while (ncap < nbuckets) ncap <<= 1;                                                                  \
    } else {                                                                                                 \
        ncap = DS_HTABLE_MAX_SIZE;                                                                           \
    }                                                                                                        \
                                                                                                             \
    if (!(new = calloc(ncap, sizeof(struct EntryType *)))) return 0;                                         \
    for (i = 0; i < this->cap; ++i) {                                                                        \
        for (e = this->buckets[i]; e; e = next) {                                                            \
            unsigned index = murmurhash(addrOfKey(entry_get_key(e)),                                         \
                                        (int) sizeOfKey(entry_get_key(e)),                                   \
                                        this->seed) % ncap;                                                  \
            next = e->next;                                                                                  \
            e->next = new[index];                                                                            \
            new[index] = e;                                                                                  \
        }                                                                                                    \
    }                                                                                                        \
                                                                                                             \
    free(this->buckets);                                                                                     \
    this->buckets = new;                                                                                     \
    this->cap = ncap;                                                                                        \
    this->threshold = (ncap * this->lf) / 100;                                                               \
    return 1;                                                                                                \
}                                                                                                            \
                                                                                                             \
DataType *__htable_insert_##id(TableType *this,                                                              \
                               const DataType data, int *inserted) {                                         \
    unsigned index;                                                                                          \
    struct EntryType *e;                                                                                     \
    if (this->size >= this->threshold) {                                                                     \
        __htable_rehash_##id(this, this->cap + 1);                                                           \
    }                                                                                                        \
    if (this->size == DS_HTABLE_MAX_SIZE) return NULL;                                                       \
                                                                                                             \
    /* get index and entry at that index */                                                                  \
    index = murmurhash(addrOfKey(data_get_key(data)),                                                        \
                       (int) sizeOfKey(data_get_key(data)),                                                  \
                       this->seed) % this->cap;                                                              \
    for (e = this->buckets[index]; e; e = e->next) {                                                         \
        if (cmp_eq(entry_get_key(e), data_get_key(data))) break;                                             \
    }                                                                                                        \
                                                                                                             \
    if (e) {                                                                                                 \
        deleteValue(e->data.second);                                                                         \
        copyValue(e->data.second, data.second);                                                              \
        if (inserted) *inserted = 0;                                                                         \
    } else {                                                                                                 \
        if (!(e = calloc(1, sizeof(struct EntryType)))) return NULL;                                         \
        copyKey(entry_get_key(e), data_get_key(data));                                                       \
        copyValue(e->data.second, data.second);                                                              \
        e->next = this->buckets[index];                                                                      \
        this->buckets[index] = e;                                                                            \
        this->size++;                                                                                        \
        if (inserted) *inserted = 1;                                                                         \
    }                                                                                                        \
    return &e->data;                                                                                         \
}                                                                                                            \
                                                                                                             \
unsigned char __htable_insert_fromArray_##id(TableType *this,                                                \
                                             DataType const *arr, unsigned n) {                              \
    unsigned i;                                                                                              \
    if (arr) {                                                                                               \
        for (i = 0; i < n; ++i) {                                                                            \
            if (!__htable_insert_##id(this, arr[i], NULL)) return 0;                                         \
        }                                                                                                    \
    }                                                                                                        \
    return 1;                                                                                                \
}                                                                                                            \
                                                                                                             \
TableType *__htable_new_fromArray_##id(DataType const *arr, unsigned n) {                                    \
    TableType *ht = calloc(1, sizeof(TableType));                                                            \
    if (!ht) return NULL;                                                                                    \
    else if (!(ht->buckets = calloc(32, sizeof(struct EntryType *)))) {                                      \
        free(ht);                                                                                            \
        return NULL;                                                                                         \
    }                                                                                                        \
    srand((unsigned) time(NULL));                                                                            \
    ht->cap = 32;                                                                                            \
    ht->lf = 75;                                                                                             \
    ht->threshold = 24;                                                                                      \
    ht->seed = ((unsigned) rand()) % UINT_MAX;                                                               \
    __htable_insert_fromArray_##id(ht, arr, n);                                                              \
    return ht;                                                                                               \
}                                                                                                            \
                                                                                                             \
TableType *__htable_createCopy_##id(TableType const *other) {                                                \
    unsigned i;                                                                                              \
    struct EntryType *e, *new;                                                                               \
    TableType *ht = malloc(sizeof(TableType));                                                               \
    if (!ht) return NULL;                                                                                    \
    else if (!(ht->buckets = calloc(other->cap, sizeof(struct EntryType *)))) {                              \
        free(ht);                                                                                            \
        return NULL;                                                                                         \
    }                                                                                                        \
    ht->size = other->size;                                                                                  \
    ht->cap = other->cap;                                                                                    \
    ht->seed = other->seed;                                                                                  \
    ht->lf = other->lf;                                                                                      \
    ht->threshold = other->threshold;                                                                        \
    for (i = 0; i < other->cap; ++i) {                                                                       \
        for (e = other->buckets[i]; e; e = e->next) {                                                        \
            if (!(new = calloc(1, sizeof(struct EntryType)))) {                                              \
                __htable_clear_##id(ht);                                                                     \
                free(ht->buckets);                                                                           \
                free(ht);                                                                                    \
                return NULL;                                                                                 \
            }                                                                                                \
            copyKey(entry_get_key(new), entry_get_key(e));                                                   \
            copyValue(new->data.second, e->data.second);                                                     \
            new->next = ht->buckets[i];                                                                      \
            ht->buckets[i] = new;                                                                            \
        }                                                                                                    \
    }                                                                                                        \
    return ht;                                                                                               \
}                                                                                                            \
                                                                                                             \
unsigned char __htable_erase_##id(TableType *this, const kt key) {                                           \
    struct EntryType *prev, *curr;                                                                           \
    unsigned index = murmurhash(addrOfKey(key), (int) sizeOfKey(key),                                        \
                                this->seed) % this->cap;                                                     \
    if (!this->buckets[index]) return 0; /* this entry does not exist */                                     \
                                                                                                             \
    prev = this->buckets[index];                                                                             \
    curr = prev->next;                                                                                       \
    if (cmp_eq(entry_get_key(prev), key)) { /* special case when it's the first bucket entry */              \
        this->buckets[index] = curr;                                                                         \
        deleteKey(entry_get_key(prev));                                                                      \
        deleteValue(prev->data.second);                                                                      \
        free(prev);                                                                                          \
        this->size--;                                                                                        \
        return 1;                                                                                            \
    }                                                                                                        \
                                                                                                             \
    for (; curr; prev = curr, curr = curr->next) {                                                           \
        if (cmp_eq(entry_get_key(curr), key)) {                                                              \
            prev->next = curr->next;                                                                         \
            deleteKey(entry_get_key(curr));                                                                  \
            deleteValue(curr->data.second);                                                                  \
            free(curr);                                                                                      \
            this->size--;                                                                                    \
            return 1;                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
    return 0;                                                                                                \
}                                                                                                            \
                                                                                                             \
void __htable_clear_##id(TableType *this) {                                                                  \
    unsigned i;                                                                                              \
    struct EntryType *e, *next;                                                                              \
    for (i = 0; i < this->cap; ++i) { /* iterate over all buckets */                                         \
        for (e = this->buckets[i]; e; e = next) {                                                            \
            next = e->next;                                                                                  \
            deleteKey(entry_get_key(e));                                                                     \
            deleteValue(e->data.second);                                                                     \
            free(e);                                                                                         \
        }                                                                                                    \
    }                                                                                                        \
    memset(this->buckets, 0, sizeof(struct EntryType *) * this->cap);                                        \
    this->size = 0;                                                                                          \
}                                                                                                            \
                                                                                                             \
DataType *__htable_find_##id(TableType const *this, const kt key) {                                          \
    unsigned index = murmurhash(addrOfKey(key), (int) sizeOfKey(key),                                        \
                                this->seed) % this->cap;                                                     \
    struct EntryType *e;                                                                                     \
    for (e = this->buckets[index]; e; e = e->next) {                                                         \
        if (cmp_eq(entry_get_key(e), key)) return &e->data;                                                  \
    }                                                                                                        \
    return NULL;                                                                                             \
}                                                                                                            \
                                                                                                             \
unsigned char __htable_set_load_factor_##id(TableType *this, unsigned lf) {                                  \
    if (lf > 24 && lf < 101) {                                                                               \
        this->lf = lf;                                                                                       \
        lf = (this->size * 100) / this->cap;                                                                 \
        if (lf >= this->lf) {                                                                                \
            return __htable_rehash_##id(this, (this->cap + 1) << 2);                                         \
        }                                                                                                    \
        this->threshold = (this->cap * this->lf) / 100;                                                      \
    }                                                                                                        \
    return 1;                                                                                                \
}                                                                                                            \

#endif /* DS_HASH_TABLE_H */
