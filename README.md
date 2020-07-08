# C Data Structures

## Introduction

This was mainly a personal project to see if I could create some generic data structures
in C, a language where generic anything is somewhat difficult to implement. I got
inspiration from the [UThash library](https://github.com/troydhanson/uthash) by Troy D.
Hanson. For better portability, most of this code conforms to the C89 standard
(with the exception of `include/str.h` - the function `string_printf` uses `vsnprintf` to
avoid a buffer overflow). When I wrote this library, I designed it to run on Linux; if anyone
ends up using this in their projects with non-GNU compilers, I will work on
making it compatible with other compilers. I designed this to be used as a shared library
(see the `Makefile` for details). To use it, clone the repository, and run `make`. At this point,
you may:

 - add `$REPOSITORY_FOLDER/lib` to `LD_LIBRARY_PATH` (recommended)
 - copy `$REPOSITORY_FOLDER/lib/libds.so` into `/usr/local/lib`

## Included Data Structures

Documentation is provided in the header files (in the `include` folder).

 - Array (named `Array`). This is analogous to a C++ vector.

 - List (named `List`). This is analogous to a C++ list and uses a doubly-linked list
 internally. This can also function as a deque.

 - Queue (named `Queue`). In contrast to `List`, this only allows pushing to the back and
 popping from the front).

 - Stack (named `Stack`).

 - Red-black tree (named `Tree`).

 - Set (named `Set`). In this implementation, a `Set` is simply a typedef for `Tree`.
 However, `include/set.h` defines additional functions that are oriented towards sets (i.e. union, 
 intersection, difference, symmetric difference, etc.).

 - String (named `String`). This is similar to a C++ string, and also includes a function
 for inserting a printf-style format string.

 - Hash table (named `Map`). This is analogous to a C++ `unordered_map` and is capable of
 using integer or string keys.

## How the Data Structures Work

Most of these data structures (with the exception of `String`) are capable of storing any
data type. They rely on a helper structure, `DSHelper` (defined in `include/ds.h`). See the
description below.

```c
typedef void (*copy_ds)(void *_dst, const void *_src);
typedef void (*del_ds)(void *_elem);
typedef int (*comparison)(const void *_e1, const void *_e2);

typedef struct {
    size_t size;
    copy_ds copy;
    del_ds del;
    comparison cmp;
} DSHelper;
```

 - The `size` member describes the size (as `sizeof()` would give) of the element type you
 wish to store.

 - The `copy` member is a function that casts void pointers into the appropriate pointer type
 and then copies the source to the destination as desired.

 - The `del` member is the "reverse" of the `copy` member. Any memory that was dynamically
 allocated in `copy` should be freed here.

 - The `cmp` member is a comparison function that returns -1 if `_e1 < _e2`, 0 if `_e1 == _e2`,
 and 1 if `_e1 > _e2`.

In general, the latter three functions require casting to a __pointer__ to the data type you
are storing. For instance, if you are storing an `int *` in the structure, the last three 
functions would cast the void pointer to `int **`. If the data you are storing requires no
special procedures to copy or delete, you may leave those members of the `DSHelper` struct
as `NULL`; the data structures will simply copy the bytes via `memcpy`.

If you don't need to make comparisons when using a data structure (note: there __must__ be a
comparison function for trees), you may also leave that member as `NULL`. An important note
about the comparison function is that both arguments should be the same type. For example, an
error will occur if you call a `find` function without passing a __pointer__ to the value you
wish to find. This is best illustrated with an example; I use the `List` data structure in
this scenario.

```c
List *l = list_new(&int_helper);
/* add some elements */
int find_val = 5;

ListEntry *e = list_find(l, find_val); /* WRONG!!! */

e = list_find(l, &find_val); /* this works */
```

Some `DSHelper` templates are provided at the bottom of `include/ds.h`:

 - `int_helper`: For storing `int`.

 - `str_val_helper`: For storing `char *` via a __deep copy__ (i.e. dynamically allocating
 a copy of the provided string).

 - `str_ptr_helper`: For storing `char *` such that it points to a string that will remain
 accessible throughout the program's lifecycle (i.e. a global `char *[]`).

The "node"-based data structures (list, queue, stack, tree, hash table) are designed similar
to the structure shown below.

```c
typedef struct Node Node;
struct Node {
    Node *next;
    char data[];
};
```

Since the elements' sizes are supplied in the `DSHelper` struct, the nodes are allocated so
that the `data` portion is large enough to hold `DSHelper.size` bytes.
