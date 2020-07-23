# C Data Structures

## Introduction

This was mainly a personal project to see if I could create some generic data structures
in C, a language where generic anything is somewhat difficult to implement. I got
inspiration from the [UThash library](https://github.com/troydhanson/uthash) by Troy D.
Hanson. When I wrote this library, I designed it to run on Linux; if anyone
ends up using this in their projects with non-GNU compilers, I will work on
making it compatible with other compilers.

Originally, I made everything generic by storing any data type in a `char[]` occupying some number of
bytes. This works fine, but after adding some "polymorphism" with variable-argument functions
(using `va_arg` from `stdarg.h`), I realized I could use macros to generate code specific to a
certain data type and avoid casting variables from `char *` to `void *`. All of the functions are
declared static, so the data structures may be used by including the relevant files from the
`include` folder without linking with a library.

## Included Data Structures

Documentation is provided in the header files (in the `include` folder). See the files in the `test`
folder for examples of using the data structures.

 - Array (named `Array`). This is analogous to a C++ vector.

 - List (named `List`). This is analogous to a C++ list and uses a doubly-linked list
 internally. This can also function as a deque.

 - Queue (named `Queue`). In contrast to `List`, this only allows pushing to the back and
 popping from the front).

 - Stack (named `Stack`).

 - Red-black tree (named `Tree`).

 - Set (named `Set`). In this implementation, a `Set` is simply a typedef for `Tree`. Note that
 set operations may also be performed on arrays (built-in or the dynamic array mentioned above) or
 the `List` data structure.

 - String (named `String`). This is similar to a C++ string, and also includes a function
 for inserting a printf-style format string.

 - Hash table (named `Map`). This is analogous to a C++ `unordered_map` and is capable of
 using integer or string keys.
