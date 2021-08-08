# C Data Structures

## Introduction

This was mainly a personal project to see if I could create some generic data structures
in C, a language where generic anything is somewhat difficult to implement. I got
inspiration from the [UThash library](https://github.com/troydhanson/uthash) by Troy D.
Hanson. I wrote this library on Linux using gcc, and the `master` branch conforms to the C89 
standard. Thus, it should be compatible with most compilers.

Originally, I made everything generic by storing any data type in a `char[]` occupying some number of
bytes. This works fine, but after adding some "polymorphism" with variable-argument functions
(using `va_arg` from `stdarg.h`), I realized I could use macros to generate code specific to a
certain data type and avoid casting variables from `char *` to `void *`.

No linking with any of the files in this library is required, except for the below (to cut down code size):

 - `String`: Link `src/str.c`.
 - `UMap`/`USet`: Link `src/hash.c`.

The data structures are generally set up where you'll need to expand 2 macros - one for type and
function declarations, and once more for the function definitions, using the pattern below (note
that some macros have more arguments - these are described in the individual header files):

 - `gen_XXX_headers(id, t)`
 - `gen_XXX_source(id, t, copyValueMacroOrFunction, deleteValueMacroOrFunction)`

Using `include/array.h` as an example, it might look like:

```c
/* my_file.h */
#include "array.h"

gen_array_headers(int, int) /* Declares the Array_int type and relevant functions */

void myFunction(Array_int *a);
```

and

```c
/* my_file.c */
#include "my_file.h"

gen_array_source(int, int, DSDefault_shallowCopy, DSDefault_shallowDelete) /* Defines the functions */

void myFunction(Array_int *a) {
    ...
}
```

Of course, you could place `gen_array_headers` and `gen_array_source` in the same source file too
if you don't need to use that type in any other files. I have split the declarations and definitions
across two macros so that, if desired, the option is there to reuse the functions across multiple
files without increasing code size. For some context, in an earlier implementation, I used the
`static` keyword to be able to use the functions across multiple source files. The issue I have with
this is that each source file including the functions would get its own copy of the functions, which
increases code size - that's why the current implementation uses non-static functions.

## Included Data Structures

Documentation is provided in the header files (in the `include` folder). See the files in the `test`
folder for examples of using the data structures. Most of the data structures' macro wrappers around 
the functions are set up like `(ID, pointerToDataStructure, ...)`. The `ID` is necessary so that the
correct function is called when the macro is expanded.

 - Array (named `Array`). This is analogous to a C++ `std::vector`.

 - List (named `List`). This is analogous to a C++ `std::list` and uses a doubly-linked list internally. This can also function as a deque.

 - Deque (named `Deque`). Allows adding or removing elements from the front and back.

 - Queue (named `Queue`). In contrast to `Deque`, this only allows pushing to the back and popping from the front).

 - Stack (named `Stack`). Allows pushing onto the back and popping from the back.

 - AVL tree - there are 2 concrete data structures which use this:
    - Dictionary (named `Map`). This is similar to a C++ `map`; it stores key-value pairs, and is an 
    alternative to the hash table implementation.
    - Set (named `Set`). This is similar to a C++ `set`; it stores a unique set of keys, and is an 
    alternative to the hash table implementation.

 - Hash table - there are 2 concrete data structures which use this:
    - Dictionary (named `UMap`). This is similar to a C++ `unordered_map`.
    - Set (named `USet`). This is similar to a C++ `unordered_set`.

 - String (named `String`). This is similar to a C++ `std::string`, and also includes a function for inserting a printf-style format string (for C99 and above).
