#ifndef DS_H
#define DS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>


/**
 * Checks whether an index is reasonable.
 
 * @param   index  Index in data structure.
 * @param   size   Size of data structure to be indexed.
 * 
 * @return         -If the index is positive, whether it is less than the size.
 *                 -If the index is negative, whether subtracting it from the size is at least 0.
 */
inline bool check_index(int index, size_t size) {
    if (index < 0) {
        return ((int) size + (int) index) >= 0;
    } else {
        return index < (int) size;
    }
}


/**
 * If successful, returns the positive modulus.
 *
 * @param   index  Index in data structure.
 * @param   size   Size of data structure to be indexed.
 *
 * @return         If the index is valid, returns the positive modulus. Otherwise, returns -1.
 */
inline int modulo(int index, size_t size) {
    if (!check_index(index, size)) {
        return -1;
    }

    int m = index % (int) size;
    return (m < 0) ? (m + (int) size) : m;
}

/**
 * After casting to the appropriate types, should return:
 *   -1 if _e1 < _e2
 *   0 if _e1 == _e2
 *   1 if _e1 > _e2
 */
typedef int (*comparison)(const void *_e1, const void *_e2);

/**
 * After casting, this should copy _src to _dst (either a pointer or the value).
 */
typedef void (*copy_ds)(void *_dst, const void *_src);

/**
 * After casting, this should free any dynamically allocated memory associated with the
 * element.
 */
typedef void (*del_ds)(void *_elem);

typedef struct {
    size_t size;
    copy_ds copy;
    del_ds del;
    comparison cmp;
} DSHelper;

typedef struct Node Node;
struct Node {
    Node *next;
    char data[];
};

#ifndef DS_OOM
#define DS_OOM() \
    do { \
        fprintf(stderr, "Out of memory error.\n"); \
        exit(1); \
    } while(0)
#endif

#define DS_UNUSED __attribute__((__unused__))

#define min(a,b) (((a) <= (b)) ? (a) : (b))
#define max(a,b) (((a) >= (b)) ? (a) : (b))
#define streq(a,b) (strcmp((a), (b)) == 0)

/**
 * Creates a node for storing arbitrary data with the size provided as an argument.
 *
 * @param   size  Size, in bytes, of the node's data portion.
 *
 * @return        Pointer to newly created node.
 */
static __attribute__((__unused__)) Node *node_new(size_t size) {
    size_t bytes = sizeof(Node) + size;
    Node *node = malloc(bytes);
    if (!node) {
        DS_OOM();
    }
    memset(node, 0, bytes);
    return node;
}


/**
 * This function dynamically allocates a string in dst and copies src to dst.
 *
 * @param   void  _dst   The destination address (char **).
 * @param   void  _src   The source string's address (char **).
 */
void ds_str_val_copy(void *_dst, const void *_src);


/**
 * Frees the memory allocated by ds_str_val_copy.
 *
 * @param   void  _elt  The address of the string to be freed (char **).
 */
void ds_str_val_del(void *_elt);


/**
 * Copies the value of _src (char **) to _dst (char **). No dynamic allocation is used.
 *
 * @param   void  _dst   The destination address (char **).
 * @param   void  _src   The source string's address (char **).
 */
void ds_str_ptr_copy(void *_dst, const void *_src);


/**
 * Performs strcmp() on _e1 and _e2, both which are represented as (char **).
 *
 * @param   void  _e1    Pointer to the first string.
 * @param   void  _e2    Pointer to the second string.
 *
 * @return  int          The result of strcmp.
 */
int ds_str_cmp(const void *_e1, const void *_e2);


/**
 * Compares _e1 and _e2 as (int *).
 *
 * @param   void  _e1    Pointer to the first int.
 * @param   void  _e2    Pointer to the second int.
 *
 * @return  int          Whether _e1 is less than, equal to, or greater than _e2.
 */
int ds_int_cmp(const void *_e1, const void *_e2);

static const DSHelper int_helper __attribute__((__unused__)) = {sizeof(int), NULL, NULL, ds_int_cmp};
static const DSHelper str_val_helper __attribute__((__unused__)) = {sizeof(char *), ds_str_val_copy, ds_str_val_del, ds_str_cmp};
static const DSHelper str_ptr_helper __attribute__((__unused__)) = {sizeof(char *), ds_str_ptr_copy, NULL, ds_str_cmp};

#endif
