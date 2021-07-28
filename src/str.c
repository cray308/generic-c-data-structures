#include "str.h"
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>

#define __str_find_x_of_body(checkCharsAction, afterCheckCharsAction, mainLoop, prevChar)                    \
    const char *c; char const *end;                                                                          \
    __ds_adjust_index(pos, this->size, return STRING_ERROR)                                                  \
    if (!chars) return STRING_ERROR;                                                                         \
    else if (!(*chars && n)) return (int) pos;                                                               \
    end = (n < 0) ? &chars[strlen(chars)] : &chars[n];                                                       \
                                                                                                             \
    for (c = chars; c != end; ++c) {                                                                         \
        if (this->s[pos] == *c) {                                                                            \
            checkCharsAction                                                                                 \
        }                                                                                                    \
    }                                                                                                        \
    afterCheckCharsAction                                                                                    \
    mainLoop {                                                                                               \
        if (this->s[pos] == this->s[prevChar]) continue;                                                     \
        for (c = chars; c != end; ++c) {                                                                     \
            if (this->s[pos] == *c) {                                                                        \
                checkCharsAction                                                                             \
            }                                                                                                \
        }                                                                                                    \
        afterCheckCharsAction                                                                                \
    }                                                                                                        \
    return STRING_NPOS;

#define __str_prefix_table_body(needle, start, len, incr, decr, whileCond) {                                 \
    const int minIndex = start;                                                                              \
    int cnd = minIndex; /* needle position */                                                                \
    int index = cnd incr 1; /* table position */                                                             \
    __ds_malloc(table, sizeof(int) * (unsigned) len)                                                         \
    table[cnd] = cnd;                                                                                        \
                                                                                                             \
    whileCond {                                                                                              \
        if (needle[index] == needle[cnd]) { /* matching characters */                                        \
            table[index] = cnd incr 1;                                                                       \
            cnd = cnd incr 1;                                                                                \
            index = index incr 1;                                                                            \
        } else { /* not a match */                                                                           \
            if (cnd != minIndex) { /* get previous table index */                                            \
                cnd = table[cnd decr 1];                                                                     \
            } else { /* set it to 0 (default) */                                                             \
                table[index] = minIndex;                                                                     \
                index = index incr 1;                                                                        \
            }                                                                                                \
        }                                                                                                    \
    }                                                                                                        \
}

#define __str_find_body(pos, hsLength, hsOffset, incr, decr, iMin, iMax, jMin, jMax, singleCharAction, tableCall, whileFind, result) \
    int res, len_haystack = hsLength;                                                                        \
    char *haystack;                                                                                          \
    __ds_adjust_index(pos, this->size, return STRING_ERROR)                                                  \
    if (!needle) return STRING_ERROR;                                                                        \
    else if (!(*needle && len_needle)) return (int) pos;                                                     \
                                                                                                             \
    if (len_needle < 0) len_needle = (int) strlen(needle);                                                   \
                                                                                                             \
    if (len_needle > len_haystack) return STRING_NPOS;                                                       \
    if (len_needle == 1) return singleCharAction;                                                            \
                                                                                                             \
    haystack = this->s + hsOffset, res = STRING_NPOS;                                                        \
    {                                                                                                        \
        int *table;                                                                                          \
        tableCall                                                                                            \
        __str_find_main_loop(haystack, needle, incr, decr, iMin, iMax, jMin, jMax, whileFind,                \
            res = result;                                                                                    \
            break;                                                                                           \
        )                                                                                                    \
        free(table);                                                                                         \
    }                                                                                                        \
    return res;

#define __str_find_main_loop(haystack, needle, incr, decr, iMin, iMax, jMin, jMax, cond, onSuccess) {        \
    const int iEnd = iMax;                                                                                   \
    int i = iMin, j = jMin;                                                                                  \
    while (cond) {                                                                                           \
        if (haystack[i] == needle[j]) {                                                                      \
            i = i incr 1;                                                                                    \
            j = j incr 1;                                                                                    \
        } else {                                                                                             \
            if (j != jMin) {                                                                                 \
                j = table[j decr 1];                                                                         \
            } else {                                                                                         \
                i = i incr 1;                                                                                \
            }                                                                                                \
        }                                                                                                    \
                                                                                                             \
        if (j == jMax) {                                                                                     \
            onSuccess                                                                                        \
        }                                                                                                    \
    }                                                                                                        \
}

void string_reserve(String *this, unsigned n) {
    unsigned val;
    char *tmp;
    if (n <= this->cap) return;

    val = this->cap;
    while (val < n) val <<= 1;
    __ds_realloc(tmp, this->s, val)
    this->s = tmp;
    this->cap = val;
}

void string_replace(String *this, unsigned pos, int nToReplace, const char *s, int len) {
    unsigned end, l = (unsigned) len, n = this->size - pos;
    if (!(s && *s && len)) return;
    else if (pos != this->size) {
        __ds_adjust_index(pos, this->size, return)
    }

    if (len < 0) l = (unsigned) strlen(s);
    if (nToReplace >= 0) n = min(n, (unsigned) nToReplace);

    string_reserve(this, this->size + l + 1);
    if ((end = pos + n) < this->size) {
        memmove(&this->s[pos + l], &this->s[end], this->size - end);
    }
    memcpy(&this->s[pos], s, l);

    if (l < n) {
        this->size -= (n - l);
    } else {
        this->size += (l - n);
    }
    this->s[this->size] = 0;
}

void string_replace_fromString(String *this, unsigned pos, int nToReplace, const String *other, unsigned subpos, int len) {
    unsigned l = other->size - subpos;
    __ds_adjust_index(subpos, other->size, return)
    if (!len) return;

    if (len >= 0) l = min(l, (unsigned) len);
    string_replace(this, pos, nToReplace, &other->s[subpos], (int) l);
}

void string_replace_repeatingChar(String *this, unsigned pos, int nToReplace, unsigned n, char c) {
    unsigned end, n2r = this->size - pos;
    if (!n) return;
    else if (pos != this->size) {
        __ds_adjust_index(pos, this->size, return)
    }

    if (nToReplace >= 0) n2r = min(n2r, (unsigned) nToReplace);

    string_reserve(this, this->size + n + 1);
    if ((end = pos + n2r) < this->size) {
        memmove(&this->s[pos + n], &this->s[end], this->size - end);
    }
    memset(&this->s[pos], c, n);

    if (n < n2r) {
        this->size -= (n2r - n);
    } else {
        this->size += (n - n2r);
    }
    this->s[this->size] = 0;
}

String *string_new_fromCStr(const char *s, int n) {
    String *t;
    __ds_malloc(t, sizeof(String))
    __ds_malloc(t->s, 64)
    t->size = 0;
    t->cap = 64;
    t->s[0] = 0;
    string_append(t, s, n);
    return t;
}

String *string_new_fromString(const String *other, unsigned pos, int n) {
    String *s = string_new();
    string_append_fromString(s, other, pos, n);
    return s;
}

String *string_new_repeatingChar(unsigned n, char c) {
    String *s = string_new();
    string_append_repeatingChar(s, n, c);
    return s;
}

void string_resize_usingChar(String *this, unsigned n, char c) {
    if (n > this->size) {
        string_reserve(this, n + 1);
        memset(&this->s[this->size], c, n - this->size);
    }
    this->s[n] = 0;
    this->size = n;
}

void string_erase(String *this, unsigned start, int n) {
    unsigned end, n2d = this->size - start;
    __ds_adjust_index(start, this->size, return)
    if (!n) return;

    if (n >= 0) n2d = min(n2d, (unsigned) n);

    if ((end = start + n2d) < this->size) { /* move any characters after end to start */
        memmove(&this->s[start], &this->s[end], this->size - end);
    }
    this->size -= n2d;
    this->s[this->size] = 0;
}

void string_shrink_to_fit(String *this) {
    char *tmp;
    if (this->size == 0 || this->cap <= 64 || this->size + 1 == this->cap) return;
    __ds_realloc(tmp, this->s, this->size + 1) /* realloc only enough space for string and '\0' */
    this->cap = this->size + 1;
    this->s = tmp;
}

int string_find_first_of(String *this, unsigned pos, const char *chars, int n) {
    __str_find_x_of_body(return (int) pos;, ____cds_do_nothing, for(++pos; pos < this->size; ++pos), pos-1)
}

int string_find_last_of(String *this, unsigned pos, const char *chars, int n) {
    __str_find_x_of_body(return (int) pos;, ____cds_do_nothing, for(--pos; pos != UINT_MAX; --pos), pos+1)
}

int string_find_first_not_of(String *this, unsigned pos, const char *chars, int n) {
    __str_find_x_of_body(break;, if (c == end) return (int) pos;, for(++pos; pos < this->size; ++pos), pos-1)
}

int string_find_last_not_of(String *this, unsigned pos, const char *chars, int n) {
    __str_find_x_of_body(break;, if (c == end) return (int) pos;, for(--pos; pos != UINT_MAX; --pos), pos+1)
}

int string_find(String *this, unsigned start_pos, const char *needle, int len_needle) {
    __str_find_body(start_pos, (int)(this->size - start_pos), start_pos, +, -, 0, len_haystack, 0, len_needle, string_find_first_of(this, start_pos, needle, 1), __str_prefix_table_body(needle, 0, len_needle, +, -, while(index < len_needle)), i < iEnd, (int)start_pos + (i - j))
}

int string_rfind(String *this, unsigned end_pos, const char *needle, int len_needle) {
    __str_find_body(end_pos, (int)(end_pos + 1), 0, -, +, (int)end_pos, -1, len_needle - 1, -1, string_find_last_of(this, end_pos, needle, 1), __str_prefix_table_body(needle, (len_needle-1), len_needle, -, +, while(index > -1)), i > iEnd, i + 1)
}

String *string_substr(String *this, unsigned start, int n, int step_size) {
    String *sub;
    int i = (int) start;
    __ds_adjust_index(start, this->size, return NULL)
    if (!n) return NULL;

    if (!step_size) step_size = 1;

    sub = string_new();

    if (step_size < 0) {
        int end = (n < 0) ? -1 : max(-1, i + (n * step_size));
        for (; i > end; i += step_size) {
            string_push_back(sub, this->s[i]);
        }
    } else {
        int size = (int) this->size;
        int end = (n < 0) ? size : min(size, i + (n * step_size));
        for (; i < end; i += step_size) {
            string_push_back(sub, this->s[i]);
        }
    }
    return sub;
}

String **string_split(String *this, const char *delim) {
    int len_delim, end, *positions, start = 0;
    unsigned arrIdx = 0, arrLen = 0;
    String **arr, *substring = NULL;
    if (!(delim && *delim && this->size)) return NULL;

    len_delim = (int) strlen(delim);
    __ds_calloc(positions, 8, sizeof(int))

    {
        unsigned pos_size = 8;
        int *table;
        __str_prefix_table_body(delim, 0, len_delim, +, -, while(index < len_delim))

        __str_find_main_loop(this->s, delim, +, -, 0, (int) this->size, 0, len_delim, i < iEnd,              \
            if (arrIdx == pos_size) {                                                                         \
                int *temp;                                                                                   \
                pos_size <<= 1;                                                                              \
                __ds_realloc(temp, positions, pos_size * sizeof(int))                               \
                positions = temp;                                                                            \
                memset(&positions[arrIdx + 1], 0, (pos_size - arrIdx + 1) * sizeof(int));              \
            }                                                                                                \
            positions[arrIdx++] = (i - j);                                                                    \
            j = table[j - 1];                                                                                \
        )
        if (arrIdx == pos_size) {
            int *temp;
            __ds_realloc(temp, positions, (pos_size + 1) * sizeof(int))
            positions = temp;
        }
        positions[arrIdx++] = -1;
        arrLen = arrIdx;
        free(table);
    }

    __ds_malloc(arr, (arrLen + 1) * sizeof(String *))
    arrIdx = 0, end = positions[0];

    while (end != -1) {
        substring = string_new();
        string_insert(substring, 0, &this->s[start], end - start);
        arr[arrIdx++] = substring;
        start = end + len_delim;
        end = positions[arrIdx];
    }

    substring = string_new();
    string_insert(substring, 0, &this->s[start], (int) this->size - start);
    arr[arrIdx++] = substring;
    arr[arrLen] = NULL;
    free(positions);
    return arr;
}

#if __STDC_VERSION__ >= 199901L
char *__string_read_format(int *n, const char *format, va_list args) {
    va_list localArgs;
    int _n = 0;
    unsigned buf_size = 256;
    char *buf, *temp;
    __ds_malloc(buf, buf_size)
    va_copy(localArgs, args);

    _n = vsnprintf(buf, buf_size, format, args);
    if (_n < 0) {
        free(buf);
        return NULL;
    }
    else if ((unsigned) _n < buf_size) {
        *n = _n;
        return buf;
    }
    buf_size = (unsigned) _n + 1; /* buffer was too small */
    __ds_realloc(temp, buf, buf_size)
    buf = temp;

    _n = vsnprintf(buf, buf_size, format, localArgs);
    if (_n < 0 || (unsigned) _n >= buf_size) {
        free(buf);
        return NULL;
    }
    *n = _n;
    return buf;
}

void string_replace_withFormat(String *this, unsigned pos, int nToReplace, const char *format, ...) {
    va_list args;
    int n = 0;
    char *result;
    va_start(args, format);
    result = __string_read_format(&n, format, args);
    va_end(args);
    if (!result) return;
    string_replace(this, pos, nToReplace, result, n);
    free(result);
}

String *string_new_withFormat(const char *format, ...) {
    String *s = string_new();
    va_list args;
    int n = 0;
    char *result;
    va_start(args, format);
    result = __string_read_format(&n, format, args);
    va_end(args);
    if (!result) return s;
    string_append(s, result, n);
    free(result);
    return s;
}
#endif /* __STDC_VERSION__ >= 199901L */
