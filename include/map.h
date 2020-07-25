#ifndef MAP_H
#define MAP_H

#include "ds.h"
#include <stdint.h>

#define __MAP_FNV_PRIME 0x01000193UL
#define __MAP_FNV_OFFSET_BASIS 0x811c9dc5UL
#define __MAP_DEFAULT_LOAD_FACTOR 0.750
#define __MAP_INITIAL_NBUCKETS 32

__DS_FUNC_PREFIX_INL uint32_t __map_fnv_int(int32_t k) {
    uint32_t hash = __MAP_FNV_OFFSET_BASIS;
    for (const char *c = (const char *) &k; *c; ++c) {
        hash ^= *c;
        hash *= __MAP_FNV_PRIME;
    }
    return hash;
}

__DS_FUNC_PREFIX_INL uint32_t __map_fnv_str(const char *k) {
    uint32_t hash = __MAP_FNV_OFFSET_BASIS;
    for (const char *c = k; *c; ++c) {
        hash ^= *c;
        hash *= __MAP_FNV_PRIME;
    }
    return hash;
}

typedef enum {
    INT,
    STR,
    STRPTR
} KeyType;

#define __map_end(m) ((m)->it.curr == NULL && (m)->it.idx >= (m)->cap)


/**
 * The number of entries in the map.
 *
 * @param   m  Pointer to map.
 */
#define map_size(m) ((int) (m)->size)


/**
 * The current value of the map's max load factor.
 *
 * @param   m  Pointer to map.
 */
#define map_max_load_factor(m) ((m)->lf)


/**
 * Whether the map is empty.
 *
 * @param   m  Pointer to map.
 */
#define map_empty(m) (!((m)->size))


/**
 * Macro for map_find(id, m, k).
 *
 * @param  id  ID used with gen_map.
 * @param  m   Pointer to map.
 * @param  k   Key to find, of type int or char *, depending on what value of "key" was passed to
 *               gen_map.
 */
#define map_at(id, m, k) map_find(id, m, k)


/**
 * Iterates through all entries in the map.
 *
 * @param  id  ID used with gen_map.
 * @param  m   Pointer to map.
 * @param  e   Pointer which will point to an entry's data.
 */
#define map_iter(id, m, e)                                                                                   \
    for (__map_iter_begin_##id(m), (e) = (((m)->it.curr != NULL) ? &((m)->it.curr->data) : NULL);            \
         !(__map_end((m)));                                                                                  \
         __map_iter_next_##id(m), (e) = (((m)->it.curr != NULL) ? &((m)->it.curr->data) : NULL))


/**
 * Depending on the key type, inserts the provided key and value into the map. If the key
 *   already exists, it will be updated with the provided value.
 *
 * @param  id  ID used with gen_map.
 * @param  m  Pointer to map.
 * @param  k  Key: either char * (if the key type is STR or STRPTR) or int (if the key type is INT).
 * @param  v  Value to be inserted.
 */
#define map_insert(id, m, key, value) map_insert_##id(m, key, value)


/**
 * Depending on the key type, finds the provided key and returns a pointer to its value.
 *
 * @param  id  ID used with gen_map.
 * @param  m   Pointer to map.
 * @param  k   Key: either char * (if the key type is STR or STRPTR) or int (if the key type is INT).
 *
 * @return     If the key is found, returns a pointer to the data corresponding to that key. If
 *                the key does not exist, returns NULL.
 */
#define map_find(id, m, key) map_find_##id(m, key)


/**
 * Removes the key and its corresponding data from the map, if it exists.
 *
 * @param  id  ID used with gen_map.
 * @param  m   Pointer to map.
 * @param  k   Key: either char * (if the key type is STR or STRPTR) or int (if the key type is INT).
 */
#define map_erase_key(id, m, key) map_erase_##id(m, key)


/**
 * Creates a new map struct.
 *
 * @param  id  ID used with gen_map.
 *
 * @return          Pointer to newly created map.
 */
#define map_new(id)  map_new_##id() 


/**
 * Deletes all elements and frees the map.
 *
 * @param  id  ID used with gen_map.
 * @param  m   Pointer to map.
 */
#define map_free(id, m) map_free_##id(m) 


/**
 * Modifies the number of buckets in the hash table. If this is less than or equal
 *   to the current number of buckets, nothing is done.
 *
 * @param  id        ID used with gen_map.
 * @param  m         Pointer to map.
 * @param  nbuckets  New number of buckets to use in the hash table.
 */
#define map_rehash(id, m, nbuckets) map_rehash_##id(m, nbuckets)


/**
 * If the provided load factor is sensible, sets the maximum load factor to the provided
 *   value, and may rehash the map if required.
 *
 * @param  id  ID used with gen_map.
 * @param  m   Pointer to map.
 * @param  lf  The new load factor to use.
 */
#define map_set_load_factor(id, m, lf) map_set_load_factor_##id(m, lf) 


/**
 * Removes all entries from the map.
 *
 * @param  id  ID used with gen_map.
 * @param  m   Pointer to map.
 */
#define map_clear(id, m) map_clear_##id(m)


/**
 * Generates map code for a specified key type, value type and ID.
 *
 * @param   id   ID to be used for the map (must be unique).
 * @param   key  Key type to be used (one of INT, STR, or STRPTR).
 * @param   t    Value type to be stored in the map.
 */
#define gen_map(id, key, t)                                                                                  \
__gen_entry(id, key, t)                                                                                      \
__gen_map_common(id)                                                                                         \
__gen_map_declarations(id, key, t)                                                                           \
__gen_map_new_free(id)                                                                                       \
__gen_map_##key(id, t)                                                                                       \
__gen_map_set_lf(id)                                                                                         \

#define __gen_map_INT(id, t)                                                                                 \
__DS_FUNC_PREFIX void map_insert_##id(Map_##id *m, int32_t key, t value) {                                   \
    double lf = (double) m->size / m->cap; /* check load factor */                                           \
    if (lf > m->lf) {                                                                                        \
        map_rehash_##id(m, m->cap << 1);                                                                     \
    }                                                                                                        \
                                                                                                             \
    /* get index and entry at that index */                                                                  \
    uint32_t index = __map_fnv_int(key) % m->cap;                                                            \
    Entry_##id *e = __find_index_##id(m, key, index);                                                        \
                                                                                                             \
    if (e) {                                                                                                 \
        e->data = value;                                                                                     \
    } else {                                                                                                 \
        e = entry_new_##id();                                                                                \
        e->key = key;                                                                                        \
        e->next = m->buckets[index];                                                                         \
        m->buckets[index] = e;                                                                               \
        m->size++;                                                                                           \
        e->data = value;                                                                                     \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL t *map_find_##id(Map_##id *m, const int32_t key) {                                      \
    uint32_t index = __map_fnv_int(key) % m->cap;                                                            \
    Entry_##id *e = __find_index_##id(m, key, index);                                                        \
    return e ? &(e->data) : NULL;                                                                            \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void map_erase_##id(Map_##id *m, const int32_t key) {                                       \
    uint32_t index = __map_fnv_int(key) % m->cap;                                                            \
    if (!(m->buckets[index])) return; /* this entry does not exist */                                        \
                                                                                                             \
    Entry_##id *prev = m->buckets[index];                                                                    \
    Entry_##id *curr = prev->next;                                                                           \
                                                                                                             \
    if (prev->key == key) { /* special case when it's the first bucket entry */                              \
        m->buckets[index] = curr;                                                                            \
        free(prev);                                                                                          \
        m->size--;                                                                                           \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    while (curr) {                                                                                           \
        if (curr->key == key) {                                                                              \
            prev->next = curr->next;                                                                         \
            free(curr);                                                                                      \
            m->size--;                                                                                       \
            return;                                                                                          \
        }                                                                                                    \
                                                                                                             \
        prev = curr;                                                                                         \
        curr = curr->next;                                                                                   \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL Entry_##id *__find_index_##id(Map_##id *m, const int32_t key, uint32_t index) {         \
    Entry_##id *e = m->buckets[index];                                                                       \
    while (e) {                                                                                              \
        if (e->key == key) return e;                                                                         \
        e = e->next;                                                                                         \
    }                                                                                                        \
    return NULL;                                                                                             \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void map_clear_##id(Map_##id *m) {                                                          \
    Entry_##id *e = NULL, *temp = NULL;                                                                      \
    for (size_t i = 0; i < m->cap; ++i) { /* iterate over all buckets */                                     \
        e = m->buckets[i];                                                                                   \
                                                                                                             \
        while (e) { /* iterate through linked list */                                                        \
            temp = e->next;                                                                                  \
            free(e);                                                                                         \
            e = temp;                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
    memset(m->buckets, 0, sizeof(Entry_##id *) * m->cap);                                                    \
    m->size = 0;                                                                                             \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void map_rehash_##id(Map_##id *m, size_t nbuckets) {                                        \
    if (nbuckets <= m->cap) return;                                                                          \
                                                                                                             \
    size_t n = m->cap; /* for consistency, use a multiple of 2 of the capacity */                            \
    while (n < nbuckets) {                                                                                   \
        n <<= 1;                                                                                             \
    }                                                                                                        \
                                                                                                             \
    Entry_##id **new = malloc(n * sizeof(Entry_##id *));                                                     \
    if (!new) DS_OOM();                                                                                      \
                                                                                                             \
    memset(new, 0, n * sizeof(Entry_##id *));                                                                \
    Entry_##id *e = NULL, *temp = NULL;                                                                      \
    uint32_t index = 0;                                                                                      \
                                                                                                             \
    for (size_t i = 0; i < m->cap; ++i) {                                                                    \
        e = m->buckets[i];                                                                                   \
        while (e) {                                                                                          \
            temp = e->next;                                                                                  \
            index = __map_fnv_int(e->key) % n;                                                               \
            e->next = new[index];                                                                            \
            new[index] = e;                                                                                  \
            e = temp;                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
    free(m->buckets);                                                                                        \
    m->buckets = new;                                                                                        \
    m->cap = n;                                                                                              \
}                                                                                                            \

#define __gen_map_STR(id, t)                                                                                 \
__gen_map_str_common(id, t)                                                                                  \
__DS_FUNC_PREFIX void map_insert_##id(Map_##id *m, char *key, t value) {                                     \
    if (!key) return;                                                                                        \
                                                                                                             \
    double lf = (double) m->size / m->cap;                                                                   \
    if (lf > m->lf) {                                                                                        \
        map_rehash_##id(m, m->cap << 1);                                                                     \
    }                                                                                                        \
                                                                                                             \
    /* get index and entry at that index */                                                                  \
    uint32_t index = __map_fnv_str(key) % m->cap;                                                            \
    Entry_##id *e = __find_index_##id(m, key, index);                                                        \
                                                                                                             \
    if (e) {                                                                                                 \
        e->data = value;                                                                                     \
    } else {                                                                                                 \
        e = entry_new_##id();                                                                                \
        e->key = malloc(strlen(key) + 1);                                                                    \
        if (!(e->key)) {                                                                                     \
            DS_OOM();                                                                                        \
        }                                                                                                    \
                                                                                                             \
        strcpy(e->key, key);                                                                                 \
        e->next = m->buckets[index];                                                                         \
        m->buckets[index] = e;                                                                               \
        m->size++;                                                                                           \
        e->data = value;                                                                                     \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void map_erase_##id(Map_##id *m, const char *key) {                                         \
    uint32_t index = __map_fnv_str(key) % m->cap;                                                            \
    if (!(m->buckets[index])) return; /* this entry does not exist */                                        \
                                                                                                             \
    Entry_##id *prev = m->buckets[index];                                                                    \
    Entry_##id *curr = prev->next;                                                                           \
    if (streq(prev->key, key)) { /* special case when it's the first bucket entry */                         \
        m->buckets[index] = curr;                                                                            \
        free(prev->key);                                                                                     \
        free(prev);                                                                                          \
        m->size--;                                                                                           \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    while (curr) {                                                                                           \
        if (streq(curr->key, key)) {                                                                         \
            prev->next = curr->next;                                                                         \
            free(curr->key);                                                                                 \
            free(curr);                                                                                      \
            m->size--;                                                                                       \
            return;                                                                                          \
        }                                                                                                    \
        prev = curr;                                                                                         \
        curr = curr->next;                                                                                   \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void map_clear_##id(Map_##id *m) {                                                          \
    Entry_##id *e = NULL, *temp = NULL;                                                                      \
    for (size_t i = 0; i < m->cap; ++i) { /* iterate over all buckets */                                     \
        e = m->buckets[i];                                                                                   \
                                                                                                             \
        while (e) { /* iterate through linked list */                                                        \
            temp = e->next;                                                                                  \
            if (e->key) {                                                                                    \
                free(e->key);                                                                                \
            }                                                                                                \
            free(e);                                                                                         \
            e = temp;                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
    memset(m->buckets, 0, sizeof(Entry_##id *) * m->cap);                                                    \
    m->size = 0;                                                                                             \
}                                                                                                            \

#define __gen_map_STRPTR(id, t)                                                                              \
__gen_map_str_common(id, t)                                                                                  \
__DS_FUNC_PREFIX void map_insert_##id(Map_##id *m, char *key, t value) {                                     \
    if (!key) return;                                                                                        \
                                                                                                             \
    double lf = (double) m->size / m->cap;                                                                   \
    if (lf > m->lf) {                                                                                        \
        map_rehash_##id(m, m->cap << 1);                                                                     \
    }                                                                                                        \
                                                                                                             \
    /* get index and entry at that index */                                                                  \
    uint32_t index = __map_fnv_str(key) % m->cap;                                                            \
    Entry_##id *e = __find_index_##id(m, key, index);                                                        \
                                                                                                             \
    if (e) {                                                                                                 \
        e->data = value;                                                                                     \
    } else {                                                                                                 \
        e = entry_new_##id();                                                                                \
        e->key = key;                                                                                        \
        e->next = m->buckets[index];                                                                         \
        m->buckets[index] = e;                                                                               \
        m->size++;                                                                                           \
        e->data = value;                                                                                     \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void map_erase_##id(Map_##id *m, const char *key) {                                         \
    uint32_t index = __map_fnv_str(key) % m->cap;                                                            \
    if (!(m->buckets[index])) return; /* this entry does not exist */                                        \
                                                                                                             \
    Entry_##id *prev = m->buckets[index];                                                                    \
    Entry_##id *curr = prev->next;                                                                           \
    if (streq(prev->key, key)) { /* special case when it's the first bucket entry */                         \
        m->buckets[index] = curr;                                                                            \
        free(prev);                                                                                          \
        m->size--;                                                                                           \
        return;                                                                                              \
    }                                                                                                        \
                                                                                                             \
    while (curr) {                                                                                           \
        if (streq(curr->key, key)) {                                                                         \
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
__DS_FUNC_PREFIX void map_clear_##id(Map_##id *m) {                                                          \
    Entry_##id *e = NULL, *temp = NULL;                                                                      \
    for (size_t i = 0; i < m->cap; ++i) { /* iterate over all buckets */                                     \
        e = m->buckets[i];                                                                                   \
        while (e) { /* iterate through linked list */                                                        \
            temp = e->next;                                                                                  \
            free(e);                                                                                         \
            e = temp;                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
    memset(m->buckets, 0, sizeof(Entry_##id *) * m->cap);                                                    \
    m->size = 0;                                                                                             \
}                                                                                                            \

#define __gen_entry(id, key, t)                                                                              \
typedef struct Entry_##id Entry_##id;                                                                        \
__gen_entry_##key(id, t)                                                                                     \
Entry_##id *entry_new_##id(void) {                                                                           \
    Entry_##id *e = malloc(sizeof(Entry_##id));                                                              \
    if (!e) {                                                                                                \
        DS_OOM();                                                                                            \
    }                                                                                                        \
    memset(e, 0, sizeof(Entry_##id));                                                                        \
    return e;                                                                                                \
}                                                                                                            \

#define __gen_entry_INT(id, t)                                                                               \
struct Entry_##id {                                                                                          \
    Entry_##id *next;                                                                                        \
    int32_t key;                                                                                             \
    t data;                                                                                                  \
};                                                                                                           \

#define __gen_entry_STR(id, t)                                                                               \
struct Entry_##id {                                                                                          \
    Entry_##id *next;                                                                                        \
    char *key;                                                                                               \
    t data;                                                                                                  \
};                                                                                                           \

#define __gen_entry_STRPTR(id, t) __gen_entry_STR(id, t)

#define __gen_map_common(id)                                                                                 \
typedef struct {                                                                                             \
    Entry_##id *curr;                                                                                        \
    unsigned idx;                                                                                            \
} MapIterator_##id;                                                                                          \
                                                                                                             \
typedef struct {                                                                                             \
    size_t size;                                                                                             \
    size_t cap;                                                                                              \
    double lf;                                                                                               \
    MapIterator_##id it;                                                                                     \
    Entry_##id **buckets;                                                                                    \
} Map_##id;                                                                                                  \
                                                                                                             \
__DS_FUNC_PREFIX_INL void __map_iter_begin_##id(Map_##id *m) {                                               \
    if (!(m->size)) {                                                                                        \
        m->it.curr = NULL;                                                                                   \
        m->it.idx = m->cap;                                                                                  \
    } else {                                                                                                 \
        unsigned idx = 0;                                                                                    \
        for (; (idx < m->cap) && (m->buckets[idx] == NULL); ++idx);                                          \
        m->it.idx = idx;                                                                                     \
        m->it.curr = m->buckets[m->it.idx];                                                                  \
    }                                                                                                        \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void __map_iter_next_##id(Map_##id *m) {                                                \
    if (m->it.curr->next) {                                                                                  \
        m->it.curr = m->it.curr->next;                                                                       \
    } else {                                                                                                 \
        unsigned idx = m->it.idx + 1;                                                                        \
        for (; (idx < m->cap) && (m->buckets[idx] == NULL); ++idx);                                          \
        m->it.idx = idx;                                                                                     \
        m->it.curr = (m->it.idx >= m->cap) ? NULL : m->buckets[m->it.idx];                                   \
    }                                                                                                        \
}                                                                                                            \

#define __gen_map_declarations(id, key, t)                                                                   \
__DS_FUNC_PREFIX void map_clear_##id(Map_##id *m);                                                           \
__DS_FUNC_PREFIX void map_rehash_##id(Map_##id *m, size_t nbuckets);                                         \
__gen_map_declarations_##key(id, t)                                                                          \

#define __gen_map_declarations_INT(id, t)                                                                    \
__DS_FUNC_PREFIX Entry_##id *__find_index_##id(Map_##id *m, const int32_t key, uint32_t index);              \
__DS_FUNC_PREFIX t *map_find_##id(Map_##id *m, const int32_t key);                                           \
__DS_FUNC_PREFIX void map_insert_##id(Map_##id *m, int32_t key, t value);                                    \
__DS_FUNC_PREFIX void map_erase_##id(Map_##id *m, const int32_t key);                                        \

#define __gen_map_declarations_STR(id, t)                                                                    \
__DS_FUNC_PREFIX Entry_##id *__find_index_##id(Map_##id *m, const char *key, uint32_t index);                \
__DS_FUNC_PREFIX t *map_find_##id(Map_##id *m, const char *key);                                             \
__DS_FUNC_PREFIX void map_insert_##id(Map_##id *m, char *key, t value);                                      \
__DS_FUNC_PREFIX void map_erase_##id(Map_##id *m, const char *key);                                          \

#define __gen_map_declarations_STRPTR(id, t) __gen_map_declarations_STR(id, t)

#define __gen_map_new_free(id)                                                                               \
__DS_FUNC_PREFIX Map_##id *map_new_##id(void) {                                                              \
    Map_##id *m = malloc(sizeof(Map_##id));                                                                  \
    if (!m) {                                                                                                \
        DS_OOM();                                                                                            \
    }                                                                                                        \
                                                                                                             \
    memset(m, 0, sizeof(Map_##id));                                                                          \
    m->cap = __MAP_INITIAL_NBUCKETS;                                                                         \
    m->lf = __MAP_DEFAULT_LOAD_FACTOR;                                                                       \
                                                                                                             \
    m->buckets = malloc(m->cap * sizeof(Entry_##id *));                                                      \
    if (!m->buckets) {                                                                                       \
        DS_OOM();                                                                                            \
    }                                                                                                        \
                                                                                                             \
    memset(m->buckets, 0, m->cap * sizeof(Entry_##id *));                                                    \
    return m;                                                                                                \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL void map_free_##id(Map_##id *m) {                                                       \
    map_clear_##id(m);                                                                                       \
    free(m->buckets);                                                                                        \
    free(m);                                                                                                 \
}                                                                                                            \

#define __gen_map_set_lf(id)                                                                                 \
__DS_FUNC_PREFIX void map_set_load_factor_##id(Map_##id *m, double lf) {                                     \
    if (lf < 0.500 || lf > 1.000) return;                                                                    \
    m->lf = lf;                                                                                              \
    double curr_lf = (double) m->size / m->cap;                                                              \
    if (curr_lf > m->lf) {                                                                                   \
        map_rehash_##id(m, m->cap << 1);                                                                     \
    }                                                                                                        \
}                                                                                                            \

#define __gen_map_str_common(id, t)                                                                          \
__DS_FUNC_PREFIX_INL t *map_find_##id(Map_##id *m, const char *key) {                                        \
    uint32_t index = __map_fnv_str(key) % m->cap;                                                            \
    Entry_##id *e = __find_index_##id(m, key, index);                                                        \
    return e ? &(e->data) : NULL;                                                                            \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX_INL Entry_##id *__find_index_##id(Map_##id *m, const char *key, uint32_t index) {           \
    Entry_##id *e = m->buckets[index];                                                                       \
    while (e) {                                                                                              \
        if (streq(e->key, key)) return e;                                                                    \
        e = e->next;                                                                                         \
    }                                                                                                        \
    return NULL;                                                                                             \
}                                                                                                            \
                                                                                                             \
__DS_FUNC_PREFIX void map_rehash_##id(Map_##id *m, size_t nbuckets) {                                        \
    if (nbuckets <= m->cap) return;                                                                          \
                                                                                                             \
    size_t n = m->cap; /* for consistency, use a multiple of 2 of the capacity */                            \
    while (n < nbuckets) {                                                                                   \
        n <<= 1;                                                                                             \
    }                                                                                                        \
                                                                                                             \
    Entry_##id **new = malloc(n * sizeof(Entry_##id *));                                                     \
    if (!new) DS_OOM();                                                                                      \
                                                                                                             \
    memset(new, 0, n * sizeof(Entry_##id *));                                                                \
    Entry_##id *e = NULL, *temp = NULL;                                                                      \
    uint32_t index = 0;                                                                                      \
                                                                                                             \
    for (size_t i = 0; i < m->cap; ++i) {                                                                    \
        e = m->buckets[i];                                                                                   \
                                                                                                             \
        while (e) {                                                                                          \
            temp = e->next;                                                                                  \
            index = __map_fnv_str(e->key) % n;                                                               \
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

#endif /* MAP_H */
