#include "defaults.h"
#include "str.h"
#include <ctype.h>

#define SPLIT_INITIAL_SIZE 8
#define INITIAL_STR_CAP 64

void _str_printf_va(String *s, int pos, const char *format, va_list args);
int *create_prefix_table(const char *needle, size_t len);
int *create_prefix_table_rev(const char *needle, size_t len);

String *string_new(StringInitializer init, ...) {
    String *s = malloc(sizeof(String));
    if (!s) {
        DS_OOM();
    }
    memset(s, 0, sizeof(String));
    string_reserve(s, INITIAL_STR_CAP);

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

void string_free(String *s) {
    if (s->cap) {
        free(s->s);
    }
    free(s);
}

void string_reserve(String *s, size_t n) {
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

void string_resize(String *s, size_t n, char c) {
    if (n > s->len) {
        string_reserve(s, n + 1);
        memset(&s->s[s->len], c, n - s->len);
        s->s[n] = 0;
    }
    s->s[n] = 0;
    s->len = n;
}

void string_clear(String *s) {
    if (!(s->s)) {
        return;
    }
    s->s[0] = 0;
    s->len = 0;
}

void string_erase(String *s, int start, int n) {
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
    
    if (end < s->len) { /* move any characters after end to start */
        memmove(&s->s[start], &s->s[end], s->len - (size_t) end);
    }
    s->len -= (size_t) n;
    s->s[s->len] = 0;
}

void string_shrink_to_fit(String *s) {
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

void string_push_back(String *s, char c) {
    if (c == 0) { /* doesn't make sense to push back a null character */
        return;
    }
    string_reserve(s, s->len + 1);
    s->s[s->len++] = c;
    s->s[s->len] = 0;
}

void string_pop_back(String *s) {
    if (!s->len) {
        return;
    }
    s->s[s->len-- - 1] = 0;
}

void string_replace(String *s, int pos, const char *other, int len) {
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

void string_insert(String *s, int pos, const char *other, int len) {
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

void string_append(String *s, const char *other, int len) {
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

void string_printf(String *s, int pos, const char *format, ...) {
    va_list args;
    va_start(args, format);
    _str_printf_va(s, pos, format, args);
    va_end(args);
}

void _str_printf_va(String *s, int pos, const char *format, va_list args) {
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

int string_find(String *s, int start_pos, const char *needle, int len_needle) {
    if (!needle || !s->len) {
        return -1;
    } else if (*needle == '\0' || !len_needle) {
        return (int) start_pos;
    }

    start_pos = modulo(start_pos, s->len);
    if (start_pos < 0) {
        return -1;
    }

    const int ln = strlen(needle);

    if (len_needle < 0) {
        len_needle = ln;
    } else {
        len_needle = min(len_needle, ln);
    }

    int len_haystack = (int) s->len - start_pos;
    if (len_needle > len_haystack) {
        return -1;
    }

    char *haystack = s->s + start_pos; /* for easier indexing */
    int i = 0;
    int j = 0;
    int *table = create_prefix_table(needle, (size_t) len_needle);
    int res = -1;

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

int *create_prefix_table(const char *needle, size_t len) {
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

int string_rfind(String *s, int end_pos, const char *needle, int len_needle) {
    if (!needle || !s->len) {
        return -1;
    } else if (*needle == '\0' || !len_needle) {
        return end_pos;
    }

    end_pos = modulo(end_pos, s->len);
    if (end_pos < 0) {
        return -1;
    }

    const int ln = strlen(needle);

    if (len_needle < 0) {
        len_needle = ln;
    } else if (len_needle > ln) {
        len_needle = min(len_needle, ln);
    }

    int len_haystack = end_pos + 1;
    if (len_needle > len_haystack) {
        return -1;
    }

    char *haystack = s->s;
    int i = end_pos;
    int j = len_needle - 1;
    int *table = create_prefix_table_rev(needle, (size_t) len_needle);
    int res = -1;

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

int *create_prefix_table_rev(const char *needle, size_t len) {
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

int string_find_first_of(String *s, int pos, const char *chars) {
    if (!s->len || !chars) {
        return -1;
    } else if (*chars == '\0') {
        return pos;
    }

    pos = modulo(pos, s->len);
    if (pos < 0) {
        return -1;
    }

    const char *c;
    int i = pos;

    for (c = chars; *c; ++c) {
        if (s->s[i] == *c) {
            return i;
        }
    }

    for (++i; i < (int) s->len; ++i) {
        /* if the previous character didn't match and this is the same, there won't be a match */
        if (s->s[i] == s->s[i - 1]) {
            continue;
        }

        for (c = chars; *c; ++c) {
            if (s->s[i] == *c) {
                return i;
            }
        }
    }
    return -1;
}

int string_find_last_of(String *s, int pos, const char *chars) {
    if (!s->len || !chars) {
        return -1;
    } else if (*chars == '\0') {
        return pos;
    }

    pos = modulo(pos, s->len);
    if (pos < 0) {
        return -1;
    }

    const char *c;
    int i = pos;

    for (c = chars; *c; ++c) {
        if (s->s[i] == *c) {
            return i;
        }
    }

    for (--i; i >= 0; --i) {
        if (s->s[i] == s->s[i + 1]) {
            continue;
        }

        for (c = chars; *c; ++c) {
            if (s->s[i] == *c) {
                return i;
            }
        }
    }
    return -1;
}

int string_find_first_not_of(String *s, int pos, const char *chars) {
    if (!s->len || !chars) {
        return -1;
    } else if (*chars == '\0') {
        return pos;
    }

    pos = modulo(pos, s->len);
    if (pos < 0) {
        return -1;
    }

    const char *c;
    int i = pos;

    for (c = chars; *c; ++c) {
        if (s->s[i] == *c) {
            break;
        }
    }

    if (!(*c)) { /* if *c is 0, we must have iterated through all of the characters */
        return i;
    }

    for (++i; i < (int) s->len; ++i) {
        if (s->s[i] == s->s[i - 1]) {
            continue;
        }

        for (c = chars; *c; ++c) {
            if (s->s[i] == *c) {
                break;
            }
        }

        if (!(*c)) {
            return i;
        }
    }
    return -1;
}

int string_find_last_not_of(String *s, int pos, const char *chars) {
    if (!s->len || !chars) {
        return -1;
    } else if (*chars == '\0') {
        return pos;
    }

    pos = modulo(pos, s->len);
    if (pos < 0) {
        return -1;
    }

    const char *c;
    int i = (int) pos;

    for (c = chars; *c; ++c) {
        if (s->s[i] == *c) {
            break;
        }
    }

    if (!(*c)) {
        return i;
    }

    for (--i; i >= 0; --i) {
        if (s->s[i] == s->s[i + 1]) {
            continue;
        }

        for (c = chars; *c; ++c) {
            if (s->s[i] == *c) {
                break;
            }
        }

        if (!(*c)) {
            return i;
        }
    }
    return -1;
}

String *string_substr(String *s, int start, int n, int step_size) {
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
    } else {
        end = (n < 0) ? STRING_END(s) : min(STRING_END(s), start + (n * step_size));
    }

    if (step_size < 0) {
        for (i = start; i > end; i += step_size) {
            string_push_back(sub, s->s[i]);
        }
    } else {
        for (i = start; i < end; i += step_size) {
            string_push_back(sub, s->s[i]);
        }
    }
    return sub;
}

String **string_split(String *s, const char *delim, int *n) {
    if (!delim || *delim == '\0' || !s->len) {
        return NULL;
    }

    int len_delim = strlen(delim);
    const char *start = s->s;
    const char *str_end = s->s + s->len;
    int d = 0;

    size_t arr_len = SPLIT_INITIAL_SIZE;
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

void string_split_free(String **arr, int n) {
    int i;
    for (i = 0; i < n; ++i) {
        string_free(arr[i]);
    }
    free(arr);
}

int isAlphaNum(const char *s) {
    const char *c;
    for (c = s; *c; ++c) {
        if (!isalnum(*c)) return 0;
    }
    return 1;
}

int isAlpha(const char *s) {
    const char *c;
    for (c = s; *c; ++c) {
        if (!isalpha(*c)) return 0;
    }
    return 1;
}

int isDigit(const char *s) {
    const char *c;
    for (c = s; *c; ++c) {
        if (!isdigit(*c)) return 0;
    }
    return 1;
}

void toLowercase(char *s) {
    char *c;
    for (c = s; *c; ++c) {
        *c = tolower(*c);
    }
}

void toUppercase(char *s) {
    char *c;
    for (c = s; *c; ++c) {
        *c = toupper(*c);
    }
}
