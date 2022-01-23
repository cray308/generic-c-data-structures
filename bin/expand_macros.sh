#!/bin/bash

includeStr="#include <stdint.h>\n\
#include <stdlib.h>\n\
#include <time.h>\n\
#include <string.h>\n\
#include <limits.h>\n\
#include <assert.h>\n\
#include <stdio.h>\n\
#include <ctype.h>\n\
#include <stdarg.h>\n\n\
#if UINT_MAX == 0xffffffff\n\
#define DS_HTABLE_MAX_SIZE 42949672\n\
#define DS_HTABLE_SHIFT_THRESHOLD 21474836\n\
#define DS_ARRAY_MAX_SIZE 0xfffffffe\n\
#define DS_ARRAY_SHIFT_THRESHOLD 0x7fffffff\n\
#define ARRAY_ERROR 0xffffffff\n\
#define DS_DQ_MAX_SIZE 0x7fffffff\n\
#define DS_DQ_SHIFT_THRESHOLD 0x3fffffff\n\
#define DS_STR_MAX_SIZE 0xfffffffd\n\
#define DS_STR_SHIFT_THRESHOLD 0x7ffffffe\n\
#define STRING_NPOS 0xfffffffe\n\
#define STRING_ERROR 0xffffffff\n\
#elif UINT_MAX == 0xffff\n\
#define DS_HTABLE_MAX_SIZE 655\n\
#define DS_HTABLE_SHIFT_THRESHOLD 327\n\
#define DS_ARRAY_MAX_SIZE 0xfffe\n\
#define DS_ARRAY_SHIFT_THRESHOLD 0x7fff\n\
#define ARRAY_ERROR 0xffff\n\
#define DS_DQ_MAX_SIZE 0x7fff\n\
#define DS_DQ_SHIFT_THRESHOLD 0x3fff\n\
#define DS_STR_MAX_SIZE 0xfffd\n\
#define DS_STR_SHIFT_THRESHOLD 0x7ffe\n\
#define STRING_NPOS 0xfffe\n\
#define STRING_ERROR 0xffff\n\
#endif\n\n"

sed -i "1s|^|${includeStr}|" $@
astyle $@
rm -f "$@.orig"
