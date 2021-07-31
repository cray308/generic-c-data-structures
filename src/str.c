#include "str.h"
#include <limits.h>

int *str_gen_prefix_table(char const *needle, int len) {
    int *table;
    int i = 1; /* table position */
    int j = 0; /* needle position */
    table = malloc(sizeof(int) * (unsigned) len);
    if (!table) return NULL;
    table[0] = 0;

    while (i < len) {
        if (needle[i] == needle[j]) { /* matching characters */
            table[i++] = ++j;
        } else if (j != 0) { /* get previous table index */
            j = table[j - 1];
        } else { /* set it to 0 (default) */
            table[i++] = 0;
        }
    }
    return table;
}

void string_reserve(String *this, unsigned n) {
    unsigned val;
    char *tmp;
    if (n <= this->cap) return;

    val = this->cap;
    while (val < n) val <<= 1;
    tmp = realloc(this->s, val);
    if (!tmp) exit(1);
    this->s = tmp;
    this->cap = val;
}

void string_replace(String *this, unsigned pos, int nToReplace, const char *s, int len) {
    unsigned end, l = (unsigned) len, n = this->size - pos;
    if (!(s && *s && len) || pos > this->size) return;

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
    if (subpos >= other->size || !len) return;

    if (len >= 0) l = min(l, (unsigned) len);
    string_replace(this, pos, nToReplace, &other->s[subpos], (int) l);
}

void string_replace_repeatingChar(String *this, unsigned pos, int nToReplace, unsigned n, char c) {
    unsigned end, n2r = this->size - pos;
    if (!n || pos > this->size) return;

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
    String *t = malloc(sizeof(String));
    if (!t) return NULL;
    t->s = malloc(64);
    if (!t->s) {
        free(t);
        return NULL;
    }
    t->size = 0;
    t->cap = 64;
    t->s[0] = 0;
    string_append(t, s, n);
    return t;
}

String *string_new_fromString(const String *other, unsigned pos, int n) {
    String *s = string_new();
    if (s) string_append_fromString(s, other, pos, n);
    return s;
}

String *string_new_repeatingChar(unsigned n, char c) {
    String *s = string_new();
    if (s) string_append_repeatingChar(s, n, c);
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
    if (start >= this->size || !n) return;

    if (n >= 0) n2d = min(n2d, (unsigned) n);

    if ((end = start + n2d) < this->size) {
        memmove(&this->s[start], &this->s[end], this->size - end);
    }
    this->size -= n2d;
    this->s[this->size] = 0;
}

void string_shrink_to_fit(String *this) {
    char *tmp;
    if (!this->size || this->cap <= 64 || this->size + 1 == this->cap) return;
    tmp = realloc(this->s, this->size + 1);
    if (!tmp) return;
    this->cap = this->size + 1;
    this->s = tmp;
}

int string_find_first_of(String *this, unsigned pos, const char *chars, int n) {
    char const *c, *end;
    if (pos >= this->size || !chars) return STRING_ERROR;
    else if (!(*chars && n)) return (int) pos;
    end = (n < 0) ? &chars[strlen(chars)] : &chars[n];

    for (c = chars; c != end; ++c) {
        if (this->s[pos] == *c) return (int) pos;
    }

    for (++pos; pos < this->size; ++pos) {
        if (this->s[pos] == this->s[pos-1]) continue;
        for (c = chars; c != end; ++c) {
            if (this->s[pos] == *c) return (int) pos;
        }
    }
    return STRING_NPOS;
}

int string_find_last_of(String *this, unsigned pos, const char *chars, int n) {
    char const *c, *end;
    if (pos >= this->size || !chars) return STRING_ERROR;
    else if (!(*chars && n)) return (int) pos;
    end = (n < 0) ? &chars[strlen(chars)] : &chars[n];

    for (c = chars; c != end; ++c) {
        if (this->s[pos] == *c) return (int) pos;
    }

    for (--pos; pos != UINT_MAX; --pos) {
        if (this->s[pos] == this->s[pos + 1]) continue;
        for (c = chars; c != end; ++c) {
            if (this->s[pos] == *c) return (int) pos;
        }
    }
    return STRING_NPOS;
}

int string_find_first_not_of(String *this, unsigned pos, const char *chars, int n) {
    char const *c, *end;
    if (pos >= this->size || !chars) return STRING_ERROR;
    else if (!(*chars && n)) return (int) pos;
    end = (n < 0) ? &chars[strlen(chars)] : &chars[n];

    for (c = chars; c != end; ++c) {
        if (this->s[pos] == *c) break;
    }
    if (c == end) return (int) pos;
    for (++pos; pos < this->size; ++pos) {
        if (this->s[pos] == this->s[pos-1]) continue;
        for (c = chars; c != end; ++c) {
            if (this->s[pos] == *c) break;
        }
        if (c == end) return (int) pos;
    }
    return STRING_NPOS;
}

int string_find_last_not_of(String *this, unsigned pos, const char *chars, int n) {
    char const *c, *end;
    if (pos >= this->size || !chars) return STRING_ERROR;
    else if (!(*chars && n)) return (int) pos;
    end = (n < 0) ? &chars[strlen(chars)] : &chars[n];

    for (c = chars; c != end; ++c) {
        if (this->s[pos] == *c) break;
    }
    if (c == end) return (int) pos;
    for (--pos; pos != UINT_MAX; --pos) {
        if (this->s[pos] == this->s[pos+1]) continue;
        for (c = chars; c != end; ++c) {
            if (this->s[pos] == *c) break;
        }
        if (c == end) return (int) pos;
    }
    return STRING_NPOS;
}

int string_find(String *this, unsigned start_pos, const char *needle, int len_needle) {
    const int len_haystack = (int) (this->size - start_pos);
    int res = STRING_NPOS, i = 0, j = 0;
    int *table;
    char *haystack;
    if (start_pos >= this->size || !needle) return STRING_ERROR;
    else if (!(*needle && len_needle)) return (int) start_pos;

    if (len_needle < 0) len_needle = (int) strlen(needle);

    if (len_needle > len_haystack) return STRING_NPOS;
    else if (len_needle == 1) return string_find_first_of(this, start_pos, needle, 1);

    haystack = this->s + start_pos;
    table = str_gen_prefix_table(needle, len_needle);
    if (!table) return STRING_ERROR;

    while (i < len_haystack) {
        if (haystack[i] == needle[j]) {
            ++i;
            ++j;
        } else if (j != 0) {
            j = table[j - 1];
        } else {
            ++i;
        }

        if (j == len_needle) {
            res = (int) start_pos + (i - j);
            break;
        }
    }
    free(table);
    return res;
}

int string_rfind(String *this, unsigned end_pos, const char *needle, int len_needle) {
    int res = STRING_NPOS, i, j, minIndex;
    int *table;
    char *haystack = this->s;
    if (end_pos >= this->size || !needle) return STRING_ERROR;
    else if (!(*needle && len_needle)) return (int) end_pos;

    if (len_needle < 0) len_needle = (int) strlen(needle);

    if (len_needle > (int) end_pos + 1) return STRING_NPOS;
    else if (len_needle == 1) return string_find_last_of(this, end_pos, needle, 1);

    minIndex = len_needle - 1, j = minIndex, i = j - 1;
    table = malloc(sizeof(int) * (unsigned) len_needle);
    if (!table) return STRING_ERROR;
    table[minIndex] = minIndex;

    while (i > -1) {
        if (needle[i] == needle[j]) {
            table[i--] = --j;
        } else if (j != minIndex) {
            j = table[j + 1];
        } else {
            table[i--] = minIndex;
        }
    }
    
    i = (int) end_pos, j = len_needle - 1;
    while (i > -1) {
        if (haystack[i] == needle[j]) {
            --i;
            --j;
        } else if (j != minIndex) {
            j = table[j + 1];
        } else {
            --i;
        }

        if (j == -1) {
            res = i + 1;
            break;
        }
    }
    free(table);
    return res;
}

String *string_substr(String *this, unsigned start, int n, int step_size) {
    String *sub;
    int i = (int) start;
    if (start >= this->size || !n) return NULL;

    if (!step_size) step_size = 1;

    sub = string_new();
    if (!sub) return NULL;

    if (step_size < 0) {
        int end = (n < 0) ? -1 : max(-1, i + (n * step_size));
        for (; i > end; i += step_size) {
            string_push_back(sub, this->s[i]);
        }
    } else {
        int end = (n < 0) ? (int) this->size : min((int) this->size, i + (n * step_size));
        for (; i < end; i += step_size) {
            string_push_back(sub, this->s[i]);
        }
    }
    return sub;
}

String **string_split(String *this, const char *delim) {
    const int iEnd = (int) this->size;
    int len_delim, i = 0, j = 0;
    unsigned arrIdx = 0, arrLen = 8;
    int *positions, *table;
    char *haystack = this->s;
    String **arr;
    String *substring = NULL;
    if (!(delim && *delim && this->size)) return NULL;

    len_delim = (int) strlen(delim);
    positions = calloc(8, sizeof(int));
    if (!positions) return NULL;

    table = str_gen_prefix_table(delim, len_delim);
    if (!table) {
        free(positions);
        return NULL;
    }

    while (i < iEnd) {
        if (haystack[i] == delim[j]) {
            ++i;
            ++j;
        } else if (j != 0) {
            j = table[j - 1];
        } else {
            ++i;
        }

        if (j == len_delim) {
            if (arrIdx == arrLen) {
                int *temp;
                arrLen <<= 1;
                temp = realloc(positions, arrLen * sizeof(int));
                if (!temp) {
                    free(positions);
                    free(table);
                    return NULL;
                }
                positions = temp;
                memset(&positions[arrIdx + 1], 0, (arrLen - arrIdx + 1) * sizeof(int));
            }
            positions[arrIdx++] = i - j;
            j = table[j - 1];
        }
    }

    if (arrIdx == arrLen) {
        int *temp;
        temp = realloc(positions, (arrLen + 1) * sizeof(int));
        if (!temp) {
            free(positions);
            free(table);
            return NULL;
        }
        positions = temp;
    }
    positions[arrIdx++] = -1;
    arrLen = arrIdx;

    arr = malloc((arrLen + 1) * sizeof(String *));
    if (!arr) {
        free(positions);
        free(table);
        return NULL;
    }

    arrIdx = 0, i = 0, j = positions[0];

    while (j != -1) {
        substring = string_new();
        if (!substring) {
            free(positions);
            free(table);
            return NULL;
        }
        string_insert(substring, 0, &haystack[i], j - i);
        arr[arrIdx++] = substring;
        i = j + len_delim;
        j = positions[arrIdx];
    }

    substring = string_new();
    if (!substring) {
        free(positions);
        free(table);
        return NULL;
    }
    string_insert(substring, 0, &haystack[i], iEnd - i);
    arr[arrIdx++] = substring;
    arr[arrLen] = NULL;
    free(positions);
    free(table);
    return arr;
}

#if __STDC_VERSION__ >= 199901L
#include <stdarg.h>
#include <stdio.h>

char *str_read_format(int *n, const char *format, va_list args) {
    va_list localArgs;
    int _n = 0, buf_size = 256;
    char *temp;
    char *buf = malloc(256);
    if (!buf) return NULL;

    va_copy(localArgs, args);

    _n = vsnprintf(buf, 256, format, args);
    if (_n < 0) {
        free(buf);
        return NULL;
    } else if (_n < buf_size) {
        *n = _n;
        return buf;
    }

    buf_size = _n + 1;
    temp = realloc(buf, (size_t) buf_size);
    if (!temp) {
        free(buf);
        return NULL;
    }
    buf = temp;

    _n = vsnprintf(buf, (size_t) buf_size, format, localArgs);
    if (_n < 0 || _n >= buf_size) {
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
    result = str_read_format(&n, format, args);
    va_end(args);
    if (!result) return;
    string_replace(this, pos, nToReplace, result, n);
    free(result);
}

String *string_new_withFormat(const char *format, ...) {
    va_list args;
    int n = 0;
    char *result;
    String *s = string_new();
    if (!s) return NULL;
    va_start(args, format);
    result = str_read_format(&n, format, args);
    va_end(args);
    if (!result) return s;
    string_append(s, result, n);
    free(result);
    return s;
}
#endif /* __STDC_VERSION__ >= 199901L */
