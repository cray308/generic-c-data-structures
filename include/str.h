#ifndef STR_H
#define STR_H

#include "ds.h"
#include "iterator.h"
#include <ctype.h>
#include <stdarg.h>

typedef struct {
    size_t size;
    size_t cap;
    char *s;
} String;

#define STRING_NPOS (-1)
#define STRING_ERROR (-2)

#define __str_test_chars_body(f) const char *c; if (!*s) return 0; for(c = s; *c; ++c) { if (!f(*c)) return 0; } return 1;

#define __str_convert_case_body(f) char *c; for(c = s; *c; ++c) { *c = (char) f(*c); }

#define __str_find_x_of_body(checkCharsAction, afterCheckCharsAction, mainLoop, prevChar)                    \
    const char *c; char const *end;                                                                          \
    int size = string_len(this);                                                                             \
    __ds_adjust_index(pos, size)                                                                             \
    if (!chars || pos < 0) return STRING_ERROR;                                                              \
    else if (!(*chars && n)) return pos;                                                                     \
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
    return STRING_NPOS;                                                                                      \

#define __str_prefix_table_body(needle, start, len, incr, decr, whileCond) {                                 \
    const int minIndex = start;                                                                              \
    int cnd = minIndex; /* needle position */                                                                \
    int index = cnd incr 1; /* table position */                                                             \
    __ds_malloc(table, sizeof(int) * (size_t) len)                                                           \
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
    int res, len_haystack, size = string_len(this);                                                          \
    char *haystack;                                                                                          \
    __ds_adjust_index(pos, size)                                                                             \
    if (!needle || pos < 0) return STRING_ERROR;                                                             \
    else if (!(*needle && len_needle)) return pos;                                                           \
                                                                                                             \
    if (len_needle < 0) len_needle = (int) strlen(needle);                                                   \
                                                                                                             \
    len_haystack = hsLength;                                                                                 \
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
    return res;                                                                                              \

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


/**
 * The c-string representation of the provided String.
 */
#define string_c_str(this) ((this)->s)


/**
 * The number of characters in the string (analogous to strlen, but O(1) time complexity in this case).
 */
#define string_len(this) ((int) (this)->size)


/**
 * The capacity of the string (maximum size + 1, to account for the null character).
 */
#define string_capacity(this) ((int) (this)->cap)


/**
 * Tests whether the size of the string is 0.
 */
#define string_empty(this) (!((this)->size))


/**
 * Direct access to the char located at index `i` of the string. Does NOT perform bounds checking.
 *
 * @param  i  Index in string.
 */
#define string_index(this, i) ((this)->s[(i)])


/**
 * Reference to the string starting at index `i`. Performs bounds checking, and negative indices are allowed.
 *
 * @param  i  Index in string.
 */
__DS_FUNC_PREFIX_INL char *string_at(String *this, int i) {
    int size = string_len(this);
    __ds_adjust_index(i, size)
    return (i >= 0) ? &(this->s[i]) : NULL;
}


/**
 * Char pointer to the front of the string.
 */
#define string_front(this) iter_begin(STR, 0, (this)->s, (this)->size)


/**
 * Char pointer to the back of the string.
 */
#define string_back(this) iter_rbegin(STR, 0, (this)->s, (this)->size)


/**
 * Iterates through each character in the string from beginning to end.
 *
 * @param  it  Char pointer to use during iteration.
 */
#define string_iter(this, it) for (it = string_front(this); it != iter_end(STR, 0, (this)->s, (this)->size); iter_next(STR, 0, it))


/**
 * Iterates through each character in the string from end to the beginning.
 *
 * @param  it  Char pointer to use during iteration.
 */
#define string_riter(this, it) for (it = string_back(this); it != iter_rend(STR, 0, (this)->s, (this)->size); iter_prev(STR, 0, it))


/**
 * Request a change in capacity (maximum size) to `n`.
 *
 * @param  n  New capacity.
 */
__DS_FUNC_PREFIX void string_reserve(String *this, size_t n) {
    size_t val;
    char *tmp;
    if (n <= this->cap) return;

    val = this->cap;
    while (val < n) { /* double the capacity from what it was before */
        val <<= 1;
    }
    __ds_realloc(tmp, this->s, val)
    this->s = tmp;
    this->cap = val;
}


/**
 * Replaces `nToReplace` characters in this string, starting at `pos`, with `len` characters from `s`.
 *
 * @param  pos         Index in the string where the replacement will occur.
 * @param  nToReplace  Number of characters to overwrite in this string.
 * @param  s           C-string used as the replacement.
 * @param  len         Number of characters from `s` that will be used. If this is -1, all characters
 *                       from `s` will be used.
 */
__DS_FUNC_PREFIX void string_replace(String *this, int pos, int nToReplace, const char *s, int len) {
    int end, difference, size = string_len(this);
    if (!(s && *s && len)) return;
    else if (pos != size) {
        __ds_adjust_index(pos, size)
        if (pos < 0) return;
    }

    if (len < 0) len = (int) strlen(s);

    if (nToReplace < 0) {
        nToReplace = size - pos;
    } else {
        nToReplace = min(nToReplace, size - pos);
    }

    string_reserve(this, this->size + (size_t) len + 1);
    end = pos + nToReplace;
    if (end < size) { 
        memmove(&this->s[pos + len], &this->s[end], this->size - (size_t) end);
    }
    memcpy(&this->s[pos], s, (size_t) len);

    difference = len - nToReplace;
    if (difference < 0) {
        this->size -= ((size_t) -difference);
    } else {
        this->size += (size_t) difference;
    }
    this->s[this->size] = 0;
}


/**
 * Replaces `nToReplace` characters in this string, starting at `pos`, with `len` characters from `other` 
 * starting at `subpos`.
 *
 * @param  pos         Index in the string where the replacement will occur.
 * @param  nToReplace  Number of characters to overwrite in this string.
 * @param  other       Existing `String` from which characters will be inserted.
 * @param  subpos      Index in `other` denoting the position of the first character to be inserted.
 * @param  len         Number of characters from `other` to insert. If this is -1, all characters from
 *                       `subpos` through the end of `other` will be used.
 */
__DS_FUNC_PREFIX void string_replace_fromString(String *this, int pos, int nToReplace, const String *other, int subpos, int len) {
    int size = string_len(other);
    __ds_adjust_index(subpos, size)
    if (!len || subpos < 0) return;

    if (len < 0) {
        len = size - subpos;
    } else {
        len = min(len, size - subpos);
    }
    string_replace(this, pos, nToReplace, &other->s[subpos], len);
}


/**
 * Replaces `nToReplace` characters in this string, starting at `pos`, with `n` copies of `c`.
 *
 * @param  pos         Index in the string where the replacement will occur.
 * @param  nToReplace  Number of characters to overwrite in this string.
 * @param  n           Number of copies of `c` to insert.
 * @param  c           Character to insert.
 */
__DS_FUNC_PREFIX void string_replace_repeatingChar(String *this, int pos, int nToReplace, size_t n, char c) {
    int end, difference, size = string_len(this);
    if (!n) return;
    else if (pos != size) {
        __ds_adjust_index(pos, size)
        if (pos < 0) return;
    }

    if (nToReplace < 0) {
        nToReplace = size - pos;
    } else {
        nToReplace = min(nToReplace, size - pos);
    }

    string_reserve(this, this->size + n + 1);
    end = pos + nToReplace;
    if (end < size) { 
        memmove(&this->s[pos + (int) n], &this->s[end], this->size - (size_t) end);
    }
    memset(&this->s[pos], c, n);

    difference = (int) n - nToReplace;
    if (difference < 0) {
        this->size -= ((size_t) -difference);
    } else {
        this->size += (size_t) difference;
    }
    this->s[this->size] = 0;
}


/**
 * Inserts `len` characters from `s` into this string before `pos`.
 *
 * @param  pos  Index in this string before which characters will be inserted. If this is `string_len`,
 *                characters from `s` will be appended.
 * @param  s    C-string from which characters will be inserted.
 * @param  len  Number of characters from `s` to insert. If this is -1, all characters from `s` will
 *                be used.
 */
#define string_insert(this, pos, s, len) string_replace(this, pos, 0, s, len)


/**
 * Inserts a substring of `other`, starting at `subpos`, into this string before `pos`.
 *
 * @param  pos     Index in this string before which characters will be inserted. If this is `string_len`,
 *                   characters from `other` will be appended to this string.
 * @param  other   Existing `String` from which characters will be inserted.
 * @param  subpos  Index in `other` denoting the position of the first character to be inserted.
 * @param  len     Number of characters from `other` to insert. If this is -1, all characters from `subpos`
 *                   through the end of `other` will be inserted.
 */
#define string_insert_fromString(this, pos, other, subpos, len) string_replace_fromString(this, pos, 0, other, subpos, len)


/**
 * Inserts `n` instances of the character `c` into this string before `pos`.
 *
 * @param  pos  Index in this string before which characters will be inserted. If this is `string_len`,
 *                characters from `other` will be appended to this string.
 * @param  n    Number of copies of `c` to insert.
 * @param  c    Character to insert.
 */
#define string_insert_repeatingChar(this, pos, n, c) string_replace_repeatingChar(this, pos, 0, n, c)


/**
 * Appends `len` characters from `s` to the end of this string.
 *
 * @param  s    C-string from which characters will be inserted.
 * @param  len  Number of characters from `s` to insert. If this is -1, all characters from `s` will
 *                be used.
 */
#define string_append(this, s, len) string_insert(this, string_len(this), s, len)


/**
 * Appends a substring of `other`, starting at `subpos`, to this string.
 *
 * @param  other   Existing `String` from which characters will be inserted.
 * @param  subpos  Index in `other` denoting the position of the first character to be inserted.
 * @param  len     Number of characters from `other` to insert. If this is -1, all characters from `subpos`
 *                   through the end of `other` will be inserted.
 */
#define string_append_fromString(this, other, subpos, len) string_insert_fromString(this, string_len(this), other, subpos, len)


/**
 * Appends `n` instances of the character `c` to this string.
 *
 * @param  n  Number of copies of `c` to insert.
 * @param  c  Character to insert.
 */
#define string_append_repeatingChar(this, n, c) string_insert_repeatingChar(this, string_len(this), n, c)


/**
 * Creates a new string from a c-string `s`.
 * 
 * @param   s  C-string.
 * @param   n  Number of characters from `s` to include. If this is -1, `strlen(s)` characters
 *               will be used.
 *
 * @return     Pointer to newly created string.
 */
__DS_FUNC_PREFIX String *string_new_fromCStr(const char *s, int n) {
    String *t;
    __ds_malloc(t, sizeof(String))
    __ds_malloc(t->s, 64)
    t->size = 0;
    t->cap = 64;
    t->s[0] = 0;
    string_append(t, s, n);
    return t;
}


/**
 * Creates a new, empty string.
 *
 * @return  Pointer to newly created string.
 */
#define string_new() string_new_fromCStr(NULL, 0)


/**
 * Creates a new string as a copy of `other`.
 * 
 * @param   other  Pointer to existing `String`.
 *
 * @return         Pointer to newly created string.
 */
#define string_createCopy(other) string_new_fromCStr((other)->s, string_len(other))


/**
 * Creates a new string as a substring of `other`, starting at index `pos` and using `n` characters.
 *
 * @param   other  Pointer to existing `String`.
 * @param   pos    Index of the first character in `other` to be copied.
 * @param   n      Maximum number of characters to be used. If this is -1, all characters from `pos`
 *                   to the end of `other` will be used.
 *
 * @return         Pointer to newly created string.
 */
__DS_FUNC_PREFIX String *string_new_fromString(const String *other, int pos, int n) {
    String *s = string_new();
    string_append_fromString(s, other, pos, n);
    return s;
}


/**
 * Creates a new string with size `n`, where each index is set to `c`.
 *
 * @param   n  Number of characters to initialize.
 * @param   c  Character to set for each of the indices.
 *
 * @return     Pointer to the newly created string.
 */
__DS_FUNC_PREFIX String *string_new_repeatingChar(size_t n, char c) {
    String *s = string_new();
    string_append_repeatingChar(s, n, c);
    return s;
}


/**
 * Resizes the string to be `n` characters long. If this is less than the current size, all but the 
 * first `n` characters are removed. If this is greater than or equal to the current size, the provided 
 * character `c` is appended.
 *
 * @param  n  The new size.
 * @param  c  Character to append.
 */
__DS_FUNC_PREFIX_INL void string_resize_usingChar(String *this, size_t n, char c) {
    if (n > this->size) {
        string_reserve(this, n + 1);
        memset(&this->s[this->size], c, n - this->size);
    }
    this->s[n] = 0;
    this->size = n;
}


/**
 * Resizes the string to be `n` characters long. If this is less than the current size, all but the 
 * first `n` characters are removed. If this is greater than or equal to the current size, the null 
 * character is appended.
 *
 * @param  n  The new size.
 */
#define string_resize(this, n) string_resize_usingChar(this, n, 0)


/**
 * Removes `n` characters from the string, starting at index `start`.
 *
 * @param  start  The first index to delete.
 * @param  n      The number of characters to delete. If this is -1, all characters from `start`
 *                  until the end will be removed.
 */
__DS_FUNC_PREFIX void string_erase(String *this, int start, int n) {
    int end, size = string_len(this);
    __ds_adjust_index(start, size)
    if (!n || start < 0) return;

    if (n < 0) {
        n = size - start;
    } else {
        n = min(n, size - start);
    }

    end = start + n;
    if (end < size) { /* move any characters after end to start */
        memmove(&this->s[start], &this->s[end], this->size - (size_t) end);
    }
    this->size -= (size_t) n;
    this->s[this->size] = 0;
}


/**
 * Removes all characters, leaving the string with a size of 0.
 */
#define string_clear(this) do { this->s[0] = 0; this->size = 0; } while(0)


/**
 * Frees memory allocated to the string struct.
 */
#define string_free(this) do { free((this)->s); free(this); } while(0)


/**
 * If the string's capacity is greater than its length plus the null terminator, reallocates only 
 * enough space to fit all characters.
 */
__DS_FUNC_PREFIX_INL void string_shrink_to_fit(String *this) {
    char *tmp;
    if (this->size == 0 || this->cap <= 64 || this->size + 1 == this->cap) return;
    __ds_realloc(tmp, this->s, this->size + 1) /* realloc only enough space for string and '\0' */
    this->cap = this->size + 1;
    this->s = tmp;
}


/**
 * Appends `c` to the end of the string.
 *
 * @param  c  Character to append.
 */
#define string_push_back(this, c) string_resize_usingChar(this, (this)->size + 1, c)


/**
 * Removes the last character, if the string is not empty.
 */
#define string_pop_back(this) string_erase(this, string_len(this) - 1, 1)


/**
 * Finds the first index at or after `pos` where one of the characters in `chars` was found.
 *
 * @param   pos    First index in the string to consider.
 * @param   chars  C-string of characters to look for.
 * @param   n      Number of characters to consider in `chars`. If this is -1, all will be considered.
 *
 * @return         The first index at or after `pos` where one of the supplied characters was
 *                   found, `STRING_NPOS` if it was not found, or `STRING_ERROR` if an error occurred.
 */
int string_find_first_of(String *this, int pos, const char *chars, int n) {
    __str_find_x_of_body(return pos;, ____cds_do_nothing, for(++pos; pos < size; ++pos), pos-1)
}


/**
 * Finds the last index at or before `pos` where one of the characters in `chars` was found.
 *
 * @param   pos    Last index in the string to consider.
 * @param   chars  C-string of characters to look for.
 * @param   n      Number of characters to consider in `chars`. If this is -1, all will be considered.
 *
 * @return         The last index at or before `pos` where one of the supplied characters was
 *                   found, `STRING_NPOS` if it was not found, or `STRING_ERROR` if an error occurred.
 */
int string_find_last_of(String *this, int pos, const char *chars, int n) {
    __str_find_x_of_body(return pos;, ____cds_do_nothing, for(--pos; pos >= 0; --pos), pos+1)
}


/**
 * Finds the first index at or after `pos` where one of the characters in `chars` was not found.
 *
 * @param   pos    First index in the string to consider.
 * @param   chars  C-string of characters to look for.
 * @param   n      Number of characters to consider in `chars`. If this is -1, all will be considered.
 *
 * @return         The first index at or after `pos` where a different character was found,
 *                   `STRING_NPOS` if it was not found, or `STRING_ERROR` if an error occurred.
 */
int string_find_first_not_of(String *this, int pos, const char *chars, int n) {
    __str_find_x_of_body(break;, if (c == end) return pos;, for(++pos; pos < size; ++pos), pos-1)
}


/**
 * Finds the last index at or before `pos` where one of the characters in `chars` was not found.
 *
 * @param   pos    Last index in the string to consider.
 * @param   chars  C-string of characters to look for.
 * @param   n      Number of characters to consider in `chars`. If this is -1, all will be considered.
 *
 * @return         The last index at or before `pos` where a different character was found,
 *                   `STRING_NPOS` if it was not found, or `STRING_ERROR` if an error occurred.
 */
int string_find_last_not_of(String *this, int pos, const char *chars, int n) {
    __str_find_x_of_body(break;, if (c == end) return pos;, for(--pos; pos >= 0; --pos), pos+1)
}


/**
 * Finds the first occurrence of the first `len_needle` characters from `needle` in this string 
 * starting at `start_pos`.
 *
 * @param   start_pos   First index in the string to consider for the search.
 * @param   needle      Substring to find.
 * @param   len_needle  Number of characters to match from needle. If this is -1, all characters from
 *                        `needle` will be used.
 *
 * @return              The index in this string, corresponding to needle[0], where `needle` was found,
 *                      `STRING_NPOS` if it was not found, or `STRING_ERROR` if an error occurred.
 */
__DS_FUNC_PREFIX int string_find(String *this, int start_pos, const char *needle, int len_needle) {
    __str_find_body(start_pos, size - start_pos, start_pos, +, -, 0, len_haystack, 0, len_needle, string_find_first_of(this, start_pos, needle, 1), __str_prefix_table_body(needle, 0, len_needle, +, -, while(index < len_needle)), i < iEnd, start_pos + (i - j))
}


/**
 * Finds the last occurrence of the first `len_needle` characters from `needle` in this string 
 * ending at `end_pos`.
 *
 * @param   end_pos     Last index in the string to consider for the search.
 * @param   needle      Substring to find.
 * @param   len_needle  Number of characters to match from needle. If this is -1, all characters from
 *                        `needle` will be used.
 *
 * @return              The index in this string, corresponding to needle[0], where `needle` was found,
 *                        `STRING_NPOS` if it was not found, or `STRING_ERROR` if an error occurred.
 */
__DS_FUNC_PREFIX int string_rfind(String *this, int end_pos, const char *needle, int len_needle) {
    __str_find_body(end_pos, end_pos + 1, 0, -, +, end_pos, -1, len_needle - 1, -1, string_find_last_of(this, end_pos, needle, 1), __str_prefix_table_body(needle, (len_needle-1), len_needle, -, +, while(index > -1)), i > iEnd, i + 1)
}


/**
 * Creates a substring from this string with `n` characters, starting at `start` and moving to
 * the next character to include with a step size of `step_size`.
 *
 * @param   start      Index where the substring should start.
 * @param   n          Maximum number of characters in the substring. -1 implies to include as many
 *                       elements as `start` and `step_size` allow.
 * @param   step_size  How to adjust the index when copying characters. 1 means move forward 1 index
 *                       at a time, -1 means move backwards one index at a time, 2 would mean every
 *                       other index, etc.
 *
 * @return             Newly allocated `String`, or NULL if an error occurred.
 */
__DS_FUNC_PREFIX String *string_substr(String *this, int start, int n, int step_size) {
    String *sub;
    int size = string_len(this);
    __ds_adjust_index(start, size)
    if (!n || start < 0) return NULL;

    if (!step_size) step_size = 1;

    sub = string_new();

    if (step_size < 0) {
        int end = (n < 0) ? -1 : max(-1, start + (n * step_size));
        for (; start > end; start += step_size) {
            string_push_back(sub, this->s[start]);
        }
    } else {
        int end = (n < 0) ? size : min(size, start + (n * step_size));
        for (; start < end; start += step_size) {
            string_push_back(sub, this->s[start]);
        }
    }
    return sub;
}


/**
 * Splits this string into substrings based on `delim` and stores them as newly allocated `String`s 
 * in an array. A sentinel value of NULL is placed at the last index of the array.
 *
 * @param   delim  The delimiter to use to split the string.
 *
 * @return         The array of pointers to `String`, each of which is a substring of this string, or
 *                 NULL if an error occurred.
 */
__DS_FUNC_PREFIX String **string_split(String *this, const char *delim) {
    int len_delim, end, *positions, index = 0, arr_len = 0, start = 0;
    String **arr, *substring = NULL;
    if (!(delim && *delim && this->size)) return NULL;

    len_delim = (int) strlen(delim);
    __ds_calloc(positions, 8, sizeof(int))

    {
        int pos_size = 8, *table;
        __str_prefix_table_body(delim, 0, len_delim, +, -, while(index < len_delim))
    
        __str_find_main_loop(this->s, delim, +, -, 0, string_len(this), 0, len_delim, i < iEnd,              \
            if (index == pos_size) {                                                                         \
                int *temp;                                                                                   \
                pos_size <<= 1;                                                                              \
                __ds_realloc(temp, positions, (size_t) pos_size * sizeof(int))                               \
                positions = temp;                                                                            \
                memset(&positions[index + 1], 0, (size_t)(pos_size - index + 1) * sizeof(int));              \
            }                                                                                                \
            positions[index++] = (i - j);                                                                    \
            j = table[j - 1];                                                                                \
        )
        if (index == pos_size) {
            int *temp;
            __ds_realloc(temp, positions, (size_t)(pos_size + 1) * sizeof(int))
            positions = temp;
        }
        positions[index++] = -1;
        arr_len = index;
        free(table);
    }
    
    __ds_malloc(arr, (size_t)(arr_len + 1) * sizeof(String *))
    index = 0, end = positions[0];

    while (end != -1) {
        substring = string_new();
        string_insert(substring, 0, &this->s[start], end - start);
        arr[index++] = substring;
        start = end + len_delim;
        end = positions[index];
    }

    substring = string_new();
    string_insert(substring, 0, &this->s[start], string_len(this) - start);
    arr[index++] = substring;
    arr[arr_len] = NULL;
    free(positions);
    return arr;    
}


/**
 * Frees the memory allocated by `string_split`.
 *
 * @param  arr  Array allocated by `string_split`.
 */
__DS_FUNC_PREFIX_INL void string_split_free(String **arr) {
    String **s;
    for (s = arr; *s; ++s) {
        string_free(*s);
    }
    free(arr);
}


/**
 * @param   s  C-string.
 *
 * @return     Whether or not all characters in `s` are alphanumeric.
 */
__DS_FUNC_PREFIX_INL unsigned char isAlphaNum(const char *s) {
    __str_test_chars_body(isalnum)
}


/**
 * @param   s  C-string.
 *
 * @return     Whether or not all characters in `s` are letters.
 */
__DS_FUNC_PREFIX_INL unsigned char isAlpha(const char *s) {
    __str_test_chars_body(isalpha)
}


/**
 * @param   s  C-string.
 *
 * @return     Whether or not all characters in `s` are digits.
 */
__DS_FUNC_PREFIX_INL unsigned char isDigit(const char *s) {
    __str_test_chars_body(isdigit)
}


/**
 * Converts all characters in `s` to lowercase.
 *
 * @param  s  C-string.
 */
__DS_FUNC_PREFIX_INL void toLowercase(char *s) {
    __str_convert_case_body(tolower)
}


/**
 * Converts all characters in `s` to uppercase.
 *
 * @param  s  C-string.
 */
__DS_FUNC_PREFIX_INL void toUppercase(char *s) {
    __str_convert_case_body(toupper)
}


#if __STDC_VERSION__ >= 199901L
__DS_FUNC_PREFIX char *__string_read_format(int *n, const char *format, va_list args) {
    va_list localArgs;
    int _n = 0;
    size_t buf_size = 256;
    char *buf, *temp;
    __ds_malloc(buf, buf_size)
    va_copy(localArgs, args);

    _n = vsnprintf(buf, buf_size, format, args);
    if (_n < 0) {
        free(buf);
        return NULL;
    }
    else if ((size_t) _n < buf_size) {
        *n = _n;
        return buf;
    }
    buf_size = (size_t) _n + 1; /* buffer was too small */
    __ds_realloc(temp, buf, buf_size)
    buf = temp;

    _n = vsnprintf(buf, buf_size, format, localArgs);
    if (_n < 0 || (size_t) _n >= buf_size) {
        free(buf);
        return NULL;
    }
    *n = _n;
    return buf;
}

/**
 * Replaces `nToReplace` characters in this string, starting at `pos`, with the format string `format`.
 *
 * @param  pos         Index in the string where the replacement will occur.
 * @param  nToReplace  Number of characters to overwrite in this string.
 * @param  format      Format string.
 */
__DS_FUNC_PREFIX void string_replace_withFormat(String *this, int pos, int nToReplace, const char *format, ...) {
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

/**
 * Inserts a format string `format` into this string before `pos`.
 *
 * @param  pos     Index in this string before which characters will be inserted. If this is `string_len`,
 *                   characters from `format` will be appended to this string.
 * @param  format  Format string.
 */
#define string_insert_withFormat(this, pos, format, ...) string_replace_withFormat(this, pos, 0, format, __VA_ARGS__)

/**
 * Appends a format string `format` to this string.
 *
 * @param  format  Format string.
 */
#define string_append_withFormat(this, format, ...) string_insert_withFormat(this, string_len(this), format, __VA_ARGS__)

/**
 * Creates a new string from the format string `format`.
 * 
 * @param   format  Format string.
 *
 * @return          Pointer to newly created string.
 */
__DS_FUNC_PREFIX String *string_new_withFormat(const char *format, ...) {
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
#endif

#endif /* STR_H */
