#ifndef MAP_H
#define MAP_H

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

#define __hash_key_VALUE(k) &(k)
#define __hash_key_PTR(k) k
#define __hash_key_PTR_COPY(k) k
#define __hash_len_VALUE(k) sizeof(k)
#define __hash_len_PTR(k) strlen(k)
#define __hash_len_PTR_COPY(k) strlen(k)

#define __map_allocate_new_key_PTR_COPY(e, key) (e)->p.first = __ds_malloc(strlen(key) + 1); strcpy((e)->p.first, key);
#define __map_allocate_new_key_VALUE(e, key) (e)->p.first = key;
#define __map_allocate_new_key_PTR(e, key) __map_allocate_new_key_VALUE(e, key)

#define __map_remove_key_PTR_COPY(e) free((e)->p.first);
#define __map_remove_key_PTR(e)
#define __map_remove_key_VALUE(e)

#define __map_check_valid_key_PTR(key) if (!(key)) return;
#define __map_check_valid_key_PTR_COPY(key) __map_check_valid_key_PTR(key)
#define __map_check_valid_key_VALUE(key)

#define __umap_find_index(MapEntry, map, cmp, result, index, key) {                                          \
    MapEntry *ptr = (map)->buckets[(index)];                                                                 \
    while (ptr) {                                                                                            \
        if (ds_cmp_eq(cmp, ptr->p.first, key)) {                                                             \
            (result) = ptr; break;                                                                           \
        }                                                                                                    \
        ptr = ptr->next;                                                                                     \
    }                                                                                                        \
}


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
 * @param  it  Pointer to pair which is assigned to the current element.
 */
#define umap_iter(id, m, it) for (it = iter_begin(UMAP, id, m, 0); it != iter_end(UMAP, id, m, 0); iter_next_UMAP(id, m, it))


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

/* --------------------------------------------------------------------------
 * Unordered map iterators
 * -------------------------------------------------------------------------- */

#define iter_begin_UMAP(id, m, n)  __umap_iter_begin_##id(m)
#define iter_end_UMAP(id, m, n)    NULL
#define iter_next_UMAP(id, m, p)   ((p) = __umap_iter_next_##id(m))
#define iter_deref_UMAP(p)         ((p)->second)

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
                                                                                                             \
gen_pair(id, kt, t)                                                                                          \
typedef struct UMapEntry_##id UMapEntry_##id;                                                                \
struct UMapEntry_##id {                                                                                      \
    UMapEntry_##id *next;                                                                                    \
    Pair_##id p;                                                                                             \
};                                                                                                           \
                                                                                                             \
typedef struct {                                                                                             \
    UMapEntry_##id *curr;                                                                                    \
    unsigned idx;                                                                                            \
} __UMap_Curr_##id;                                                                                          \
                                                                                                             \
typedef struct {                                                                                             \
    size_t size;                                                                                             \
    size_t cap;                                                                                              \
    const uint32_t seed;                                                                                     \
    double lf;                                                                                               \
    __UMap_Curr_##id it;                                                                                     \
    UMapEntry_##id **buckets;                                                                                \
} UMap_##id;                                                                                                 \
                                                                                                             \
__DS_FUNC_PREFIX_INL Pair_##id *__umap_iter_begin_##id(UMap_##id *m) {                                       \
    if (!(m->size)) {                                                                                        \
        m->it.curr = NULL;                                                                                   \
        m->it.idx = m->cap;                                                                                  \
    } else {                                                                                                 \
        unsigned idx = 0;                                                                                    \
        for (; (idx < m->cap) && (m->buckets[idx] == NULL); ++idx);                                          \
        m->it.idx = idx;                                                                                     \
        m->it.curr = m->buckets[m->it.idx];                                                                  \
    }                                                                                                        \
    return m->it.curr ? &(m->it.curr->p) : NULL;                                                             \
}                                                                                                            \
__DS_FUNC_PREFIX_INL Pair_##id *__umap_iter_next_##id(UMap_##id *m) {                                        \
    if (m->it.curr->next) {                                                                                  \
        m->it.curr = m->it.curr->next;                                                                       \
    } else {                                                                                                 \
        unsigned idx = m->it.idx + 1;                                                                        \
        for (; (idx < m->cap) && (m->buckets[idx] == NULL); ++idx);                                          \
        m->it.idx = idx;                                                                                     \
        m->it.curr = (m->it.idx >= m->cap) ? NULL : m->buckets[m->it.idx];                                   \
    }                                                                                                        \
    return m->it.curr ? &(m->it.curr->p) : NULL;                                                             \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void umap_clear_##id(UMap_##id *m);                                                         \
__DS_FUNC_PREFIX void umap_rehash_##id(UMap_##id *m, size_t nbuckets);                                       \
__DS_FUNC_PREFIX Pair_##id *umap_find_##id(UMap_##id *m, const kt key);                                      \
__DS_FUNC_PREFIX void umap_insert_##id(UMap_##id *m, kt key, t value);                                       \
__DS_FUNC_PREFIX void umap_erase_##id(UMap_##id *m, const kt key);                                           \
                                                                                                             \
__DS_FUNC_PREFIX UMap_##id *umap_new_##id(void) {                                                            \
    UMap_##id *m = __ds_malloc(sizeof(UMap_##id));                                                           \
    srand(time(NULL));                                                                                       \
    memcpy(m, &((UMap_##id){.cap = 32, .seed = rand() % UINT32_MAX, .lf = 0.75, .it = {NULL, 0}}), sizeof(UMap_##id)); \
    m->buckets = __ds_calloc(m->cap, sizeof(UMapEntry_##id *));                                              \
    return m;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void umap_free_##id(UMap_##id *m) {                                                     \
    umap_clear_##id(m);                                                                                      \
    free(m->buckets);                                                                                        \
    free(m);                                                                                                 \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL t *umap_at_##id(UMap_##id *m, const kt key) {                                           \
    Pair_##id *p = umap_find_##id(m, key);                                                                   \
    return p ? &(p->second) : NULL;                                                                          \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL Pair_##id *umap_find_##id(UMap_##id *m, const kt key) {                                 \
    uint32_t index = murmurhash(__hash_key_##key_decl(key), __hash_len_##key_decl(key), m->seed) % m->cap;   \
    UMapEntry_##id *e = NULL;                                                                                \
    __umap_find_index(UMapEntry_##id, m, cmp_lt, e, index, key)                                              \
    return e ? &(e->p) : NULL;                                                                               \
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
    UMapEntry_##id **new = __ds_calloc(n, sizeof(UMapEntry_##id *));                                         \
    UMapEntry_##id *e = NULL, *temp = NULL;                                                                  \
    uint32_t index = 0;                                                                                      \
                                                                                                             \
    for (size_t i = 0; i < m->cap; ++i) {                                                                    \
        e = m->buckets[i];                                                                                   \
                                                                                                             \
        while (e) {                                                                                          \
            temp = e->next;                                                                                  \
            index = murmurhash(__hash_key_##key_decl(e->p.first), __hash_len_##key_decl(e->p.first), m->seed) % n; \
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
                                                                                                             \
__DS_FUNC_PREFIX void umap_set_load_factor_##id(UMap_##id *m, double lf) {                                   \
    if (lf < 0.500 || lf > 1.000) return;                                                                    \
    m->lf = lf;                                                                                              \
    double curr_lf = (double) m->size / m->cap;                                                              \
    if (curr_lf > m->lf) {                                                                                   \
        umap_rehash_##id(m, m->cap << 1);                                                                    \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void umap_insert_##id(UMap_##id *m, kt key, t value) {                                      \
    __map_check_valid_key_##key_decl(key)                                                                    \
                                                                                                             \
    double lf = (double) m->size / m->cap;                                                                   \
    if (lf > m->lf) {                                                                                        \
        umap_rehash_##id(m, m->cap << 1);                                                                    \
    }                                                                                                        \
                                                                                                             \
    /* get index and entry at that index */                                                                  \
    uint32_t index = murmurhash(__hash_key_##key_decl(key), __hash_len_##key_decl(key), m->seed) % m->cap;   \
    UMapEntry_##id *e = NULL;                                                                                \
    __umap_find_index(UMapEntry_##id, m, cmp_lt, e, index, key)                                              \
                                                                                                             \
    if (e) {                                                                                                 \
        e->p.second = value;                                                                                 \
    } else {                                                                                                 \
        e = __ds_calloc(1, sizeof(UMapEntry_##id));                                                          \
        __map_allocate_new_key_##key_decl(e, key)                                                            \
        e->next = m->buckets[index];                                                                         \
        m->buckets[index] = e;                                                                               \
        m->size++;                                                                                           \
        e->p.second = value;                                                                                 \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void umap_erase_##id(UMap_##id *m, const kt key) {                                          \
    __map_check_valid_key_##key_decl(key)                                                                    \
                                                                                                             \
    uint32_t index = murmurhash(__hash_key_##key_decl(key), __hash_len_##key_decl(key), m->seed) % m->cap;   \
    if (!(m->buckets[index])) return; /* this entry does not exist */                                        \
                                                                                                             \
    UMapEntry_##id *prev = m->buckets[index];                                                                \
    UMapEntry_##id *curr = prev->next;                                                                       \
    if (ds_cmp_eq(cmp_lt, prev->p.first, key)) { /* special case when it's the first bucket entry */         \
        m->buckets[index] = curr;                                                                            \
        __map_remove_key_##key_decl(prev)                                                                    \
        free(prev);                                                                                          \
        m->size--;                                                                                           \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    while (curr) {                                                                                           \
        if (ds_cmp_eq(cmp_lt, curr->p.first, key)) {                                                         \
            prev->next = curr->next;                                                                         \
            __map_remove_key_##key_decl(curr)                                                                \
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
            __map_remove_key_##key_decl(e)                                                                   \
            free(e);                                                                                         \
            e = temp;                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
    memset(m->buckets, 0, sizeof(UMapEntry_##id *) * m->cap);                                                \
    m->size = 0;                                                                                             \
}                                                                                                            \

#endif /* MAP_H */
