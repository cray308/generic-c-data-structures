#ifndef MAP_H
#define MAP_H

#include "ds.h"
#include "iterator.h"
#include "pair.h"
#include <time.h>
#include <limits.h>
#include <stdint.h>

#define __MAP_FNV_PRIME 0x01000193UL
#define __MAP_FNV_OFFSET_BASIS 0x811c9dc5UL
#define __MAP_DEFAULT_LOAD_FACTOR 0.750
#define __MAP_INITIAL_NBUCKETS 32

inline uint32_t __rotl(uint32_t x, int8_t r) {
    return (x << r) | (x >> (32 - r));
}

inline uint32_t __getblock(const uint32_t *p, int i) {
    return p[i];
}

inline uint32_t __fmix (uint32_t h) {
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}

uint32_t __murmurhash(const void *key, int len, uint32_t seed) {
    const uint8_t *data = (const uint8_t *) key;
    const int nblocks = len / 4;

    uint32_t h1 = seed;

    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    const uint32_t *blocks = (const uint32_t *) (data + (nblocks * 4));

    for (int i = -nblocks; i; i++) {
        uint32_t k1 = __getblock(blocks, i);

        k1 *= c1;
        k1 = __rotl(k1, 15);
        k1 *= c2;
        
        h1 ^= k1;
        h1 = __rotl(h1, 13); 
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
        k1 *= c1;
        k1 = __rotl(k1,15);
        k1 *= c2;
        h1 ^= k1;
    };

    h1 ^= len;
    h1 = __fmix(h1);
    return h1;
} 

#define __umap_create_hash_fn_VALUE(id, kt, t)                                                               \
__DS_FUNC_PREFIX_INL uint32_t __umap_hash_##id(const uint32_t seed, kt k) {                                  \
    return __murmurhash(&k, sizeof(k), seed);                                                                \
}                                                                                                            \

#define __umap_create_hash_fn_PTR(id, kt, t)                                                                 \
__DS_FUNC_PREFIX_INL uint32_t __umap_hash_##id(const uint32_t seed, const kt k) {                            \
    return __murmurhash(k, strlen(k), seed);                                                                 \
}                                                                                                            \

#define __umap_create_hash_fn_PTR_COPY(id, kt, t) __umap_create_hash_fn_PTR(id, kt, t)


/**
 * The number of entries in the map.
 *
 * @param   m  Pointer to map.
 */
#define umap_size(m) ((int) (m)->size)


/**
 * The current value of the map's max load factor.
 *
 * @param   m  Pointer to map.
 */
#define umap_max_load_factor(m) ((m)->lf)


/**
 * Whether the map is empty.
 *
 * @param   m  Pointer to map.
 */
#define umap_empty(m) (!((m)->size))


/**
 * Similar to umap_find, but returns a pointer to the pair's value rather than to the pair as a whole.
 *
 * @param  id  ID used with gen_umap.
 * @param  m   Pointer to map.
 * @param  k   Key to find.
 */
#define umap_at(id, m, k) umap_at_##id(m, k)


/**
 * Iterates through all entries in the map.
 *
 * @param  id  ID used with gen_umap.
 * @param  m   Pointer to map.
 * @param  it  UMapIterator (pair *) which is assigned to the current element.
 */
#define umap_iter(id, m, it)                                                                                 \
    for (it = iter_begin(UMAP, id, m, 0); it != iter_end(UMAP, id, m, 0);  __iter_next_UMAP(id, m, it))


/**
 * Inserts the provided key and value into the map. If the key already exists, it will be updated
 *   with the provided value.
 *
 * @param  id  ID used with gen_umap.
 * @param  m  Pointer to map.
 * @param  k  Key to be used.
 * @param  v  Value to be inserted.
 */
#define umap_insert(id, m, key, value) umap_insert_##id(m, key, value)


/**
 * Finds the provided key.
 *
 * @param  id  ID used with gen_umap.
 * @param  m   Pointer to map.
 * @param  k   Key to be used.
 *
 * @return     If the key is found, returns a pointer to the pair (key, value) corresponding to
 *               that key. If the key does not exist, returns NULL.
 */
#define umap_find(id, m, key) umap_find_##id(m, key)


/**
 * Removes the key and its corresponding value from the map, if it exists.
 *
 * @param  id  ID used with gen_umap.
 * @param  m   Pointer to map.
 * @param  k   Key to be used.
 */
#define umap_erase_key(id, m, key) umap_erase_##id(m, key)


/**
 * Creates a new unordered map.
 *
 * @param  id  ID used with gen_umap.
 *
 * @return     Pointer to newly created map.
 */
#define umap_new(id)  umap_new_##id() 


/**
 * Deletes all elements and frees the map.
 *
 * @param  id  ID used with gen_umap.
 * @param  m   Pointer to map.
 */
#define umap_free(id, m) umap_free_##id(m) 


/**
 * Modifies the number of buckets in the map. If this is less than or equal to the current number
 *   of buckets, nothing is done.
 *
 * @param  id        ID used with gen_umap.
 * @param  m         Pointer to map.
 * @param  nbuckets  New number of buckets to use in the map.
 */
#define umap_rehash(id, m, nbuckets) umap_rehash_##id(m, nbuckets)


/**
 * If the provided load factor is sensible, sets the maximum load factor to the provided
 *   value, and may rehash the map if required.
 *
 * @param  id  ID used with gen_umap.
 * @param  m   Pointer to map.
 * @param  lf  The new load factor to use.
 */
#define umap_set_load_factor(id, m, lf) umap_set_load_factor_##id(m, lf) 


/**
 * Removes all entries from the map.
 *
 * @param  id  ID used with gen_umap.
 * @param  m   Pointer to map.
 */
#define umap_clear(id, m) umap_clear_##id(m)


/**
 * Generates unordered map (hash table) code for a specified key type, value type and ID.
 *
 * @param   id        ID to be used for the map (must be unique).
 * @param   kt        Key type to be used.
 * @param   key_decl  One of VALUE, PTR, or PTR_COPY.
 *                     - use VALUE if the key type is not a pointer (i.e. int, double, char, etc)
 *                     - use PTR if the key type is a pointer (i.e. char *) and the pointed-to value does not need to be copied.
 *                     - use PTR_COPY if the key type is a pointer and the pointed-to value *should* be copied.
 *                     - Note that PTR_COPY uses strcpy iternally, so using other types of pointers will cause undefined behavior.
 * @param   t         Value type to be stored in the map.
 * @param   cmp_lt    Macro of the form (x, y) that returns whether x is strictly less than y.
 */
#define gen_umap(id, kt, key_decl, t, cmp_lt)                                                                \
__umap_create_hash_fn_##key_decl(id, kt, t)                                                                  \
__gen_umap_entry(id, kt, t)                                                                                  \
__gen_umap_common(id)                                                                                        \
__gen_iter_UMAP(id)                                                                                          \
__gen_umap_declarations(id, kt, t)                                                                           \
__gen_umap_new_free(id)                                                                                      \
__gen_umap_common_fns(id, kt, t, cmp_lt)                                                                     \
__gen_umap_##key_decl(id, kt, t, cmp_lt)                                                                     \
__gen_umap_set_lf(id)                                                                                        \

#define __gen_umap_entry(id, kt, t)                                                                          \
gen_pair(id, kt, t)                                                                                          \
typedef struct UMapEntry_##id UMapEntry_##id;                                                                \
struct UMapEntry_##id {                                                                                      \
    UMapEntry_##id *next;                                                                                    \
    Pair_##id p;                                                                                             \
};                                                                                                           \
UMapEntry_##id *umap_entry_new_##id(void) {                                                                  \
    UMapEntry_##id *e = malloc(sizeof(UMapEntry_##id));                                                      \
    if (!e) {                                                                                                \
        DS_OOM();                                                                                            \
    }                                                                                                        \
    memset(e, 0, sizeof(UMapEntry_##id));                                                                    \
    return e;                                                                                                \
}                                                                                                            \

#define __gen_umap_common(id)                                                                                \
typedef struct {                                                                                             \
    UMapEntry_##id *curr;                                                                                    \
    unsigned idx;                                                                                            \
} __UMap_Curr_##id;                                                                                          \
                                                                                                             \
typedef struct {                                                                                             \
    size_t size;                                                                                             \
    size_t cap;                                                                                              \
    const uint32_t __seed;                                                                                   \
    double lf;                                                                                               \
    __UMap_Curr_##id it;                                                                                     \
    UMapEntry_##id **buckets;                                                                                \
} UMap_##id;                                                                                                 \

#define __gen_umap_declarations(id, kt, t)                                                                   \
__DS_FUNC_PREFIX void umap_clear_##id(UMap_##id *m);                                                         \
__DS_FUNC_PREFIX void umap_rehash_##id(UMap_##id *m, size_t nbuckets);                                       \
__DS_FUNC_PREFIX UMapEntry_##id *__find_index_##id(UMap_##id *m, const kt key, uint32_t index);              \
__DS_FUNC_PREFIX Pair_##id *umap_find_##id(UMap_##id *m, const kt key);                                      \
__DS_FUNC_PREFIX void umap_insert_##id(UMap_##id *m, kt key, t value);                                       \
__DS_FUNC_PREFIX void umap_erase_##id(UMap_##id *m, const kt key);                                           \

#define __gen_umap_new_free(id)                                                                              \
__DS_FUNC_PREFIX UMap_##id *umap_new_##id(void) {                                                            \
    UMap_##id *m = malloc(sizeof(UMap_##id));                                                                \
    if (!m) DS_OOM();                                                                                        \
                                                                                                             \
    srand(time(NULL));                                                                                       \
    uint32_t seed = rand() % UINT32_MAX;                                                                     \
    UMap_##id temp = {                                                                                       \
        .size = 0,                                                                                           \
        .cap = __MAP_INITIAL_NBUCKETS,                                                                       \
        .__seed = seed,                                                                                      \
        .lf = __MAP_DEFAULT_LOAD_FACTOR,                                                                     \
        .it = {NULL, 0},                                                                                     \
        .buckets = NULL                                                                                      \
    };                                                                                                       \
    memcpy(m, &temp, sizeof(UMap_##id));                                                                     \
                                                                                                             \
    m->buckets = malloc(m->cap * sizeof(UMapEntry_##id *));                                                  \
    if (!m->buckets) DS_OOM();                                                                               \
                                                                                                             \
    memset(m->buckets, 0, m->cap * sizeof(UMapEntry_##id *));                                                \
    return m;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void umap_free_##id(UMap_##id *m) {                                                     \
    umap_clear_##id(m);                                                                                      \
    free(m->buckets);                                                                                        \
    free(m);                                                                                                 \
}                                                                                                            \

#define __gen_umap_set_lf(id)                                                                                \
__DS_FUNC_PREFIX void umap_set_load_factor_##id(UMap_##id *m, double lf) {                                   \
    if (lf < 0.500 || lf > 1.000) return;                                                                    \
    m->lf = lf;                                                                                              \
    double curr_lf = (double) m->size / m->cap;                                                              \
    if (curr_lf > m->lf) {                                                                                   \
        umap_rehash_##id(m, m->cap << 1);                                                                    \
    }                                                                                                        \
}                                                                                                            \

#define __gen_umap_common_fns(id, kt, t, cmp_lt)                                                             \
__DS_FUNC_PREFIX_INL t *umap_at_##id(UMap_##id *m, const kt key) {                                           \
    Pair_##id *p = umap_find_##id(m, key);                                                                   \
    return p ? &(p->second) : NULL;                                                                          \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL Pair_##id *umap_find_##id(UMap_##id *m, const kt key) {                                 \
    uint32_t index = __umap_hash_##id(m->__seed, key) % m->cap;                                              \
    UMapEntry_##id *e = __find_index_##id(m, key, index);                                                    \
    return e ? &(e->p) : NULL;                                                                               \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL UMapEntry_##id *__find_index_##id(UMap_##id *m, const kt key, uint32_t index) {         \
    UMapEntry_##id *e = m->buckets[index];                                                                   \
    while (e) {                                                                                              \
        if (ds_cmp_eq(cmp_lt, e->p.first, key)) return e;                                                    \
        e = e->next;                                                                                         \
    }                                                                                                        \
    return NULL;                                                                                             \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void umap_rehash_##id(UMap_##id *m, size_t nbuckets) {                                      \
    if (nbuckets <= m->cap) return;                                                                          \
                                                                                                             \
    size_t n = m->cap; /* for consistency, use a multiple of 2 of the capacity */                            \
    while (n < nbuckets) {                                                                                   \
        n <<= 1;                                                                                             \
    }                                                                                                        \
                                                                                                             \
    UMapEntry_##id **new = malloc(n * sizeof(UMapEntry_##id *));                                             \
    if (!new) DS_OOM();                                                                                      \
                                                                                                             \
    memset(new, 0, n * sizeof(UMapEntry_##id *));                                                            \
    UMapEntry_##id *e = NULL, *temp = NULL;                                                                  \
    uint32_t index = 0;                                                                                      \
                                                                                                             \
    for (size_t i = 0; i < m->cap; ++i) {                                                                    \
        e = m->buckets[i];                                                                                   \
                                                                                                             \
        while (e) {                                                                                          \
            temp = e->next;                                                                                  \
            index = __umap_hash_##id(m->__seed, e->p.first) % n;                                             \
            e->next = new[index];                                                                            \
            new[index] = e;                                                                                  \
            e = temp;                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
                                                                                                             \
    free(m->buckets);                                                                                        \
    m->buckets = new;                                                                                        \
    m->cap = n;                                                                                              \
}                                                                                                            \

#define __gen_umap_PTR_COPY(id, kt, t, cmp_lt)                                                               \
__DS_FUNC_PREFIX void umap_insert_##id(UMap_##id *m, kt key, t value) {                                      \
    if (!key) return;                                                                                        \
                                                                                                             \
    double lf = (double) m->size / m->cap;                                                                   \
    if (lf > m->lf) {                                                                                        \
        umap_rehash_##id(m, m->cap << 1);                                                                    \
    }                                                                                                        \
                                                                                                             \
    /* get index and entry at that index */                                                                  \
    uint32_t index = __umap_hash_##id(m->__seed, key) % m->cap;                                              \
    UMapEntry_##id *e = __find_index_##id(m, key, index);                                                    \
                                                                                                             \
    if (e) {                                                                                                 \
        e->p.second = value;                                                                                 \
    } else {                                                                                                 \
        e = umap_entry_new_##id();                                                                           \
        e->p.first = malloc(strlen(key) + 1);                                                                \
        if (!(e->p.first)) {                                                                                 \
            DS_OOM();                                                                                        \
        }                                                                                                    \
                                                                                                             \
        strcpy(e->p.first, key);                                                                             \
        e->next = m->buckets[index];                                                                         \
        m->buckets[index] = e;                                                                               \
        m->size++;                                                                                           \
        e->p.second = value;                                                                                 \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void umap_erase_##id(UMap_##id *m, const kt key) {                                          \
    if (!key) return;                                                                                        \
    uint32_t index = __umap_hash_##id(m->__seed, key) % m->cap;                                              \
    if (!(m->buckets[index])) return; /* this entry does not exist */                                        \
                                                                                                             \
    UMapEntry_##id *prev = m->buckets[index];                                                                \
    UMapEntry_##id *curr = prev->next;                                                                       \
    if (ds_cmp_eq(cmp_lt, prev->p.first, key)) { /* special case when it's the first bucket entry */         \
        m->buckets[index] = curr;                                                                            \
        free(prev->p.first);                                                                                 \
        free(prev);                                                                                          \
        m->size--;                                                                                           \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    while (curr) {                                                                                           \
        if (ds_cmp_eq(cmp_lt, curr->p.first, key)) {                                                         \
            prev->next = curr->next;                                                                         \
            free(curr->p.first);                                                                             \
            free(curr);                                                                                      \
            m->size--;                                                                                       \
            return;                                                                                          \
        }                                                                                                    \
        prev = curr;                                                                                         \
        curr = curr->next;                                                                                   \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void umap_clear_##id(UMap_##id *m) {                                                        \
    UMapEntry_##id *e = NULL, *temp = NULL;                                                                  \
    for (size_t i = 0; i < m->cap; ++i) { /* iterate over all buckets */                                     \
        e = m->buckets[i];                                                                                   \
                                                                                                             \
        while (e) { /* iterate through linked list */                                                        \
            temp = e->next;                                                                                  \
            if (e->p.first) {                                                                                \
                free(e->p.first);                                                                            \
            }                                                                                                \
            free(e);                                                                                         \
            e = temp;                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
    memset(m->buckets, 0, sizeof(UMapEntry_##id *) * m->cap);                                                \
    m->size = 0;                                                                                             \
}                                                                                                            \

#define __gen_umap_VALUE(id, kt, t, cmp_lt)                                                                  \
__DS_FUNC_PREFIX void umap_insert_##id(UMap_##id *m, kt key, t value) {                                      \
    double lf = (double) m->size / m->cap;                                                                   \
    if (lf > m->lf) {                                                                                        \
        umap_rehash_##id(m, m->cap << 1);                                                                    \
    }                                                                                                        \
                                                                                                             \
    /* get index and entry at that index */                                                                  \
    uint32_t index = __umap_hash_##id(m->__seed, key) % m->cap;                                              \
    UMapEntry_##id *e = __find_index_##id(m, key, index);                                                    \
                                                                                                             \
    if (e) {                                                                                                 \
        e->p.second = value;                                                                                 \
    } else {                                                                                                 \
        e = umap_entry_new_##id();                                                                           \
        e->p.first = key;                                                                                    \
        e->next = m->buckets[index];                                                                         \
        m->buckets[index] = e;                                                                               \
        m->size++;                                                                                           \
        e->p.second = value;                                                                                 \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void umap_erase_##id(UMap_##id *m, const kt key) {                                          \
    uint32_t index = __umap_hash_##id(m->__seed, key) % m->cap;                                              \
    if (!(m->buckets[index])) return; /* this entry does not exist */                                        \
                                                                                                             \
    UMapEntry_##id *prev = m->buckets[index];                                                                \
    UMapEntry_##id *curr = prev->next;                                                                       \
    if (ds_cmp_eq(cmp_lt, prev->p.first, key)) { /* special case when it's the first bucket entry */         \
        m->buckets[index] = curr;                                                                            \
        free(prev);                                                                                          \
        m->size--;                                                                                           \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    while (curr) {                                                                                           \
        if (ds_cmp_eq(cmp_lt, curr->p.first, key)) {                                                         \
            prev->next = curr->next;                                                                         \
            free(curr);                                                                                      \
            m->size--;                                                                                       \
            return;                                                                                          \
        }                                                                                                    \
        prev = curr;                                                                                         \
        curr = curr->next;                                                                                   \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void umap_clear_##id(UMap_##id *m) {                                                        \
    UMapEntry_##id *e = NULL, *temp = NULL;                                                                  \
    for (size_t i = 0; i < m->cap; ++i) { /* iterate over all buckets */                                     \
        e = m->buckets[i];                                                                                   \
                                                                                                             \
        while (e) { /* iterate through linked list */                                                        \
            temp = e->next;                                                                                  \
            free(e);                                                                                         \
            e = temp;                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
    memset(m->buckets, 0, sizeof(UMapEntry_##id *) * m->cap);                                                \
    m->size = 0;                                                                                             \
}                                                                                                            \

#define __gen_umap_PTR(id, kt, t, cmp_lt)                                                                    \
__DS_FUNC_PREFIX void umap_insert_##id(UMap_##id *m, kt key, t value) {                                      \
    if (!key) return;                                                                                        \
                                                                                                             \
    double lf = (double) m->size / m->cap;                                                                   \
    if (lf > m->lf) {                                                                                        \
        umap_rehash_##id(m, m->cap << 1);                                                                    \
    }                                                                                                        \
                                                                                                             \
    /* get index and entry at that index */                                                                  \
    uint32_t index = __umap_hash_##id(m->__seed, key) % m->cap;                                              \
    UMapEntry_##id *e = __find_index_##id(m, key, index);                                                    \
                                                                                                             \
    if (e) {                                                                                                 \
        e->p.second = value;                                                                                 \
    } else {                                                                                                 \
        e = umap_entry_new_##id();                                                                           \
        e->p.first = key;                                                                                    \
        e->next = m->buckets[index];                                                                         \
        m->buckets[index] = e;                                                                               \
        m->size++;                                                                                           \
        e->p.second = value;                                                                                 \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void umap_erase_##id(UMap_##id *m, const kt key) {                                          \
    if (!key) return;                                                                                        \
    uint32_t index = __umap_hash_##id(m->__seed, key) % m->cap;                                              \
    if (!(m->buckets[index])) return; /* this entry does not exist */                                        \
                                                                                                             \
    UMapEntry_##id *prev = m->buckets[index];                                                                \
    UMapEntry_##id *curr = prev->next;                                                                       \
    if (ds_cmp_eq(cmp_lt, prev->p.first, key)) { /* special case when it's the first bucket entry */         \
        m->buckets[index] = curr;                                                                            \
        free(prev);                                                                                          \
        m->size--;                                                                                           \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    while (curr) {                                                                                           \
        if (ds_cmp_eq(cmp_lt, curr->p.first, key)) {                                                         \
            prev->next = curr->next;                                                                         \
            free(curr);                                                                                      \
            m->size--;                                                                                       \
            return;                                                                                          \
        }                                                                                                    \
        prev = curr;                                                                                         \
        curr = curr->next;                                                                                   \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void umap_clear_##id(UMap_##id *m) {                                                        \
    UMapEntry_##id *e = NULL, *temp = NULL;                                                                  \
    for (size_t i = 0; i < m->cap; ++i) { /* iterate over all buckets */                                     \
        e = m->buckets[i];                                                                                   \
                                                                                                             \
        while (e) { /* iterate through linked list */                                                        \
            temp = e->next;                                                                                  \
            free(e);                                                                                         \
            e = temp;                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
    memset(m->buckets, 0, sizeof(UMapEntry_##id *) * m->cap);                                                \
    m->size = 0;                                                                                             \
}                                                                                                            \

#endif /* MAP_H */
