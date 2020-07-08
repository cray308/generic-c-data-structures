#include "defaults.h"
#include "map.h"

#define FNV_PRIME 0x01000193UL
#define FNV_OFFSET_BASIS 0x811c9dc5UL
#define DEFAULT_LOAD_FACTOR 0.750
#define INITIAL_NBUCKETS 32

typedef unsigned char map_uint8_t;

map_uint32_t fnv(const void *k, size_t n);
Entry *entry_new(size_t size);
Entry *_find_int_index(Map *m, const map_int32_t key, map_uint32_t index);
Entry *_find_str_index(Map *m, const char *key, map_uint32_t index);
void *_map_find_int(Map *m, const map_int32_t key);
void *_map_find_str(Map *m, const char *key);
void _map_insert_int(Map *m, const map_int32_t key, const void *value);
void _map_insert_str(Map *m, const char *key, const void *value);
void _map_erase_int(Map *m, const map_int32_t key);
void _map_erase_str(Map *m, const char *key);

#define map_copy(m, e, value) \
    do { \
        if ((m)->helper.copy) { \
            (m)->helper.copy((e)->data, (value)); \
        } else { \
            memcpy((e)->data, (value), (m)->helper.size); \
        } \
    } while(0)

#define map_rm(m, e) \
    do { \
        if ((m)->helper.del) { \
            (m)->helper.del((e)->data); \
        } \
    } while(0)

Entry *entry_new(size_t size) {
    size_t bytes = sizeof(Entry) + size;
    Entry *e = malloc(bytes);
    if (!e) {
        DS_OOM();
    }
    memset(e, 0, bytes);
    return e;
}

Map *map_new(KeyType kt, const DSHelper *helper) {
    if (!helper || helper->size == 0) {
        return NULL;
    }

    Map *m = malloc(sizeof(Map));
    if (!m) {
        DS_OOM();
    }

    memset(m, 0, sizeof(Map));
    m->helper = *helper;
    m->cap = INITIAL_NBUCKETS;
    m->kt = kt;
    m->lf = DEFAULT_LOAD_FACTOR;

    m->buckets = malloc(m->cap * sizeof(Entry *));
    if (!m->buckets) {
        DS_OOM();
    }

    memset(m->buckets, 0, m->cap * sizeof(Entry *));
    return m;
}

void map_free(Map *m) {
    map_clear(m);
    free(m->buckets);
    free(m);
}

void map_clear(Map *m) {
    Entry *e = NULL, *temp = NULL;
    size_t i;
    for (i = 0; i < m->cap; ++i) { /* iterate over all buckets */
        e = m->buckets[i];

        while (e) { /* iterate through linked list */
            temp = e->next;
            map_rm(m, e);
            if (m->kt == STRING && e->key.k_str) {
                free(e->key.k_str);
            }
            free(e);
            e = temp;
        }
    }
    memset(m->buckets, 0, sizeof(Entry*) * m->cap);
    m->size = 0;
}

void map_insert(Map *m, const void *key, const void *value) {
    switch (m->kt) {
        case NUMBER:
            _map_insert_int(m, *(map_int32_t *) key, value);
            break;
        case STRING:
            _map_insert_str(m, (const char *) key, value);
            break;
    }
}

void _map_insert_int(Map *m, const map_int32_t key, const void *value) {
    double lf = (double) m->size / m->cap; /* check load factor */
    if (lf > m->lf) {
        map_rehash(m, m->cap << 1);
    }

    /* get index and entry at that index */
    map_uint32_t index = fnv(&key, sizeof(map_int32_t)) % m->cap;
    Entry *e = _find_int_index(m, key, index);

    if (e) {
        map_rm(m, e);
        map_copy(m, e, value);
    } else {
        e = entry_new(m->helper.size);
        e->key.k_int = key;
        e->next = m->buckets[index];
        m->buckets[index] = e;
        m->size++;
        map_copy(m, e, value);
    }
}

void _map_insert_str(Map *m, const char *key, const void *value) {
    if (!key) return;

    double lf = (double) m->size / m->cap;
    if (lf > m->lf) {
        map_rehash(m, m->cap << 1);
    }

    /* get index and entry at that index */
    size_t nBytes = strlen(key);
    map_uint32_t index = fnv(key, nBytes) % m->cap;
    Entry *e = _find_str_index(m, key, index);

    if (e) {
        map_rm(m, e);
        map_copy(m, e, value);
    } else {
        e = entry_new(m->helper.size);
        e->key.k_str = malloc(nBytes + 1);
        if (!(e->key.k_str)) {
            DS_OOM();
        }

        strcpy(e->key.k_str, key);
        e->next = m->buckets[index];
        m->buckets[index] = e;
        m->size++;
        map_copy(m, e, value);

    }
}

void *map_find(Map *m, const void *key) {
    void *rv = NULL;
    switch (m->kt) {
        case NUMBER:
            rv = _map_find_int(m, *(map_int32_t *) key);
            break;
        case STRING:
            rv = _map_find_str(m, (const char *) key);
            break;
    }
    return rv;
}

void *_map_find_int(Map *m, const map_int32_t key) {
    map_uint32_t index = fnv(&key, sizeof(map_int32_t)) % m->cap;
    Entry *e = _find_int_index(m, key, index);
    return e ? e->data : NULL;
}

void *_map_find_str(Map *m, const char *key) {
    size_t nBytes = strlen(key);
    map_uint32_t index = fnv(key, nBytes) % m->cap;
    Entry *e = _find_str_index(m, key, index);
    return e ? e->data : NULL;
}

void map_erase_key(Map *m, const void *key) {
    switch (m->kt) {
        case NUMBER:
            _map_erase_int(m, *(map_int32_t *) key);
            break;
        case STRING:
            _map_erase_str(m, (const char *) key);
            break;
    }
}

void _map_erase_int(Map *m, const map_int32_t key) {
    map_uint32_t index = fnv(&key, sizeof(map_int32_t)) % m->cap;
    if (!(m->buckets[index])) return; /* this entry does not exist */

    Entry* prev = m->buckets[index];
    Entry *curr = prev->next;

    if (prev->key.k_int == key) { /* special case when it's the first bucket entry */
        m->buckets[index] = curr;
        map_rm(m, prev);
        free(prev);
        m->size--;
        return;
    }

    while (curr) {
        if (curr->key.k_int == key) {
            prev->next = curr->next;
            map_rm(m, curr);
            free(curr);
            m->size--;
            return;
        }

        prev = curr;
        curr = curr->next;
    }
}

void _map_erase_str(Map *m, const char *key) {
    map_uint32_t index = fnv(key, strlen(key)) % m->cap;
    if (!(m->buckets[index])) return; /* this entry does not exist */

    Entry* prev = m->buckets[index];
    Entry *curr = prev->next;

    if (streq(prev->key.k_str, key)) { /* special case when it's the first bucket entry */
        m->buckets[index] = curr;
        map_rm(m, prev);
        free(prev->key.k_str);
        free(prev);
        m->size--;
        return;
    }

    while (curr) {
        if (streq(curr->key.k_str, key)) {
            prev->next = curr->next;
            map_rm(m, curr);
            free(curr->key.k_str);
            free(curr);
            m->size--;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

/* These two functions are to avoid calculating the fnv hash twice when
 * inserting, finding, or removing elements.
*/

Entry *_find_int_index(Map *m, const map_int32_t key, map_uint32_t index) {
    Entry *e = m->buckets[index];

    while (e) {
        if (e->key.k_int == key) return e;
        e = e->next;
    }

    return NULL;
}

Entry *_find_str_index(Map *m, const char *key, map_uint32_t index) {
    Entry *e = m->buckets[index];

    while (e) {
        if (streq(e->key.k_str, key)) return e;
        e = e->next;
    }

    return NULL;
}

void map_rehash(Map *m, size_t nbuckets) {
    if (nbuckets <= m->cap) return;

    size_t n = m->cap; /* for consistency, use a multiple of 2 of the capacity */
    while (n < nbuckets) {
        n <<= 1;
    }

    Entry **new = malloc(n * sizeof(Entry*));
    if (!new) DS_OOM();

    memset(new, 0, n * sizeof(Entry*));
    Entry *e = NULL, *temp = NULL;
    map_uint32_t index = 0;
    size_t i;

    for (i = 0; i < m->cap; ++i) {
        e = m->buckets[i];

        while (e) {
            temp = e->next;

            switch (m->kt) { /* find index based on the new number of buckets */
                case NUMBER:
                    index = fnv(&(e->key.k_int), sizeof(map_int32_t)) % n;
                    break;
                case STRING:
                    index = fnv(e->key.k_str, strlen(e->key.k_str)) % n;
                    break;
            }

            e->next = new[index];
            new[index] = e;
            e = temp;
        }
    }

    free(m->buckets);
    m->buckets = new;
    m->cap = n;
}

void map_set_load_factor(Map *m, double lf) {
    if (lf <= 0.500 || lf >= 1.000) return;

    m->lf = lf;
    double curr_lf = (double) m->size / m->cap;

    if (curr_lf > m->lf) {
        map_rehash(m, m->cap << 1);
    }
}

map_uint32_t fnv(const void *k, size_t n) {
    const map_uint8_t *key = (const map_uint8_t *) k;
    map_uint32_t hash = FNV_OFFSET_BASIS;
    size_t i;

    for (i = 0; i < n; ++i) {
        hash ^= key[i];
        hash *= FNV_PRIME;
    }
    return hash;
}
