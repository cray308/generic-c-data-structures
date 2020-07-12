#include "ds.h"

void ds_str_val_copy(void *_dst, const void *_src) {
    char **dst = (char**) _dst;
    char **src = (char**) _src;
    if (*src) {
        *dst = malloc(strlen(*src) + 1);
        if (!(*dst)) DS_OOM();
        strcpy(*dst, *src);
    } else {
        *dst = NULL;
    }
}

void ds_str_val_del(void *_elt) {
    char **elt = (char**) _elt;
    if (*elt) {
        free(*elt);
    }
}

void ds_str_ptr_copy(void *_dst, const void *_src) {
    char **dst = (char**) _dst;
    char **src = (char**) _src;

    *dst = *src ? *src : NULL;
}

int ds_str_cmp(const void* _e1, const void* _e2) {
    const char **e1 = (const char **) _e1;
    const char **e2 = (const char **) _e2;
    return strcmp(*e1,*e2);
}

int ds_int_cmp(const void* _e1, const void* _e2) {
    const int *e1 = (const int*) _e1;
    const int *e2 = (const int*) _e2;
    return (*e1 == *e2) ? 0 : ((*e1 < *e2) ? -1 : 1);
}

int ds_unsigned_cmp(const void *_e1, const void *_e2) {
    const unsigned *e1 = (const unsigned*) _e1;
    const unsigned *e2 = (const unsigned*) _e2;
    return (*e1 == *e2) ? 0 : ((*e1 < *e2) ? -1 : 1);
}
