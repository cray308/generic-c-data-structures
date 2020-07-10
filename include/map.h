#ifndef MAP_H
#define MAP_H

#include "ds.h"
#include <stdint.h>

typedef enum {
    NUMBER,
    STRING
} KeyType;

typedef struct Entry Entry;
struct Entry {
    Entry *next;
    union {
        char *k_str;
        int32_t k_int;
    } key;
    char data[];
};

typedef struct {
    Entry *curr;
    unsigned idx;
} MapIterator;

typedef struct {
    unsigned int size;
    unsigned int cap;
    double lf;
    MapIterator it;
    KeyType kt;
    DSHelper helper;
    Entry **buckets;
} Map;


inline void _map_iter_begin(Map *m) {
    if (!(m->size)) {
        m->it.curr = NULL;
        m->it.idx = m->cap;
    } else {
        unsigned idx = 0;
        for (; (idx < m->cap) && (m->buckets[idx] == NULL); ++idx);
        m->it.idx = idx;
        m->it.curr = m->buckets[m->it.idx];
    }
}

inline void _map_iter_next(Map *m) {
    if (m->it.curr->next) {
        m->it.curr = m->it.curr->next;
    } else {
        unsigned idx = m->it.idx + 1;
        for (; (idx < m->cap) && (m->buckets[idx] == NULL); ++idx);
        m->it.idx = idx;
        m->it.curr = (m->it.idx >= m->cap) ? NULL : m->buckets[m->it.idx];
    }
}


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
 * Macro for map_find(m, k).
 *
 * @param   m  Pointer to map.
 * @param   k  [description]
 */
#define map_at(m,k) map_find((m),(k))


#define _map_end(m) ((m)->it.curr == NULL && (m)->it.idx >= (m)->cap)


/**
 * Iterates through all entries in the map.
 *
 * @param   m  Pointer to map.
 * @param   e  Pointer which will point to an entry's data.
 */
#define map_iter(m,e) \
    for (_map_iter_begin((m)), (e) = (void*)(((m)->it.curr != NULL) ? (m)->it.curr->data : NULL); \
         !(_map_end((m))); \
         _map_iter_next(m), (e) = (void*)(((m)->it.curr != NULL) ? (m)->it.curr->data : NULL))


/**
 * Depending on the key type, inserts the provided key and value into the map. If the key
 *   already exists, it will be updated with the provided value.
 *
 * @param   m  Pointer to map.
 * @param   k  Pointer to key: either (char *) (if the key type is STRING) or (int *) (if the
 *               key type is INT).
 * @param   v  Pointer to value to be inserted.
 */
void map_insert(Map *m, const void *key, const void *value);


/**
 * Depending on the key type, finds the provided key and returns a pointer to its value.
 *
 * @param   m  Pointer to map.
 * @param   k  Pointer to key: either (char *) (if the key type is STRING) or (int *) (if the
 *               key type is INT).
 *
 * @return     If the key is found, returns a pointer to the data corresponding to that key. If
 *                the key does not exist, returns NULL.
 */
void *map_find(Map *m, const void *key);


/**
 * Removes the key and its corresponding data from the map, if it exists.
 *
 * @param   m  Pointer to map.
 * @param   k  Pointer to key: either (char *) (if the key type is STRING) or (int *) (if the
 *               key type is INT).
 */
void map_erase_key(Map *m, const void *key);


/**
 * Creates a new map struct.
 *
 * @param   kt      Key type to use in the map: either NUMBER or STRING.
 * @param   helper  Pointer to DSHelper struct.
 *
 * @return          Pointer to newly created map.
 */
Map *map_new(KeyType kt, const DSHelper *helper);


/**
 * Deletes all elements and frees the map.
 *
 * @param  m  Pointer to map.
 */
void map_free(Map *m);


/**
 * Modifies the number of buckets in the hash table. If this is less than or equal
 *   to the current number of buckets, nothing is done.
 *
 * @param  m         Pointer to map.
 * @param  nbuckets  New number of buckets to use in the hash table.
 */
void map_rehash(Map *m, size_t nbuckets);


/**
 * If the provided load factor is sensible, sets the maximum load factor to the provided
 *   value, and may rehash the map if required.
 *
 * @param  m   Pointer to map.
 * @param  lf  The new load factor to use.
 */
void map_set_load_factor(Map *m, double lf);


/**
 * Removes all entries from the map.
 *
 * @param  m  Pointer to map.
 */
void map_clear(Map *m);

#endif /* MAP_H */
