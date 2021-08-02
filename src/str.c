#include "str.h"
#include <limits.h>

unsigned *str_gen_prefix_table(char const *needle, unsigned len) {
    unsigned *table = malloc(sizeof(unsigned) * len);
    unsigned i = 1; /* table position */
    unsigned j = 0; /* needle position */
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

unsigned char string_reserve(String *this, unsigned n) {
    unsigned ncap = this->cap;
    char *tmp;
    if (n <= ncap) return 1;
    else if (ncap == 2147483648) return 0;
    else if (n < 1073741824) {
        while (ncap < n) ncap <<= 1;
    } else {
        ncap = 2147483648;
    }

    tmp = realloc(this->s, ncap);
    if (!tmp) return 0;
    this->cap = ncap;
    this->s = tmp;
    return 1;
}

unsigned char string_replace(String *this, unsigned pos, unsigned nToReplace, const char *s, unsigned len) {
    unsigned end, n = this->size - pos;
    if (!(s && *s && len) || pos > this->size) return 1;

    if (len == STRING_NOT_APPLICABLE) len = (unsigned) strlen(s);
    if (nToReplace != STRING_NOT_APPLICABLE) n = min(n, nToReplace);

    if (!string_reserve(this, this->size + len + 1)) return 0;
    if ((end = pos + n) < this->size) {
        memmove(&this->s[pos + len], &this->s[end], this->size - end);
    }
    memcpy(&this->s[pos], s, len);

    if (len < n) {
        this->size -= (n - len);
    } else {
        this->size += (len - n);
    }
    this->s[this->size] = 0;
    return 1;
}

unsigned char string_replace_fromString(String *this, unsigned pos, unsigned nToReplace, const String *other, unsigned subpos, unsigned len) {
    unsigned l = other->size - subpos;
    if (subpos >= other->size || !len) return 1;

    if (len != STRING_NOT_APPLICABLE) l = min(l, len);
    return string_replace(this, pos, nToReplace, &other->s[subpos], l);
}

unsigned char string_replace_repeatingChar(String *this, unsigned pos, unsigned nToReplace, unsigned n, char c) {
    unsigned end, n2r = this->size - pos;
    if (!n || pos > this->size) return 1;

    if (nToReplace != STRING_NOT_APPLICABLE) n2r = min(n2r, nToReplace);

    if (!string_reserve(this, this->size + n + 1)) return 0;
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
    return 1;
}

String *string_new_fromCStr(const char *s, unsigned n) {
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

String *string_new_fromString(const String *other, unsigned pos, unsigned n) {
    String *s = string_new();
    if (s) string_append_fromString(s, other, pos, n);
    return s;
}

String *string_new_repeatingChar(unsigned n, char c) {
    String *s = string_new();
    if (s) string_append_repeatingChar(s, n, c);
    return s;
}

unsigned char string_resize_usingChar(String *this, unsigned n, char c) {
    if (n > this->size) {
        if (!string_reserve(this, n + 1)) return 0;
        memset(&this->s[this->size], c, n - this->size);
    }
    this->s[n] = 0;
    this->size = n;
    return 1;
}

void string_erase(String *this, unsigned start, unsigned n) {
    unsigned end, n2d = this->size - start;
    if (start >= this->size || !n) return;

    if (n != STRING_NOT_APPLICABLE) n2d = min(n2d, n);

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

unsigned string_find_first_of(String *this, unsigned pos, const char *chars, unsigned n) {
    char const *c, *end;
    if (pos >= this->size || !chars) return STRING_ERROR;
    else if (!(*chars && n)) return pos;
    end = (n == STRING_NOT_APPLICABLE) ? &chars[strlen(chars)] : &chars[n];

    for (c = chars; c != end; ++c) {
        if (this->s[pos] == *c) return pos;
    }

    for (++pos; pos < this->size; ++pos) {
        if (this->s[pos] == this->s[pos-1]) continue;
        for (c = chars; c != end; ++c) {
            if (this->s[pos] == *c) return pos;
        }
    }
    return STRING_NPOS;
}

unsigned string_find_last_of(String *this, unsigned pos, const char *chars, unsigned n) {
    char const *c, *end;
    if (pos >= this->size || !chars) return STRING_ERROR;
    else if (!(*chars && n)) return pos;
    end = (n == STRING_NOT_APPLICABLE) ? &chars[strlen(chars)] : &chars[n];

    for (c = chars; c != end; ++c) {
        if (this->s[pos] == *c) return pos;
    }

    for (--pos; pos != UINT_MAX; --pos) {
        if (this->s[pos] == this->s[pos + 1]) continue;
        for (c = chars; c != end; ++c) {
            if (this->s[pos] == *c) return pos;
        }
    }
    return STRING_NPOS;
}

unsigned string_find_first_not_of(String *this, unsigned pos, const char *chars, unsigned n) {
    char const *c, *end;
    if (pos >= this->size || !chars) return STRING_ERROR;
    else if (!(*chars && n)) return pos;
    end = (n == STRING_NOT_APPLICABLE) ? &chars[strlen(chars)] : &chars[n];

    for (c = chars; c != end; ++c) {
        if (this->s[pos] == *c) break;
    }
    if (c == end) return pos;
    for (++pos; pos < this->size; ++pos) {
        if (this->s[pos] == this->s[pos-1]) continue;
        for (c = chars; c != end; ++c) {
            if (this->s[pos] == *c) break;
        }
        if (c == end) return pos;
    }
    return STRING_NPOS;
}

unsigned string_find_last_not_of(String *this, unsigned pos, const char *chars, unsigned n) {
    char const *c, *end;
    if (pos >= this->size || !chars) return STRING_ERROR;
    else if (!(*chars && n)) return pos;
    end = (n == STRING_NOT_APPLICABLE) ? &chars[strlen(chars)] : &chars[n];

    for (c = chars; c != end; ++c) {
        if (this->s[pos] == *c) break;
    }
    if (c == end) return pos;
    for (--pos; pos != UINT_MAX; --pos) {
        if (this->s[pos] == this->s[pos+1]) continue;
        for (c = chars; c != end; ++c) {
            if (this->s[pos] == *c) break;
        }
        if (c == end) return pos;
    }
    return STRING_NPOS;
}

unsigned string_find(String *this, unsigned start_pos, const char *needle, unsigned len) {
    const unsigned len_haystack = this->size - start_pos;
    unsigned res = STRING_NPOS, i = 0, j = 0;
    unsigned *table;
    char *haystack;
    if (start_pos >= this->size || !needle) return STRING_ERROR;
    else if (!(*needle && len)) return start_pos;

    if (len == STRING_NOT_APPLICABLE) len = (unsigned) strlen(needle);

    if (len > len_haystack) return STRING_NPOS;
    else if (len == 1) return string_find_first_of(this, start_pos, needle, 1);

    haystack = this->s + start_pos;
    table = str_gen_prefix_table(needle, len);
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

        if (j == len) {
            res = start_pos + (i - j);
            break;
        }
    }
    free(table);
    return res;
}

unsigned string_rfind(String *this, unsigned end_pos, const char *needle, unsigned len) {
    unsigned res = STRING_NPOS, i, j, minIndex;
    unsigned *table;
    char *haystack = this->s;
    if (end_pos >= this->size || !needle) return STRING_ERROR;
    else if (!(*needle && len)) return end_pos;

    if (len == STRING_NOT_APPLICABLE) len = (unsigned) strlen(needle);

    if (len > end_pos + 1) return STRING_NPOS;
    else if (len == 1) return string_find_last_of(this, end_pos, needle, 1);

    minIndex = len - 1, j = minIndex, i = j - 1;
    table = malloc(sizeof(unsigned) * len);
    if (!table) return STRING_ERROR;
    table[minIndex] = minIndex;

    while (i != UINT_MAX) {
        if (needle[i] == needle[j]) {
            table[i--] = --j;
        } else if (j != minIndex) {
            j = table[j + 1];
        } else {
            table[i--] = minIndex;
        }
    }
    
    i = end_pos, j = len - 1;
    while (i != UINT_MAX) {
        if (haystack[i] == needle[j]) {
            --i;
            --j;
        } else if (j != minIndex) {
            j = table[j + 1];
        } else {
            --i;
        }

        if (j == UINT_MAX) {
            res = i + 1;
            break;
        }
    }
    free(table);
    return res;
}

String *string_substr(String *this, unsigned start, unsigned n, int step_size) {
    String *sub;
    char *const s = this->s;
    if (start >= this->size || !n) return NULL;

    if (step_size == 0) step_size = 1;
    sub = string_new();
    if (!sub) return NULL;

    if (step_size < 0) {
        long i = start, end = -1;
        if (n != STRING_NOT_APPLICABLE) {
            const long limit = i + ((long) n * step_size);
            end = max(end, limit);
        }
        for (; i > end; i += step_size) {
            string_push_back(sub, s[i]);
        }
    } else {
        const unsigned ss = (unsigned) step_size;
        unsigned long i = start, end = this->size;
        if (n != STRING_NOT_APPLICABLE) {
            const unsigned long limit = i + (n * ss);
            end = min(end, limit);
        }
        for (; i < end; i += ss) {
            string_push_back(sub, s[i]);
        }
    }
    return sub;
}

String **string_split(String *this, const char *delim) {
    const unsigned iEnd = this->size;
    unsigned len, i = 0, j = 0, arrIdx = 0, arrLen = 8;
    unsigned *positions, *table;
    char *haystack = this->s;
    String **arr;
    String *substring = NULL;
    if (!(delim && *delim && this->size)) return NULL;

    len = (unsigned) strlen(delim);
    positions = calloc(8, sizeof(unsigned));
    if (!positions) return NULL;

    table = str_gen_prefix_table(delim, len);
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

        if (j == len) {
            if (arrIdx == arrLen) {
                unsigned *temp;
                arrLen <<= 1;
                temp = realloc(positions, arrLen * sizeof(unsigned));
                if (!temp) {
                    free(positions);
                    free(table);
                    return NULL;
                }
                positions = temp;
                memset(&positions[arrIdx + 1], 0, (arrLen - arrIdx + 1) * sizeof(unsigned));
            }
            positions[arrIdx++] = i - j;
            j = table[j - 1];
        }
    }

    if (arrIdx == arrLen) {
        unsigned *temp = realloc(positions, (arrLen + 1) * sizeof(unsigned));
        if (!temp) {
            free(positions);
            free(table);
            return NULL;
        }
        positions = temp;
    }
    positions[arrIdx++] = UINT_MAX;
    arrLen = arrIdx;

    arr = malloc((arrLen + 1) * sizeof(String *));
    if (!arr) {
        free(positions);
        free(table);
        return NULL;
    }

    arrIdx = 0, i = 0, j = positions[0];

    while (j != UINT_MAX) {
        substring = string_new();
        if (!substring) {
            free(positions);
            free(table);
            return NULL;
        }
        string_insert(substring, 0, &haystack[i], j - i);
        arr[arrIdx++] = substring;
        i = j + len;
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

char *str_read_format(unsigned *n, const char *format, va_list args) {
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
        *n = (unsigned) _n;
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
    *n = (unsigned) _n;
    return buf;
}

unsigned char string_replace_withFormat(String *this, unsigned pos, unsigned nToReplace, const char *format, ...) {
    va_list args;
    unsigned char res;
    unsigned n = 0;
    char *result;
    va_start(args, format);
    result = str_read_format(&n, format, args);
    va_end(args);
    if (!result) return 0;
    res = string_replace(this, pos, nToReplace, result, n);
    free(result);
    return res;
}

String *string_new_withFormat(const char *format, ...) {
    va_list args;
    unsigned n = 0;
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
