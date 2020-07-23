#ifndef STR_H
#define STR_H

#include "ds.h"
#include <ctype.h>

typedef enum {
    STR_INIT_NONE,
    STR_INIT_CSTR,
    STR_INIT_STRING
} StringInitializer;

typedef struct {
    size_t len;
    size_t cap;
    char *s;
} String;


#define STRING_END(s) ((int) (s)->len)
#define STRING_NPOS (-1)
#define STRING_ERROR (-2)
#define __STR_SPLIT_INITIAL_SIZE 8
#define __STR_INITIAL_STR_CAP 64

static DS_UNUSED void string_reserve(String *s, size_t n);
static DS_UNUSED void string_append(String *s, const char *other, int len);
void __str_printf_va(String *s, int pos, const char *format, va_list args);
int *__str_create_prefix_table(const char *needle, size_t len);
int *__str_create_prefix_table_rev(const char *needle, size_t len);
int __str_find_x_of(String *s, int pos, const char *chars, bool first, bool match);

#define __gen_test_chars(f)                                                                                  \
static inline DS_UNUSED bool __str_test_chars_##f(const char *s) {                                           \
    for (const char *c = s; *c; ++c) {                                                                       \
        if (!f(*c)) return false;                                                                            \
    }                                                                                                        \
    return true;                                                                                             \
}                                                                                                            \

#define __gen_convert_case(f)                                                                                \
static inline DS_UNUSED void __str_convert_case_##f(char *s) {                                               \
    for (char *c = s; *c; ++c) {                                                                             \
        *c = f(*c);                                                                                          \
    }                                                                                                        \
}                                                                                                            \

__gen_test_chars(isalnum)
__gen_test_chars(isalpha)
__gen_test_chars(isdigit)
__gen_convert_case(toupper)
__gen_convert_case(tolower)


/**
 * The c-string representation of the provided String.
 *
 * @param   str  Pointer to string struct.
 */
#define string_c_str(str) ((str)->s)


/**
 * The number of characters in the string (analogous to strlen, but O(1) time complexity in
 *   this case).
 *
 * @param   str  Pointer to string struct.
 */
#define string_len(str) ((int) (str)->len)


/**
 * The capacity of the string (maximum size + 1, to account for the null character).
 *
 * @param   str  Pointer to string struct.
 */
#define string_capacity(str) ((int) (str)->cap)


/**
 * Tests whether the size of the string is 0.
 *
 * @param   str  Pointer to string struct.
 */
#define string_empty(str) (!((str)->len))


/**
 * The numeric index in the string from a char pointer.
 *
 * @param   str  Pointer to string struct.
 * @param   chr  The char pointer whose index you wish to find.
 */
#define string_index(str, chr) ((int) ((chr) - (str)->s))

/**
 * Reference to the string starting at index i.
 *
 * @param   str  Pointer to string struct.
 * @param   i    Index in string.
 */
static inline DS_UNUSED char *string_ref(String *str, int i) {
    int _idx = modulo(i, str->len);
    return (_idx >= 0) ? &(str->s[_idx]) : NULL;
}

/**
 * The char located at index i of the string.
 *
 * @param   str  Pointer to string struct.
 * @param   i    Index in string.
 */
static inline DS_UNUSED char string_at(String *str, int i) {
    int _idx = modulo(i, str->len);
    return (_idx >= 0) ? str->s[_idx] : 0;
}


/**
 * A char pointer to the front of the string.
 *
 * @param   str  Pointer to string struct.
 */
#define string_front(str) (((str)->len) ? &((str)->s[0]) : NULL)


/**
 * A char pointer to the back of the string.
 *
 * @param   str  Pointer to string struct.
 */
#define string_back(str) (((str)->len) ? &((str)->s[(str)->len - 1]) : NULL)


/**
 * Iterates through each character in the string from beginning to end.
 *
 * @param   str    Pointer to string struct.
 * @param   chr    Char pointer to use during iteration.
 */
#define string_iter(str, chr)                                                                                \
    for ((chr) = ((str)->len) ? &((str)->s[0]) : NULL; (chr) != NULL;                                        \
    (chr) = ((chr) != &(((str)->s[(str)->len - 1]))) ? (chr) + 1 : NULL)


/**
 * Iterates through each character in the string from end to the beginning.
 *
 * @param   str    Pointer to string struct.
 * @param   chr    Char pointer to use during iteration.
 */
#define string_riter(str, chr)                                                                               \
    for ((chr) = ((str)->len) ? &((str)->s[(str)->len - 1]) : NULL; (chr) != NULL;                           \
    (chr) = ((chr) != &(((str)->s[0]))) ? (chr) - 1 : NULL)


/**
 * Creates a new string.
 * In (1), an empty String is created.
 * In (2), a String is initialized from a c-string (const char *).
 * In (3), a String is initialized from another pointer to String.
 * 
 * (1) init = STR_INIT_EMPTY:   string_new(StringInitializer init)
 * (2) init = STR_INIT_CSTR:    string_new(StringInitializer init, const char *str)
 * (3) init = STR_INIT_STRING:  string_new(StringInitializer init, const String *s)
 * 
 * @param   init  Type of initializer to execute.
 *
 * @return    Pointer to newly created string.
 */
static DS_UNUSED String *string_new(StringInitializer init, ...) {
    String *s = malloc(sizeof(String));
    if (!s) {
        DS_OOM();
    }
    memset(s, 0, sizeof(String));
    string_reserve(s, __STR_INITIAL_STR_CAP);

    if (init == STR_INIT_NONE) {
        s->s[0] = 0;
        return s;

    }

    const void *val;
    const char *str;
    const String *other;

    /* parse arguments */
    va_list args;
    va_start(args, init);

    val = va_arg(args, void *);

    va_end(args);

    if (val == NULL) {
        return s;
    }

    if (init == STR_INIT_CSTR) {
        str = (const char *) val;
        string_append(s, str, strlen(str));
    } else {
        other = (const String *) val;
        string_append(s, other->s, other->len);
    }

    return s;
}


/**
 * Frees memory allocated to the string struct.
 *
 * @param  s  Pointer to string.
 */
static inline DS_UNUSED void string_free(String *s) {
    if (s->cap) {
        free(s->s);
    }
    free(s);
}


/**
 * Request a change in capacity (maximum size).
 *
 * @param  s  Pointer to string.
 * @param  n  New capacity.
 */
static DS_UNUSED void string_reserve(String *s, size_t n) {
    if (n <= s->cap) {
        return;
    }

    size_t val = s->cap ? s->cap : 1;
    while (val < n) { /* double the capacity from what it was before */
        val <<= 1;
    }
    char *tmp = realloc(s->s, val);
    if (!tmp) {
        DS_OOM();
    }
    s->s = tmp;
    s->cap = val;
}


/**
 * Resizes the string to be n characters long. If n is less than the current size, all but the first n
 *   characters are removed. If n is greater than or equal to the current size, the provided
 *   character will be appended.
 *
 * @param  s  Pointer to string.
 * @param  n  The new size.
 * @param  c  Character to append.
 */
static DS_UNUSED void string_resize(String *s, size_t n, char c) {
    if (n > s->len) {
        string_reserve(s, n + 1);
        memset(&s->s[s->len], c, n - s->len);
        s->s[n] = 0;
    }
    s->s[n] = 0;
    s->len = n;
}


/**
 * Removes all characters, leaving the string with a size of 0.
 *
 * @param  s  Pointer to string.
 */
static inline DS_UNUSED void string_clear(String *s) {
    if (!(s->s)) {
        return;
    }
    s->s[0] = 0;
    s->len = 0;
}


/**
 * Removes "n" characters from the string, starting at index "start".
 *
 * @param  s      Pointer to string.
 * @param  start  The first index to delete.
 * @param  n      The number of characters to delete. If this is -1, all characters
 *                  from "start" until the end will be removed.
 */
static DS_UNUSED void string_erase(String *s, int start, int n) {
    if (!n || s->len == 0) {
        return;
    }

    start = modulo(start, s->len);
    if (start < 0) return;

    if (n < 0) {
        n = (int) s->len - start;
    } else {
        n = min(n, (int) s->len - start);
    }

    int end = start + n;
    
    if (end < string_len(s)) { /* move any characters after end to start */
        memmove(&s->s[start], &s->s[end], s->len - (size_t) end);
    }
    s->len -= (size_t) n;
    s->s[s->len] = 0;
}


/**
 * If the string's capacity is greater than its length plus the null terminator, reallocates
 *   only enough space to fit all characters.
 *
 * @param  s  Pointer to string.
 */
static inline DS_UNUSED void string_shrink_to_fit(String *s) {
    if (s->len + 1 == s->cap || s->len == 0) {
        return;
    }
    char *tmp = realloc(s->s, s->len + 1); /* realloc only enough space for string and '\0' */
    if (!tmp) {
        DS_OOM();
    }
    s->cap = s->len + 1;
    s->s = tmp;
}


/**
 * Appends a character to the end of the string.
 *
 * @param  s  Pointer to string.
 * @param  c  Character to append.
 */
static inline DS_UNUSED void string_push_back(String *s, char c) {
    if (c == 0) { /* doesn't make sense to push back a null character */
        return;
    }
    string_reserve(s, s->len + 1);
    s->s[s->len++] = c;
    s->s[s->len] = 0;
}


/**
 * Removes the last character, if the string is not empty.
 *
 * @param  s  Pointer to string.
 */
static inline DS_UNUSED void string_pop_back(String *s) {
    if (!s->len) {
        return;
    }
    s->s[s->len-- - 1] = 0;
}


/**
 * Replaces characters in s, starting at pos, with len characters from other.
 *
 * @param  s      Pointer to string.
 * @param  pos    Index in s where the replacement will occur.
 * @param  other  C-string used as the replacement.
 * @param  len    Number of characters from other that will be used. If this is -1,
 *                  all characters from "other" will be used.
 */
static DS_UNUSED void string_replace(String *s, int pos, const char *other, int len) {
    if (!other || (*other == '\0') || !len) {
        return;
    }

    if (pos >= (int) s->len) {
        string_append(s, other, len);
        return;
    }

    pos = modulo(pos, s->len);
    if (pos < 0) {
        return;
    }

    const int l_other = strlen(other);

    if (len < 0) { /* use all characters from other */
        len = l_other;
    } else {
        len = min(len, l_other); /* account for if len is larger than the supplied string */
    }

    string_reserve(s, (size_t) (pos + len + 1));
    memcpy(&s->s[pos], other, len);

    if (pos + len > (int) s->len) {
        s->len = (size_t) pos + (size_t) len;
        s->s[s->len] = 0;
    }
}


/**
 * Inserts "len" characters from the c-string "other" into "s" before "pos".
 *
 * @param  s      Pointer to string.
 * @param  pos    Index in "s" before which characters will be inserted. If this is STRING_END,
 *                  characters from "other" will be appended to s.
 * @param  other  C-string from which characters will be inserted.
 * @param  len    Number of characters from "other" to insert. If this is -1,
 *                  all characters from "other" will be used.
 */
static DS_UNUSED void string_insert(String *s, int pos, const char *other, int len) {
    if (!other || (*other == '\0') || !len) {
        return;
    }

    if (pos >= (int) s->len) {
        string_append(s, other, len);
        return;
    }

    pos = modulo(pos, s->len);
    if (pos < 0) {
        return;
    }

    const int l_other = strlen(other);

    if (len < 0) {
        len = l_other;
    } else {
        len = min(len, l_other); /* account for if len is larger than the supplied string */
    }

    string_reserve(s, s->len + (size_t) len + 1);
    size_t nBytes = s->len - (size_t) pos;
    memmove(&s->s[pos + len], &s->s[pos], nBytes);
    memcpy(&s->s[pos], other, len);
    s->len += (size_t) len;
    s->s[s->len] = 0;
}


/**
 * Appends "len" characters from the c-string "other" to the end of "s".
 *
 * @param  s      Pointer to string.
 * @param  other  C-string from which characters will be inserted.
 * @param  len    Number of characters from "other" to insert. If this is -1,
 *                  all characters from "other" will be used.
 */
static DS_UNUSED void string_append(String *s, const char *other, int len) {
    if (!other || (*other == '\0') || !len) {
        return;
    }

    const int l_other = strlen(other);

    if (len < 0) {
        len = l_other;
    } else {
        len = min(len, l_other); /* account for if len is larger than the supplied string */
    }

    string_reserve(s, s->len + (size_t) len + 1);
    memcpy(&s->s[s->len], other, len);
    s->len += (size_t) len;
    s->s[s->len] = 0;
}


/**
 * Inserts a format string into "s" before "pos".
 *
 * @param  s       Pointer to string.
 * @param  pos     Index in "s" before which characters will be inserted. If this is STRING_END,
 *                  characters from the format string will be appended to s.
 * @param  format  Format string.
 */
static inline DS_UNUSED void string_printf(String *s, int pos, const char *format, ...) {
    va_list args;
    va_start(args, format);
    __str_printf_va(s, pos, format, args);
    va_end(args);
}


/**
 * Finds the first occurrence of "needle" in "s" starting at "start_pos".
 *
 * @param   s           Pointer to string.
 * @param   start_pos   First index in the string to consider for the search.
 * @param   needle      Substring to find.
 * @param   len_needle  Number of characters to match from needle. If this is -1,
 *                        all characters from "needle" will be used.
 *
 * @return              The index in "s", corresponding to needle[0], where needle was found,
 *                        STRING_NPOS if it was not found, or STRING_ERROR if an error occurred.
 */
static DS_UNUSED int string_find(String *s, int start_pos, const char *needle, int len_needle) {
    if (!needle || !s->len) {
        return STRING_ERROR;
    } else if (*needle == '\0' || !len_needle) {
        return (int) start_pos;
    }

    start_pos = modulo(start_pos, s->len);
    if (start_pos < 0) {
        return STRING_ERROR;
    }

    const int ln = strlen(needle);

    if (len_needle < 0) {
        len_needle = ln;
    } else {
        len_needle = min(len_needle, ln);
    }

    int len_haystack = (int) s->len - start_pos;
    if (len_needle > len_haystack) {
        return STRING_NPOS;
    }

    char *haystack = s->s + start_pos; /* for easier indexing */
    int i = 0;
    int j = 0;
    int *table = __str_create_prefix_table(needle, (size_t) len_needle);
    int res = STRING_NPOS;

    while (i < len_haystack) {
        if (haystack[i] == needle[j]) { /* match */
            i++;
            j++;
        } else {
            if (j != 0) {
                j = table[j - 1];
            } else {
                i++;
            }
        }

        if (j == len_needle) { /* found the substring */
            res = start_pos + (i - j);
            break;
        }
    }
    free(table);
    return res;
}


/**
 * Finds the last occurrence of "needle" in "s" ending at "end_pos".
 *
 * @param   s           Pointer to string.
 * @param   end_pos     Last index in the string to consider for the search.
 * @param   needle      Substring to find.
 * @param   len_needle  Number of characters to match from needle. If this is -1,
 *                        all characters from "needle" will be used.
 *
 * @return              The index in "s", corresponding to needle[0], where needle was found,
 *                        STRING_NPOS if it was not found, or STRING_ERROR if an error occurred.
 */
static DS_UNUSED int string_rfind(String *s, int end_pos, const char *needle, int len_needle) {
    if (!needle || !s->len) {
        return STRING_ERROR;
    } else if (*needle == '\0' || !len_needle) {
        return end_pos;
    }

    end_pos = modulo(end_pos, s->len);
    if (end_pos < 0) {
        return STRING_ERROR;
    }

    const int ln = strlen(needle);

    if (len_needle < 0) {
        len_needle = ln;
    } else if (len_needle > ln) {
        len_needle = min(len_needle, ln);
    }

    int len_haystack = end_pos + 1;
    if (len_needle > len_haystack) {
        return STRING_NPOS;
    }

    char *haystack = s->s;
    int i = end_pos;
    int j = len_needle - 1;
    int *table = __str_create_prefix_table_rev(needle, (size_t) len_needle);
    int res = STRING_NPOS;

    while (i >= 0) {
        if (haystack[i] == needle[j]) { /* found matching character */
            i--;
            j--;
        } else {
            if (j != (len_needle - 1)) {
                j = table[j + 1];
            } else {
                i--;
            }
        }

        if (j == -1) { /* found the substring */
            res = i + 1;
            break;
        }
    }
    free(table);
    return res;
}


/**
 * Finds the first index at or after "pos" where one of the supplied characters was found.
 *
 * @param   s      Pointer to string.
 * @param   pos    First index in the string to consider.
 * @param   chars  C-string of characters to look for.
 *
 * @return         The first index at or after "pos" where one of the supplied characters was
 *                   found, STRING_NPOS if it was not found, or STRING_ERROR if an error occurred.
 */
#define string_find_first_of(s, pos, chars) __str_find_x_of((s), (pos), (chars), true, true)


/**
 * Finds the last index at or before "pos" where one of the supplied characters was found.
 *
 * @param   s      Pointer to string.
 * @param   pos    Last index in the string to consider.
 * @param   chars  C-string of characters to look for.
 *
 * @return         The last index at or before "pos" where one of the supplied characters was
 *                   found, STRING_NPOS if it was not found, or STRING_ERROR if an error occurred.
 */
#define string_find_last_of(s, pos, chars) __str_find_x_of((s), (pos), (chars), false, true)


/**
 * Finds the first index at or after "pos" where one of the supplied characters was not found.
 *
 * @param   s      Pointer to string.
 * @param   pos    First index in the string to consider.
 * @param   chars  C-string of characters to look for.
 *
 * @return         The first index at or after "pos" where a different character was found,
 *                   STRING_NPOS if it was not found, or STRING_ERROR if an error occurred.
 */
#define string_find_first_not_of(s, pos, chars) __str_find_x_of((s), (pos), (chars), true, false)


/**
 * Finds the last index at or before "pos" where one of the supplied characters was not found.
 *
 * @param   s      Pointer to string.
 * @param   pos    Last index in the string to consider.
 * @param   chars  C-string of characters to look for.
 *
 * @return         The last index at or before "pos" where a different character was found,
 *                   STRING_NPOS if it was not found, or STRING_ERROR if an error occurred.
 */
#define string_find_last_not_of(s, pos, chars) __str_find_x_of((s), (pos), (chars), false, false)


/**
 * Creates a substring from "s" with "n" characters, starting at "start" and moving to
 * the next character to include with a step size of "step_size".
 *
 * @param   s          Pointer to string.
 * @param   start      Index where the substring should start.
 * @param   n          Maximum number of characters in the substring. -1 implies to
 *                       include as many elements as the start and step size allow.
 * @param   step_size  How to adjust the index when copying characters. 1 means move forward 1 index
 *                       at a time, -1 means move backwards one index at a time, 2 would mean every
 *                       other index, etc.
 *
 * @return       Newly allocated String, or NULL if an error occurred.
 */
static DS_UNUSED String *string_substr(String *s, int start, int n, int step_size) {
    if (!s->len || !n) {
        return NULL;
    } else if (step_size == 0) {
        step_size = 1;
    }

    start = modulo(start, s->len);
    if (start < 0) {
        return NULL;
    }

    String *sub = string_new(STR_INIT_NONE);
    int end;
    int i;

    if (step_size < 0) {
        end = (n < 0) ? -1 : max(-1, start + (n * step_size));
        for (i = start; i > end; i += step_size) {
            string_push_back(sub, s->s[i]);
        }
    } else {
        end = (n < 0) ? STRING_END(s) : min(STRING_END(s), start + (n * step_size));
        for (i = start; i < end; i += step_size) {
            string_push_back(sub, s->s[i]);
        }
    }
    return sub;
}


/**
 * Splits "s" into substrings based on the provided delimiter and stores them as
 * newly allocated String pointers in an array.
 *
 * @param   s      Pointer to string.
 * @param   delim  The delimiter to use to split the string.
 * @param   n      Pointer to int which will be assigned the size of the array.
 *
 * @return         The array of pointers to String, each of which is a substring of "s", or NULL
 *                   if an error occurred.
 */
static DS_UNUSED String **string_split(String *s, const char *delim, int *n) {
    if (!delim || *delim == '\0' || !s->len) {
        return NULL;
    }

    int len_delim = strlen(delim);
    const char *start = s->s;
    const char *str_end = s->s + s->len;
    int d = 0;

    size_t arr_len = __STR_SPLIT_INITIAL_SIZE;
    String **arr = malloc(arr_len * sizeof(String *));
    if (!arr) {
        DS_OOM();
    }

    memset(arr, 0, sizeof(String *) * arr_len);
    size_t index = 0;
    String *substring = NULL;
    char *end = strstr(start, delim);

    while (end) {
        if (index == arr_len) {
            arr_len <<= 1;
            String **temp = realloc(arr, arr_len * sizeof(String *));
            if (!temp) {
                DS_OOM();
            }

            arr = temp;
            memset(&arr[index], 0, (arr_len - index) * sizeof(String *));
        }

        d = end - start;
        substring = string_new(STR_INIT_NONE);
        string_insert(substring, 0, start, d);
        arr[index++] = substring;
        start = end + len_delim;
        end = strstr(start, delim);
    }

    if (index == arr_len) {
        arr_len <<= 1;
        String **temp = realloc(arr, arr_len * sizeof(String *));
        if (!temp) {
            DS_OOM();
        }

        arr = temp;
        memset(&arr[index], 0, (arr_len - index) * sizeof(String *));
    }

    d = str_end - start;
    substring = string_new(STR_INIT_NONE);
    string_insert(substring, 0, start, d);
    arr[index++] = substring;
    *n = (int) index;
    return arr;    
}


/**
 * Frees the memory allocated by string_split.
 *
 * @param  arr  Array allocated by string_split.
 * @param  n    Size of the array.
 */
static inline DS_UNUSED void string_split_free(String **arr, int n) {
    for (int i = 0; i < n; ++i) {
        string_free(arr[i]);
    }
    free(arr);
}


/**
 * @param   s  C-string.
 *
 * @return     Whether or not all characters in s are alphanumeric.
 */
#define isAlphaNum(s) __str_test_chars_isalnum(s)


/**
 * @param   s  C-string.
 *
 * @return     Whether or not all characters in s are letters.
 */
#define isAlpha(s) __str_test_chars_isalpha(s)


/**
 * @param   s  C-string.
 *
 * @return     Whether or not all characters in s are digits.
 */
#define isDigit(s) __str_test_chars_isdigit(s)


/**
 * Converts all characters in s to lowercase.
 *
 * @param  s  C-string.
 */
#define toLowercase(s) __str_convert_case_tolower(s)


/**
 * Converts all characters in s to uppercase.
 *
 * @param  s  C-string.
 */
#define toUppercase(s) __str_convert_case_toupper(s)


/* --------------------------------------------------------------------------
 * Helper functions
 * -------------------------------------------------------------------------- */
void __str_printf_va(String *s, int pos, const char *format, va_list args) {
    int n = 0;
    va_list args_copy; /* in case vsnprintf doesn't succeed initially */
    /* use temporary buffer, then insert it into the string afterwards */
    size_t buf_size = 256;
    char *buf = malloc(buf_size);

    while (1) {
        va_copy(args_copy, args);
        n = vsnprintf(buf, buf_size, format, args_copy);
        va_end(args_copy);

        if ((n > -1) && ((size_t) n < buf_size)) { /* vsnprintf was successful */
            string_insert(s, pos, buf, n);
            free(buf);
            return;
        } else if (n > -1) { /* buffer was too small */
            buf_size <<= 1;
            char *temp = realloc(buf, buf_size);
            if (!temp) DS_OOM();
            buf = temp;
        } else { /* some error with vsnprintf, stop the function */
            free(buf);
            return;
        }
    }
}

int *__str_create_prefix_table(const char *needle, size_t len) {
    int *table = malloc(sizeof(int) * len);
    if (!table) {
        DS_OOM();
    }

    int cnd = 0; /* needle position */
    int index = 1; /* table position */
    table[0] = 0;

    while (index < (int) len) {
        if (needle[index] == needle[cnd]) { /* matching characters */
            table[index] = cnd + 1;
            cnd++;
            index++;
        } else { /* not a match */
            if (cnd != 0) { /* get previous table index */
                cnd = table[cnd - 1];
            } else { /* set it to 0 (default) */
                table[index] = 0;
                index++;
            }
        }
    }
    return table;
}

int *__str_create_prefix_table_rev(const char *needle, size_t len) {
    int *table = malloc(sizeof(int) * len);
    if (!table) {
        DS_OOM();
    }

    int cnd = len - 1; /* needle position */
    int index = cnd - 1; /* table position */
    table[cnd] = cnd;

    while (index >= 0) {
        if (needle[index] == needle[cnd]) { /* matching characters */
            table[index] = cnd - 1;
            cnd--;
            index--;
        } else { /* not a match */
            if (cnd != (int) (len - 1)) { /* get previous table index */
                cnd = table[cnd + 1];
            } else { /* set it to 0 (default) */
                table[index] = (int) len - 1;
                index--;
            }
        }
    }
    return table;
}

int __str_find_x_of(String *s, int pos, const char *chars, bool first, bool match) {
    if (!s->len || !chars) {
        return STRING_NPOS;
    } else if (*chars == '\0') {
        return pos;
    }

    pos = modulo(pos, s->len);
    if (pos < 0) {
        return STRING_ERROR;
    }

    const char *c;
    int i = pos;

    if (match) {
        for (c = chars; *c; ++c) {
            if (s->s[i] == *c) return i;
        }
        if (first) {
            for (++i; i < (int) s->len; ++i) {
                /* if the previous character didn't match and this is the same, there won't be a match */
                if (s->s[i] == s->s[i - 1]) continue;
                for (c = chars; *c; ++c) {
                    if (s->s[i] == *c) return i;
                }
            }
        } else {
            for (--i; i >= 0; --i) {
                if (s->s[i] == s->s[i + 1]) continue;
                for (c = chars; *c; ++c) {
                    if (s->s[i] == *c) return i;
                }
            }
        }
    } else {
        for (c = chars; *c; ++c) {
            if (s->s[i] == *c) break;
        }
        /* if *c is 0, we must have iterated through all of the characters */
        if (!(*c)) return i;
        if (first) {
            for (++i; i < (int) s->len; ++i) {
                if (s->s[i] == s->s[i - 1]) continue;
                for (c = chars; *c; ++c) {
                    if (s->s[i] == *c) break;
                }
                if (!(*c)) return i;
            }
        } else {
            for (--i; i >= 0; --i) {
                if (s->s[i] == s->s[i + 1]) continue;
                for (c = chars; *c; ++c) {
                    if (s->s[i] == *c) break;
                }
                if (!(*c)) return i;
            }   
        }
    }
    return STRING_NPOS;
}

#endif /* STR_H */
