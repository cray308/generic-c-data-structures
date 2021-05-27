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
certain data type and avoid casting variables from `char *` to `void *`. All of the functions are
declared static, so the data structures may be used by including the relevant files from the
`include` folder without linking with a library.

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
