#include "defaults.h"
#include "str.h"

#define SPLIT_INITIAL_SIZE 8
#define INITIAL_STR_CAP 64

void _str_printf_va(String *s, size_t pos, const char *format, va_list args);
int *create_prefix_table(const char *needle, size_t len);
int *create_prefix_table_rev(const char *needle, size_t len);

String *string_new(void) {
    String *s = malloc(sizeof(String));
    if (!s) {
        DS_OOM();
    }
    memset(s, 0, sizeof(String));
    string_reserve(s, INITIAL_STR_CAP);
    s->s[0] = 0;
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
    while (val < n) { // double the capacity from what it was before
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
    if (n == s->len) { // nothing to do
        return;
    }

    if (n > s->len) {
        string_reserve(s, n + 1);
        memset(&s->s[s->len], c, n - s->len);
        s->s[n] = 0;
    }
    s->s[n] = 0;
    s->len = n;
}

void string_clear(String *s) {
    if (!(s->len)) {
        return;
    }
    s->s[0] = 0;
    s->len = 0;
}

void string_erase(String *s, size_t start, size_t n) {
    if (start >= s->len || n == 0) {
        return;
    }

    size_t end = min(s->len, start + n);
    n = end - start;
    
    if (end < s->len) { // move any characters after end to start
        memmove(&s->s[start], &s->s[end], s->len - end);
    }
    s->len -= n;
    s->s[s->len] = 0;
}

void string_shrink_to_fit(String *s) {
    if (s->len + 1 == s->cap || s->len == 0) {
        return;
    }
    char *tmp = realloc(s->s, s->len + 1); // realloc only enough space for string and '\0'
    if (!tmp) {
        DS_OOM();
    }
    s->cap = s->len + 1;
    s->s = tmp;
}

void string_push_back(String *s, char c) {
    if (c == 0) { // doesn't make sense to push back a null character
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

void string_replace(String *s, size_t pos, const char *other, size_t len) {
    if (!len) {
        return;
    }
    len = min(len, strlen(other)); // account for if len is larger than the supplied string

    if (pos >= s->len) { // assume that an out-of-bounds index means append
        string_append(s, other, len);
        return;
    }

    string_reserve(s, pos + len + 1);
    memcpy(&s->s[pos], other, len);

    if (pos + len >= s->len) {
        s->len = pos + len;
        s->s[s->len] = 0;
    }
}

void string_insert(String *s, size_t pos, const char *other, size_t len) {
    if (!len) {
        return;
    }
    len = min(len, strlen(other)); // account for if len is larger than the supplied string

    if (pos >= s->len) { // assume that an out-of-bounds index means append
        string_append(s, other, len);
        return;
    }

    string_reserve(s, len + s->len + 1);
    size_t nBytes = s->len - pos;
    // move characters that were at pos to after the inserted portion
    memmove(&s->s[pos + len], &s->s[pos], nBytes);
    memcpy(&s->s[pos], other, len);
    s->len += len;
    s->s[s->len] = 0;
}

void string_append(String *s, const char *other, size_t len) {
    if (!len) {
        return;
    }
    len = min(len, strlen(other)); // account for if len is larger than the supplied string
    string_reserve(s, s->len + len + 1);
    memcpy(&s->s[s->len], other, len);
    s->len += len;
    s->s[s->len] = 0;
}

void string_printf(String *s, size_t pos, const char *format, ...) {
    va_list args;
    va_start(args, format);
    _str_printf_va(s, pos, format, args);
    va_end(args);
}

void _str_printf_va(String *s, size_t pos, const char *format, va_list args) {
    int n = 0;
    va_list cp_args;
    size_t buf_size = 256;
    char *buf = malloc(buf_size); // use temporary buffer, then insert it into the string afterwards

    while (1) {
        va_copy(cp_args, args);
        n = vsnprintf(buf, buf_size, format, cp_args);
        va_end(cp_args);

        if ((n > -1) && ((size_t) n < buf_size)) { // vsnprintf was successful
            string_insert(s, pos, buf, (size_t) n);
            free(buf);
            return;
        } else if (n > -1) { // buffer was too small
            buf_size <<= 1;
            char *temp = realloc(buf, buf_size);
            if (!temp) DS_OOM();
            buf = temp;
        } else { // some error with vsnprintf, stop the function
            free(buf);
            return;
        }
    }
}

int string_find(String *s, size_t start_pos, const char *needle, size_t len_needle) {
    if (start_pos >= s->len || !needle) {
        return -1;
    }
    
    len_needle = min(len_needle, strlen(needle));
    if (!len_needle) {
        return start_pos;
    }

    int len_haystack = s->len - start_pos;
    if (len_needle > len_haystack) {
        return -1;
    }

    char *haystack = s->s + start_pos; // for easier indexing
    int i = 0;
    int j = 0;
    int *table = create_prefix_table(needle, len_needle);
    int res = -1;

    while (i < len_haystack) {
        if (haystack[i] == needle[j]) { // match
            i++;
            j++;
        } else {
            if (j != 0) {
                j = table[j - 1];
            } else {
                i++;
            }
        }

        if (j == (int) len_needle) { // found the substring
            res = (int) start_pos + (i - j);
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

    int cnd = 0; // needle position
    int index = 1; // table position
    table[0] = 0;

    while (index < (int) len) {
        if (needle[index] == needle[cnd]) { // matching characters
            table[index] = cnd + 1;
            cnd++;
            index++;
        } else { // not a match
            if (cnd != 0) { // get previous table index
                cnd = table[cnd - 1];
            } else { // set it to 0 (default)
                table[index] = 0;
                index++;
            }
        }
    }
    return table;
}

int string_rfind(String *s, size_t end_pos, const char *needle, size_t len_needle) {
    if (!end_pos || !needle) {
        return -1;
    }

    end_pos = min(s->len - 1, end_pos);
    len_needle = min(len_needle, strlen(needle));
    if (!len_needle) {
        return end_pos;
    }

    size_t len_haystack = end_pos + 1;
    if (len_needle > len_haystack) {
        return -1;
    }

    char *haystack = s->s;
    int i = (int) end_pos;
    int j = (int) len_needle - 1;
    int *table = create_prefix_table_rev(needle, len_needle);
    int res = -1;

    while (i >= 0) {
        if (haystack[i] == needle[j]) { // found matching character
            i--;
            j--;
        } else {
            if (j != ((int) len_needle - 1)) {
                j = table[j + 1];
            } else {
                i--;
            }
        }

        if (j == -1) { // found the substring
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

    int cnd = len - 1; // needle position
    int index = cnd - 1; // table position
    table[cnd] = cnd;

    while (index >= 0) {
        if (needle[index] == needle[cnd]) { // matching characters
            table[index] = cnd - 1;
            cnd--;
            index--;
        } else { // not a match
            if (cnd != (int) (len - 1)) { // get previous table index
                cnd = table[cnd + 1];
            } else { // set it to 0 (default)
                table[index] = (int) len - 1;
                index--;
            }
        }
    }
    return table;
}

int string_find_first_of(String *s, size_t pos, const char *chars) {
    if (pos >= s->len || !chars) {
        return -1;
    } else if (*chars == '\0') {
        return pos;
    }

    const char *c;
    int i = (int) pos;

    for (c = chars; *c; ++c) {
        if (s->s[i] == *c) {
            return i;
        }
    }

    for (++i; i < (int) s->len; ++i) {
        // if the previous character didn't match and this is the same, there won't be a match
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
    if (!chars) {
        return -1;
    } else if (pos < 0) {
        return -1;
    }

    pos = min(pos, s->len - 1);
    if (*chars == '\0') {
        return pos;
    }

    const char *c;
    int i = (int) pos;

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

int string_find_first_not_of(String *s, size_t pos, const char *chars) {
    if (pos >= s->len || !chars) {
        return -1;
    } else if (*chars == '\0') {
        return pos;
    }

    const char *c;
    int i = (int) pos;

    for (c = chars; *c; ++c) {
        if (s->s[i] == *c) {
            break;
        }
    }

    if (!(*c)) { // if *c is 0, we must have iterated through all of the characters
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
    if (!chars) {
        return -1;
    } else if (pos < 0) {
        return -1;
    }

    pos = min(pos, s->len - 1);
    if (*chars == '\0') {
        return pos;
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

char *string_substr(String *s, size_t pos, int len) {
    if (pos >= s->len) {
        pos = s->len;
    }

    len = (len < 0) ? (s->len - pos) : len;
    size_t end = min(s->len, pos + (size_t) len);
    size_t nChars = end - pos;

    char *substr = malloc(nChars + 1);
    if (!substr) {
        DS_OOM();
    }

    if (nChars) {
        strncpy(substr, &(s->s[pos]), nChars);
    }
    substr[nChars] = 0;
    return substr;
}

char **str_split(const char *str, const char *delim, int *n) {
    if (!delim || *delim == '\0') {
        return NULL;
    }

    int len_delim = strlen(delim);
    const char *start = str;
    const char *str_end = str + strlen(str);

    size_t d = 0;

    size_t arr_len = SPLIT_INITIAL_SIZE;
    char **arr = malloc(arr_len * sizeof(char*));
    if (!arr) DS_OOM();

    memset(arr, 0, sizeof(char*) * arr_len);
    size_t index = 0;
    char *substring = NULL;
    char *end = strstr(start, delim);
    while (end) {
        if (index == arr_len) {
            arr_len <<= 1;
            char **temp = realloc(arr, arr_len * sizeof(char*));
            if (!temp) {
                DS_OOM();
            }

            arr = temp;
            memset(&arr[index], 0, (arr_len - index) * sizeof(char*));
        }

        d = end - start;
        substring = malloc(d + 1);
        if (!substring) {
            DS_OOM();
        }

        if (d) {
            strncpy(substring, start, d);
        }
        substring[d] = 0;
        arr[index++] = substring;
        start = end + len_delim;
        end = strstr(start, delim);
    }

    if (index == arr_len) {
            arr_len <<= 1;
            char **temp = realloc(arr, arr_len * sizeof(char*));
            if (!temp) {
                DS_OOM();
            }

            arr = temp;
            memset(&arr[index], 0, (arr_len - index) * sizeof(char*));
    }

    d = str_end - start;
    substring = malloc(d + 1);
    if (!substring) {
        DS_OOM();
    }

    if (d) {
        strncpy(substring, start, d);
    }
    substring[d] = 0;
    arr[index++] = substring;
    *n = (int) index;
    return arr;    
}

void str_split_free(char **arr, int n) {
    for (int i = 0; i < n; ++i) {
        free(arr[i]);
    }
    free(arr);
}

bool isAlphaNum(const char *s) {
    for (const char *c = s; *c; ++c) {
        if (!isalnum(*c)) return false;
    }
    return true;
}

bool isAlpha(const char *s) {
    for (const char *c = s; *c; ++c) {
        if (!isalpha(*c)) return false;
    }
    return true;
}

bool isDigit(const char *s) {
    for (const char *c = s; *c; ++c) {
        if (!isdigit(*c)) return false;
    }
    return true;
}

void toLowercase(char *s) {
    for (char *c = s; *c; ++c) {
        *c = tolower(*c);
    }
}

void toUppercase(char *s) {
    for (char *c = s; *c; ++c) {
        *c = toupper(*c);
    }
}
