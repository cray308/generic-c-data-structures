#ifndef DS_HASH_TABLE_H
#define DS_HASH_TABLE_H

#include "ds.h"
#include "hash.h"

#if UINT_MAX == 0xffffffff
#define DS_HTABLE_MAX_SIZE 42949672
#define DS_HTABLE_SHIFT_THRESHOLD 21474836
#elif UINT_MAX == 0xffff
#define DS_HTABLE_MAX_SIZE 655
#define DS_HTABLE_SHIFT_THRESHOLD 327
#endif

#define __setup_hash_table_headers(id, kt, TableType, DataType, EntryType)               \
                                                                                         \
struct EntryType {                                                                       \
    struct EntryType *next;                                                              \
    DataType data;                                                                       \
};                                                                                       \
                                                                                         \
typedef struct {                                                                         \
    unsigned size;                                                                       \
    unsigned cap;                                                                        \
    unsigned threshold;                                                                  \
    unsigned lf;                                                                         \
    unsigned seed;                                                                       \
    struct {                                                                             \
        struct EntryType *curr;                                                          \
        unsigned idx;                                                                    \
    } it;                                                                                \
    struct EntryType **buckets;                                                          \
} TableType;                                                                             \
                                                                                         \
DataType* __htable_iter_begin_##id(TableType *this) __attribute__((nonnull));            \
DataType* __htable_iter_next_##id(TableType *this) __attribute__((nonnull));             \
                                                                                         \
unsigned char __htable_rehash_##id(TableType *this, unsigned nbuckets)                   \
  __attribute__((nonnull));                                                              \
DataType* __htable_insert_##id(TableType *this,                                          \
                               DataType const data, int *inserted)                       \
  __attribute__((nonnull (1)));                                                          \
unsigned char __htable_insert_fromArray_##id(TableType *this,                            \
                                             DataType const *arr, unsigned n)            \
  __attribute__((nonnull));                                                              \
TableType *__htable_new_fromArray_##id(DataType const *arr, unsigned n);                 \
TableType *__htable_createCopy_##id(TableType const *other)                              \
  __attribute__((nonnull));                                                              \
unsigned char __htable_erase_##id(TableType *this, kt const key)                         \
  __attribute__((nonnull));                                                              \
void __htable_clear_##id(TableType *this)                                                \
  __attribute__((nonnull));                                                              \
DataType* __htable_find_##id(TableType const *this, kt const key)                        \
  __attribute__((nonnull));                                                              \
unsigned char __htable_set_load_factor_##id(TableType *this, unsigned lf)                \
  __attribute__((nonnull));                                                              \

#define __setup_hash_table_source(id, kt, cmp_eq, TableType, DataType,                   \
                                  EntryType, entry_get_key, data_get_key,                \
                                  addrOfKey, sizeOfKey, copyKey, deleteKey,              \
                                  copyValue, deleteValue)                                \
                                                                                         \
DataType* __htable_iter_begin_##id(TableType *this) {                                    \
    if (!this->size) {                                                                   \
        this->it.curr = NULL;                                                            \
        this->it.idx = this->cap;                                                        \
    } else {                                                                             \
        unsigned idx;                                                                    \
        for (idx = 0; idx < this->cap && this->buckets[idx] == NULL; ++idx);             \
        this->it.idx = idx;                                                              \
        this->it.curr = this->buckets[this->it.idx];                                     \
    }                                                                                    \
    return this->it.curr ? &this->it.curr->data : NULL;                                  \
}                                                                                        \
                                                                                         \
DataType* __htable_iter_next_##id(TableType *this) {                                     \
    if (this->it.curr->next) {                                                           \
        this->it.curr = this->it.curr->next;                                             \
    } else {                                                                             \
        unsigned idx = this->it.idx + 1;                                                 \
        for (; idx < this->cap && this->buckets[idx] == NULL; ++idx);                    \
        this->it.idx = idx;                                                              \
        this->it.curr = (this->it.idx >= this->cap) ?                                    \
            NULL : this->buckets[this->it.idx];                                          \
    }                                                                                    \
    return this->it.curr ? &this->it.curr->data : NULL;                                  \
}                                                                                        \
                                                                                         \
static struct EntryType *__htable_find_entry_##id(TableType const *this,                 \
                                                  unsigned *index,                       \
                                                  kt const key) {                        \
    /* get index and entry at this index */                                              \
    struct EntryType *e;                                                                 \
    *index = murmurhash(addrOfKey(key),                                                  \
                        (int) sizeOfKey(key), this->seed) % this->cap;                   \
    for (e = this->buckets[*index]; e; e = e->next) {                                    \
        if (cmp_eq(entry_get_key(e), key)) break;                                        \
    }                                                                                    \
    return e;                                                                            \
}                                                                                        \
                                                                                         \
static DataType* __htable_insert_nocheck_##id(TableType *this,                           \
                                              DataType const data,                       \
                                              int *inserted) {                           \
    unsigned index;                                                                      \
    struct EntryType *e = __htable_find_entry_##id(this, &index,                         \
                                                   data_get_key(data));                  \
                                                                                         \
    if (e) {                                                                             \
        deleteValue(e->data.second);                                                     \
        copyValue(e->data.second, data.second);                                          \
        if (inserted) *inserted = 0;                                                     \
    } else {                                                                             \
        if (this->size == DS_HTABLE_MAX_SIZE ||                                          \
                !(e = calloc(1, sizeof(struct EntryType)))) return NULL;                 \
        copyKey(entry_get_key(e), data_get_key(data));                                   \
        copyValue(e->data.second, data.second);                                          \
        e->next = this->buckets[index];                                                  \
        this->buckets[index] = e;                                                        \
        this->size++;                                                                    \
        if (inserted) *inserted = 1;                                                     \
    }                                                                                    \
    return &e->data;                                                                     \
}                                                                                        \
                                                                                         \
unsigned char __htable_rehash_##id(TableType *this, unsigned nbuckets) {                 \
    unsigned ncap = this->cap, i;                                                        \
    struct EntryType **new, *e, *next;                                                   \
    if (nbuckets <= ncap) return 1;                                                      \
                                                                                         \
    if (nbuckets < DS_HTABLE_SHIFT_THRESHOLD) {                                          \
        while (ncap < nbuckets) ncap <<= 1;                                              \
    } else {                                                                             \
        if (nbuckets > DS_HTABLE_MAX_SIZE) return 0;                                     \
        ncap = DS_HTABLE_MAX_SIZE;                                                       \
    }                                                                                    \
                                                                                         \
    if (!(new = calloc(ncap, sizeof(struct EntryType *)))) return 0;                     \
    for (i = 0; i < this->cap; ++i) {                                                    \
        for (e = this->buckets[i]; e; e = next) {                                        \
            unsigned index = murmurhash(addrOfKey(entry_get_key(e)),                     \
                                        (int) sizeOfKey(entry_get_key(e)),               \
                                        this->seed) % ncap;                              \
            next = e->next;                                                              \
            e->next = new[index];                                                        \
            new[index] = e;                                                              \
        }                                                                                \
    }                                                                                    \
                                                                                         \
    free(this->buckets);                                                                 \
    this->buckets = new;                                                                 \
    this->cap = ncap;                                                                    \
    this->threshold = (ncap * this->lf) / 100;                                           \
    return 1;                                                                            \
}                                                                                        \
                                                                                         \
DataType* __htable_insert_##id(TableType *this,                                          \
                               DataType const data, int *inserted) {                     \
    if (this->size >= this->threshold) {                                                 \
        __htable_rehash_##id(this, this->cap + 1);                                       \
    }                                                                                    \
    return __htable_insert_nocheck_##id(this, data, inserted);                           \
}                                                                                        \
                                                                                         \
unsigned char __htable_insert_fromArray_##id(TableType *this,                            \
                                             DataType const *arr, unsigned n) {          \
    unsigned i, newSize = this->size + n;                                                \
    if (newSize >= this->threshold || newSize < this->size) {                            \
        unsigned newCap = this->cap + n;                                                 \
        if (newCap < this->cap) newCap = DS_HTABLE_MAX_SIZE;                             \
        __htable_rehash_##id(this, newCap);                                              \
    }                                                                                    \
    for (i = 0; i < n; ++i) {                                                            \
        if (!__htable_insert_nocheck_##id(this, arr[i], NULL)) return 0;                 \
    }                                                                                    \
    return 1;                                                                            \
}                                                                                        \
                                                                                         \
TableType *__htable_new_fromArray_##id(DataType const *arr, unsigned n) {                \
    TableType *ht = calloc(1, sizeof(TableType));                                        \
    customAssert(ht)                                                                     \
    if (!ht) return NULL;                                                                \
    ht->buckets = calloc(32, sizeof(struct EntryType *));                                \
    customAssert(ht->buckets)                                                            \
    if (!ht->buckets) {                                                                  \
        free(ht);                                                                        \
        return NULL;                                                                     \
    }                                                                                    \
    ht->cap = 32;                                                                        \
    ht->lf = 75;                                                                         \
    ht->threshold = 24;                                                                  \
    ht->seed = ((unsigned) rand()) % UINT_MAX;                                           \
    if (arr && n) __htable_insert_fromArray_##id(ht, arr, n);                            \
    return ht;                                                                           \
}                                                                                        \
                                                                                         \
TableType *__htable_createCopy_##id(TableType const *other) {                            \
    unsigned i;                                                                          \
    struct EntryType *e;                                                                 \
    TableType *ht = __htable_new_fromArray_##id(NULL, 0);                                \
    if (ht) {                                                                            \
        __htable_rehash_##id(ht, other->cap);                                            \
        for (i = 0; i < other->cap; ++i) {                                               \
            for (e = other->buckets[i]; e; e = e->next) {                                \
                __htable_insert_nocheck_##id(ht, e->data, NULL);                         \
            }                                                                            \
        }                                                                                \
    }                                                                                    \
    return ht;                                                                           \
}                                                                                        \
                                                                                         \
unsigned char __htable_erase_##id(TableType *this, kt const key) {                       \
    struct EntryType *prev, *curr;                                                       \
    unsigned index = murmurhash(addrOfKey(key), (int) sizeOfKey(key),                    \
                                this->seed) % this->cap;                                 \
    if (!this->buckets[index]) return 0; /* this entry does not exist */                 \
                                                                                         \
    prev = this->buckets[index];                                                         \
    curr = prev->next;                                                                   \
    if (cmp_eq(entry_get_key(prev), key)) { /* special case for first bucket entry */    \
        this->buckets[index] = curr;                                                     \
        deleteKey(entry_get_key(prev));                                                  \
        deleteValue(prev->data.second);                                                  \
        free(prev);                                                                      \
        this->size--;                                                                    \
        return 1;                                                                        \
    }                                                                                    \
                                                                                         \
    for (; curr; prev = curr, curr = curr->next) {                                       \
        if (cmp_eq(entry_get_key(curr), key)) {                                          \
            prev->next = curr->next;                                                     \
            deleteKey(entry_get_key(curr));                                              \
            deleteValue(curr->data.second);                                              \
            free(curr);                                                                  \
            this->size--;                                                                \
            return 1;                                                                    \
        }                                                                                \
    }                                                                                    \
    return 0;                                                                            \
}                                                                                        \
                                                                                         \
void __htable_clear_##id(TableType *this) {                                              \
    unsigned i;                                                                          \
    struct EntryType *e, *next;                                                          \
    for (i = 0; i < this->cap; ++i) { /* iterate over all buckets */                     \
        for (e = this->buckets[i]; e; e = next) {                                        \
            next = e->next;                                                              \
            deleteKey(entry_get_key(e));                                                 \
            deleteValue(e->data.second);                                                 \
            free(e);                                                                     \
        }                                                                                \
    }                                                                                    \
    memset(this->buckets, 0, sizeof(struct EntryType *) * this->cap);                    \
    this->size = 0;                                                                      \
}                                                                                        \
                                                                                         \
DataType* __htable_find_##id(TableType const *this, kt const key) {                      \
    unsigned i;                                                                          \
    struct EntryType *e = __htable_find_entry_##id(this, &i, key);                       \
    return e ? &e->data : NULL;                                                          \
}                                                                                        \
                                                                                         \
unsigned char __htable_set_load_factor_##id(TableType *this, unsigned lf) {              \
    if (lf > 24 && lf < 101) {                                                           \
        this->lf = lf;                                                                   \
        lf = (this->size * 100) / this->cap;                                             \
        if (lf >= this->lf) {                                                            \
            return __htable_rehash_##id(this, (this->cap + 1) << 2);                     \
        }                                                                                \
        this->threshold = (this->cap * this->lf) / 100;                                  \
    }                                                                                    \
    return 1;                                                                            \
}                                                                                        \

#endif /* DS_HASH_TABLE_H */
