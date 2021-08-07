#ifndef STR_H
#define STR_H

#include "ds.h"
#include <ctype.h>

typedef struct {
    unsigned size;
    unsigned cap;
    char *s;
} String;

#define STRING_NPOS 4294967293
#define STRING_ERROR 4294967294

/**
 * The c-string representation of the provided String.
 */
#define string_c_str(this) (this)->s


/**
 * The number of characters in the string (analogous to strlen, but O(1) time complexity in this case).
 */
#define string_len(this) (this)->size


/**
 * The capacity of the string (maximum size + 1, to account for the null character).
 */
#define string_capacity(this) (this)->cap


/**
 * Tests whether the size of the string is 0.
 */
#define string_empty(this) !(this)->size


/**
 * Direct access to the char located at index @c i of the string. Does NOT perform bounds checking.
 *
 * @param  i  Index in string.
 */
#define string_index(this, i) (this)->s[i]


/**
 * Reference to the string starting at index @c i . Performs bounds checking.
 *
 * @param  i  Index in string.
 */
#define string_at(this, i) ((i) < (this)->size ? &(this)->s[i] : NULL)


/**
 * Char pointer to the front of the string.
 */
#define string_front(this) ((this)->size ? &(this)->s[0] : NULL)


/**
 * Char pointer to the back of the string.
 */
#define string_back(this) ((this)->size ? &(this)->s[(this)->size - 1] : NULL)


/**
 * Iterates through each character in the string from beginning to end.
 *
 * @param  it  Char pointer to use during iteration.
 */
#define string_iter(this, it) for (it = string_front(this); it != ((this)->size ? &(this)->s[(this)->size] : NULL); ++it)


/**
 * Iterates through each character in the string from end to the beginning.
 *
 * @param  it  Char pointer to use during iteration.
 */
#define string_riter(this, it) for (it = string_back(this); it != ((this)->size ? &(this)->s[-1] : NULL); --it)


/**
 * Request a change in capacity (maximum size) to @c n .
 *
 * @param   n  New capacity.
 *
 * @return     Whether the operation succeeded.
 */
unsigned char string_reserve(String *this, unsigned n);


/**
 * Replaces @c nToReplace characters in this string, starting at @c pos , with @c len characters from @c s .
 *
 * @param   pos         Index in the string where the replacement will occur.
 * @param   nToReplace  Number of characters to overwrite in this string.
 * @param   s            C-string used as the replacement.
 * @param   len         Number of characters from @c s that will be used. If this is DS_ARG_NOT_APPLICABLE ,
 *                       all characters from @c s will be used.
 *
 * @return              Whether the operation succeeded.
 */
unsigned char string_replace(String *this, unsigned pos, unsigned nToReplace, const char *s, unsigned len);


/**
 * Replaces @c nToReplace characters in this string, starting at @c pos , with @c len characters from @c other 
 * starting at @c subpos .
 *
 * @param   pos         Index in the string where the replacement will occur.
 * @param   nToReplace  Number of characters to overwrite in this string.
 * @param   other       Existing @c String from which characters will be inserted.
 * @param   subpos      Index in @c other denoting the position of the first character to be inserted.
 * @param   len         Number of characters from @c other to insert. If this is DS_ARG_NOT_APPLICABLE ,
 *                       all characters from @c subpos through the end of @c other will be used.
 *
 * @return              Whether the operation succeeded.
 */
unsigned char string_replace_fromString(String *this, unsigned pos, unsigned nToReplace, const String *other, unsigned subpos, unsigned len);


/**
 * Replaces @c nToReplace characters in this string, starting at @c pos , with @c n copies of @c c .
 *
 * @param   pos         Index in the string where the replacement will occur.
 * @param   nToReplace  Number of characters to overwrite in this string.
 * @param   n           Number of copies of @c c to insert.
 * @param   c           Character to insert.
 *
 * @return              Whether the operation succeeded.
 */
unsigned char string_replace_repeatingChar(String *this, unsigned pos, unsigned nToReplace, unsigned n, char c);


/**
 * Inserts @c len characters from @c s into this string before @c pos .
 *
 * @param   pos  Index in this string before which characters will be inserted. If this is @c string_len ,
 *                characters from @c s will be appended.
 * @param   s    C-string from which characters will be inserted.
 * @param   len  Number of characters from @c s to insert. If this is DS_ARG_NOT_APPLICABLE ,
 *                all characters from @c s will be used.
 *
 * @return       Whether the operation succeeded.
 */
#define string_insert(this, pos, s, len) string_replace(this, pos, 0, s, len)


/**
 * Inserts a substring of @c other , starting at @c subpos , into this string before @c pos .
 *
 * @param   pos     Index in this string before which characters will be inserted. If this is @c string_len ,
 *                   characters from @c other will be appended to this string.
 * @param   other   Existing @c String from which characters will be inserted.
 * @param   subpos  Index in @c other denoting the position of the first character to be inserted.
 * @param   len     Number of characters from @c other to insert. If this is DS_ARG_NOT_APPLICABLE ,
 *                   all characters from @c subpos through the end of @c other will be inserted.
 *
 * @return          Whether the operation succeeded.
 */
#define string_insert_fromString(this, pos, other, subpos, len) string_replace_fromString(this, pos, 0, other, subpos, len)


/**
 * Inserts @c n instances of the character @c c into this string before @c pos .
 *
 * @param   pos  Index in this string before which characters will be inserted. If this is @c string_len ,
 *                characters from @c other will be appended to this string.
 * @param   n    Number of copies of @c c to insert.
 * @param   c    Character to insert.
 *
 * @return       Whether the operation succeeded.
 */
#define string_insert_repeatingChar(this, pos, n, c) string_replace_repeatingChar(this, pos, 0, n, c)


/**
 * Appends @c len characters from @c s to the end of this string.
 *
 * @param   s    C-string from which characters will be inserted.
 * @param   len  Number of characters from @c s to insert. If this is DS_ARG_NOT_APPLICABLE , all characters
 *                from @c s will be used.
 *
 * @return       Whether the operation succeeded.
 */
#define string_append(this, s, len) string_insert(this, string_len(this), s, len)


/**
 * Appends a substring of @c other , starting at @c subpos , to this string.
 *
 * @param   other   Existing @c String from which characters will be inserted.
 * @param   subpos  Index in @c other denoting the position of the first character to be inserted.
 * @param   len     Number of characters from @c other to insert. If this is DS_ARG_NOT_APPLICABLE ,
 *                   all characters from @c subpos through the end of @c other will be inserted.
 *
 * @return          Whether the operation succeeded.
 */
#define string_append_fromString(this, other, subpos, len) string_insert_fromString(this, string_len(this), other, subpos, len)


/**
 * Appends @c n instances of the character @c c to this string.
 *
 * @param   n  Number of copies of @c c to insert.
 * @param   c  Character to insert.
 *
 * @return     Whether the operation succeeded.
 */
#define string_append_repeatingChar(this, n, c) string_insert_repeatingChar(this, string_len(this), n, c)


/**
 * Creates a new string from a c-string @c s .
 * 
 * @param   s  C-string.
 * @param   n  Number of characters from @c s to include. If this is DS_ARG_NOT_APPLICABLE , @c strlen(s)
 *              characters will be used.
 *
 * @return     Pointer to newly created string.
 */
String *string_new_fromCStr(const char *s, unsigned n);


/**
 * Creates a new, empty string.
 *
 * @return  Pointer to newly created string.
 */
#define string_new() string_new_fromCStr(NULL, 0)


/**
 * Creates a new string as a copy of @c other .
 * 
 * @param   other  Pointer to existing @c String .
 *
 * @return         Pointer to newly created string.
 */
#define string_createCopy(other) string_new_fromCStr((other)->s, (other)->size)


/**
 * Creates a new string as a substring of @c other , starting at index @c pos and using @c n characters.
 *
 * @param   other  Pointer to existing @c String .
 * @param   pos    Index of the first character in @c other to be copied.
 * @param   n      Maximum number of characters to be used. If this is DS_ARG_NOT_APPLICABLE , all characters
 *                  from @c pos to the end of @c other will be used.
 *
 * @return         Pointer to newly created string.
 */
String *string_new_fromString(const String *other, unsigned pos, unsigned n);


/**
 * Creates a new string with size @c n , where each index is set to @c c .
 *
 * @param   n  Number of characters to initialize.
 * @param   c  Character to set for each of the indices.
 *
 * @return     Pointer to the newly created string.
 */
String *string_new_repeatingChar(unsigned n, char c);


/**
 * Resizes the string to be @c n characters long. If this is less than the current size, all but the 
 * first @c n characters are removed. If this is greater than or equal to the current size, the provided 
 * character @c c is appended.
 *
 * @param   n  The new size.
 * @param   c  Character to append.
 *
 * @return     Whether the operation succeeded.
 */
unsigned char string_resize_usingChar(String *this, unsigned n, char c);


/**
 * Resizes the string to be @c n characters long. If this is less than the current size, all but the 
 * first @c n characters are removed. If this is greater than or equal to the current size, the null 
 * character is appended.
 *
 * @param   n  The new size.
 *
 * @return     Whether the operation succeeded.
 */
#define string_resize(this, n) string_resize_usingChar(this, n, 0)


/**
 * Removes @c n characters from the string, starting at index @c start .
 *
 * @param  start  The first index to delete.
 * @param  n      The number of characters to delete. If this is DS_ARG_NOT_APPLICABLE , all characters
 *                 from @c start until the end will be removed.
 */
void string_erase(String *this, unsigned start, unsigned n);


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
void string_shrink_to_fit(String *this);


/**
 * Appends @c c to the end of the string.
 *
 * @param   c  Character to append.
 *
 * @return     Whether the operation succeeded.
 */
#define string_push_back(this, c) string_resize_usingChar(this, (this)->size + 1, c)


/**
 * Removes the last character, if the string is not empty.
 */
#define string_pop_back(this) string_erase(this, string_len(this) - 1, 1)


/**
 * Finds the first index at or after @c pos where one of the characters in @c chars was found.
 *
 * @param   pos    First index in the string to consider.
 * @param   chars  C-string of characters to look for.
 * @param   n      Number of characters to consider in @c chars . If this is DS_ARG_NOT_APPLICABLE ,
 *                  all will be considered.
 *
 * @return         The first index at or after @c pos where one of the supplied characters was
 *                   found, @c STRING_NPOS if it was not found, or @c STRING_ERROR if an error occurred.
 */
unsigned string_find_first_of(String *this, unsigned pos, const char *chars, unsigned n);


/**
 * Finds the last index at or before @c pos where one of the characters in @c chars was found.
 *
 * @param   pos    Last index in the string to consider.
 * @param   chars  C-string of characters to look for.
 * @param   n      Number of characters to consider in @c chars . If this is DS_ARG_NOT_APPLICABLE ,
 *                  all will be considered.
 *
 * @return         The last index at or before @c pos where one of the supplied characters was
 *                   found, @c STRING_NPOS if it was not found, or @c STRING_ERROR if an error occurred.
 */
unsigned string_find_last_of(String *this, unsigned pos, const char *chars, unsigned n);


/**
 * Finds the first index at or after @c pos where one of the characters in @c chars was not found.
 *
 * @param   pos    First index in the string to consider.
 * @param   chars  C-string of characters to look for.
 * @param   n      Number of characters to consider in @c chars . If this is DS_ARG_NOT_APPLICABLE ,
 *                  all will be considered.
 *
 * @return         The first index at or after @c pos where a different character was found,
 *                   @c STRING_NPOS if it was not found, or @c STRING_ERROR if an error occurred.
 */
unsigned string_find_first_not_of(String *this, unsigned pos, const char *chars, unsigned n);


/**
 * Finds the last index at or before @c pos where one of the characters in @c chars was not found.
 *
 * @param   pos    Last index in the string to consider.
 * @param   chars  C-string of characters to look for.
 * @param   n      Number of characters to consider in @c chars . If this is DS_ARG_NOT_APPLICABLE ,
 *                  all will be considered.
 *
 * @return         The last index at or before @c pos where a different character was found,
 *                   @c STRING_NPOS if it was not found, or @c STRING_ERROR if an error occurred.
 */
unsigned string_find_last_not_of(String *this, unsigned pos, const char *chars, unsigned n);


/**
 * Finds the first occurrence of the first @c len_needle characters from @c needle in this string 
 * starting at @c start_pos .
 *
 * @param   start_pos   First index in the string to consider for the search.
 * @param   needle      Substring to find.
 * @param   len         Number of characters to match from needle. If this is DS_ARG_NOT_APPLICABLE ,
 *                       all characters from @c needle will be used.
 *
 * @return              The index in this string, corresponding to needle[0], where @c needle was found,
 *                      @c STRING_NPOS if it was not found, or @c STRING_ERROR if an error occurred.
 */
unsigned string_find(String *this, unsigned start_pos, const char *needle, unsigned len);


/**
 * Finds the last occurrence of the first @c len_needle characters from @c needle in this string 
 * ending at @c end_pos .
 *
 * @param   end_pos     Last index in the string to consider for the search.
 * @param   needle      Substring to find.
 * @param   len         Number of characters to match from needle. If this is DS_ARG_NOT_APPLICABLE ,
 *                       all characters from @c needle will be used.
 *
 * @return              The index in this string, corresponding to needle[0], where @c needle was found,
 *                        @c STRING_NPOS if it was not found, or @c STRING_ERROR if an error occurred.
 */
unsigned string_rfind(String *this, unsigned end_pos, const char *needle, unsigned len);


/**
 * Creates a substring from this string with @c n characters, starting at @c start and moving to 
 * the next character to include with a step size of @c step_size .
 *
 * @param   start      Index where the substring should start.
 * @param   n          Maximum number of characters in the substring. DS_ARG_NOT_APPLICABLE implies
 *                      to include as many elements as @c start and @c step_size allow.
 * @param   step_size  How to adjust the index when copying characters. 1 means move forward 1 index
 *                       at a time, -1 means move backwards one index at a time, 2 would mean every
 *                       other index, etc.
 *
 * @return             Newly allocated @c String , or NULL if an error occurred.
 */
String *string_substr(String *this, unsigned start, unsigned n, int step_size);


/**
 * Splits this string into substrings based on @c delim and stores them as newly allocated @c Strings 
 * in an array. A sentinel value of NULL is placed at the last index of the array.
 *
 * @param   delim  The delimiter to use to split the string.
 *
 * @return         The array of pointers to @c String , each of which is a substring of this string, or
 *                 NULL if an error occurred.
 */
String **string_split(String *this, const char *delim);


/**
 * Frees the memory allocated by @c string_split .
 *
 * @param  arr  Array allocated by @c string_split .
 */
#define string_split_free(arr) {                                                                             \
    String **_sPtr;                                                                                          \
    for (_sPtr = arr; *_sPtr; ++_sPtr) string_free(*_sPtr);                                                  \
    free(arr);                                                                                               \
}


/**
 * @param   s  C-string.
 *
 * @return     Whether or not all characters in @c s are alphanumeric.
 */
__attribute__((__unused__)) static
#if __STDC_VERSION__ >= 199901L
inline
#endif
unsigned char isAlphaNum(const char *s) {
    const char *c;
    if (!*s) return 0;
    for (c = s; *c; ++c) {
        if (!isalnum(*c)) return 0;
    }
    return 1;
}


/**
 * @param   s  C-string.
 *
 * @return     Whether or not all characters in @c s are letters.
 */
__attribute__((__unused__)) static
#if __STDC_VERSION__ >= 199901L
inline
#endif
unsigned char isAlpha(const char *s) {
    const char *c;
    if (!*s) return 0;
    for (c = s; *c; ++c) {
        if (!isalpha(*c)) return 0;
    }
    return 1;
}


/**
 * @param   s  C-string.
 *
 * @return     Whether or not all characters in @c s are digits.
 */
__attribute__((__unused__)) static
#if __STDC_VERSION__ >= 199901L
inline
#endif
unsigned char isDigit(const char *s) {
    const char *c;
    if (!*s) return 0;
    for (c = s; *c; ++c) {
        if (!isdigit(*c)) return 0;
    }
    return 1;
}


/**
 * Converts all characters in @c s to lowercase.
 *
 * @param  s  C-string.
 */
__attribute__((__unused__)) static
#if __STDC_VERSION__ >= 199901L
inline
#endif
void toLowercase(char *s) {
    char *c;
    for (c = s; *c; ++c) *c = (char) tolower(*c);
}


/**
 * Converts all characters in @c s to uppercase.
 *
 * @param  s  C-string.
 */
__attribute__((__unused__)) static
#if __STDC_VERSION__ >= 199901L
inline
#endif
void toUppercase(char *s) {
    char *c;
    for (c = s; *c; ++c) *c = (char) toupper(*c);
}


#if __STDC_VERSION__ >= 199901L

/**
 * Replaces @c nToReplace characters in this string, starting at @c pos , with the format string @c format .
 *
 * @param   pos         Index in the string where the replacement will occur.
 * @param   nToReplace  Number of characters to overwrite in this string.
 * @param   format      Format string.
 *
 * @return              Whether the operation succeeded.
 */
unsigned char string_replace_withFormat(String *this, unsigned pos, unsigned nToReplace, const char *format, ...);

/**
 * Inserts a format string @c format into this string before @c pos .
 *
 * @param   pos     Index in this string before which characters will be inserted. If this is @c string_len ,
 *                   characters from @c format will be appended to this string.
 * @param   format  Format string.
 *
 * @return          Whether the operation succeeded.
 */
#define string_insert_withFormat(this, pos, format, ...) string_replace_withFormat(this, pos, 0, format, __VA_ARGS__)

/**
 * Appends a format string @c format to this string.
 *
 * @param   format  Format string.
 *
 * @return          Whether the operation succeeded.
 */
#define string_append_withFormat(this, format, ...) string_insert_withFormat(this, string_len(this), format, __VA_ARGS__)

/**
 * Creates a new string from the format string @c format .
 * 
 * @param   format  Format string.
 *
 * @return          Pointer to newly created string.
 */
String *string_new_withFormat(const char *format, ...);

#endif /* __STDC_VERSION__ >= 199901L */
#endif /* STR_H */
