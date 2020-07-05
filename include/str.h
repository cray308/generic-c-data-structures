#ifndef STR_H
#define STR_H

#include "ds.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>

typedef struct {
    size_t len;
    size_t cap;
    char *s;
} String;


#define STRING_END(s) ((s)->len)
#define STRING_NPOS (-1)


/**
 * The c-string representation of the provided String.
 *
 * @param   s  Pointer to string struct.
 */
inline char *string_c_str(String *s) {
    return s->s;
}


/**
 * The number of characters in the string (analogous to strlen, but O(1) time complexity in
 *   this case).
 *
 * @param   s  Pointer to string struct.
 */
#define string_len(s) ((s)->len)


/**
 * The capacity of the string (maximum size + 1, to account for the null character).
 *
 * @param   s  Pointer to string struct.
 */
#define string_capacity(s) ((s)->cap)


/**
 * Tests whether the size of the string is 0.
 *
 * @param   s  Pointer to string struct.
 */
#define string_empty(s) (!((s)->len))


/**
 * The numeric index in the string from a char pointer.
 *
 * @param   s    Pointer to string struct.
 * @param   chr  The char pointer whose index you wish to find.
 */
#define string_index(s, chr) ((chr) - (s)->s)

/**
 * Reference to the string starting at index i.
 *
 * @param   s  Pointer to string struct.
 * @param   i  Index in string.
 */
inline char *string_ref(String *s, int i) {
    int _idx = modulo(i, s->len);
    return (_idx >= 0) ? &(s->s[_idx]) : NULL;
}

/**
 * The char located at index i of the string.
 *
 * @param   s  Pointer to string struct.
 * @param   i  Index in string.
 */
inline char string_at(String *s, int i) {
    int _idx = modulo(i, s->len);
    return (_idx >= 0) ? s->s[_idx] : 0;
}


/**
 * A char pointer to the front of the string.
 *
 * @param   s  Pointer to string struct.
 */
#define string_front(s) (((s)->len) ? &((s)->s[0]) : NULL)


/**
 * A char pointer to the back of the string.
 *
 * @param   s  Pointer to string struct.
 */
#define string_back(s) (((s)->len) ? &((s)->s[(s)->len - 1]) : NULL)


/**
 * Iterates through each character in the string from beginning to end.
 *
 * @param   s    Pointer to string struct.
 * @param   chr  Char pointer to use during iteration.
 */
#define string_iter(s, chr) \
    for ((chr) = ((s)->len) ? &((s)->s[0]) : NULL; (chr) != NULL; \
    (chr) = ((chr) != &(((s)->s[(s)->len - 1]))) ? (chr) + 1 : NULL)


/**
 * Iterates through each character in the string from end to the beginning.
 *
 * @param   s    Pointer to string struct.
 * @param   chr  Char pointer to use during iteration.
 */
#define string_riter(s, chr) \
    for ((chr) = ((s)->len) ? &((s)->s[(s)->len - 1]) : NULL; (chr) != NULL; \
    (chr) = ((chr) != &(((s)->s[0]))) ? (chr) - 1 : NULL)


/**
 * Creates a new string.
 *
 * @return    Pointer to newly created string.
 */
String *string_new(void);


/**
 * Frees memory allocated to the string struct.
 *
 * @param  s  Pointer to string.
 */
void string_free(String *s);


/**
 * Request a change in capacity (maximum size).
 *
 * @param  s  Pointer to string.
 * @param  n  New capacity.
 */
void string_reserve(String *s, size_t n);


/**
 * Resizes the string to be n characters long. If n is less than the current size, all but the first n
 *   characters are removed. If n is greater than or equal to the current size, the provided
 *   character will be appended.
 *
 * @param  s  Pointer to string.
 * @param  n  The new size.
 * @param  c  Character to append.
 */
void string_resize(String *s, size_t n, char c);


/**
 * Removes all characters, leaving the string with a size of 0.
 *
 * @param  s  Pointer to string.
 */
void string_clear(String *s);


/**
 * Removes "n" characters from the string, starting at index "start".
 *
 * @param  s      Pointer to string.
 * @param  start  The first index to delete.
 * @param  n      The number of characters to delete. If this is -1, all characters
 *                  from "start" until the end will be removed.
 */
void string_erase(String *s, int start, int n);


/**
 * If the string's capacity is greater than its length plus the null terminator, reallocates
 *   only enough space to fit all characters.
 *
 * @param  s  Pointer to string.
 */
void string_shrink_to_fit(String *s);


/**
 * Appends a character to the end of the string.
 *
 * @param  s  Pointer to string.
 * @param  c  Character to append.
 */
void string_push_back(String *s, char c);


/**
 * Removes the last character, if the string is not empty.
 *
 * @param  s  Pointer to string.
 */
void string_pop_back(String *s);


/**
 * Replaces characters in s, starting at pos, with len characters from other.
 *
 * @param  s      Pointer to string.
 * @param  pos    Index in s where the replacement will occur.
 * @param  other  C-string used as the replacement.
 * @param  len    Number of characters from other that will be used. If this is -1,
 *                  all characters from "other" will be used.
 */
void string_replace(String *s, int pos, const char *other, int len);


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
void string_insert(String *s, int pos, const char *other, int len);


/**
 * Appends "len" characters from the c-string "other" to the end of "s".
 *
 * @param  s      Pointer to string.
 * @param  other  C-string from which characters will be inserted.
 * @param  len    Number of characters from "other" to insert. If this is -1,
 *                  all characters from "other" will be used.
 */
void string_append(String *s, const char *other, int len);


/**
 * Inserts a format string into "s" before "pos".
 *
 * @param  s       Pointer to string.
 * @param  pos     Index in "s" before which characters will be inserted. If this is STRING_END,
 *                  characters from the format string will be appended to s.
 * @param  format  Format string.
 */
void string_printf(String *s, int pos, const char *format, ...);


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
 *                          or -1 if it was not found.
 */
int string_find(String *s, int start_pos, const char *needle, int len_needle);


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
 *                          or -1 if it was not found.
 */
int string_rfind(String *s, int end_pos, const char *needle, int len_needle);


/**
 * Finds the first index at or after "pos" where one of the supplied characters was found.
 *
 * @param   s      Pointer to string.
 * @param   pos    First index in the string to consider.
 * @param   chars  C-string of characters to look for.
 *
 * @return         The first index at or after "pos" where one of the supplied characters was
 *                     found, or -1 if no such character was found.
 */
int string_find_first_of(String *s, int pos, const char *chars);


/**
 * Finds the last index at or before "pos" where one of the supplied characters was found.
 *
 * @param   s      Pointer to string.
 * @param   pos    Last index in the string to consider.
 * @param   chars  C-string of characters to look for.
 *
 * @return         The last index at or before "pos" where one of the supplied characters was
 *                     found, or -1 if no such character was found.
 */
int string_find_last_of(String *s, int pos, const char *chars);


/**
 * Finds the first index at or after "pos" where one of the supplied characters was not found.
 *
 * @param   s      Pointer to string.
 * @param   pos    First index in the string to consider.
 * @param   chars  C-string of characters to look for.
 *
 * @return         The first index at or after "pos" where a different character was found, or -1
 *                     if no such character was found.
 */
int string_find_first_not_of(String *s, int pos, const char *chars);


/**
 * Finds the last index at or before "pos" where one of the supplied characters was not found.
 *
 * @param   s      Pointer to string.
 * @param   pos    Last index in the string to consider.
 * @param   chars  C-string of characters to look for.
 *
 * @return         The last index at or before "pos" where a different character was found, or -1
 *                     if no such character was found.
 */
int string_find_last_not_of(String *s, int pos, const char *chars);


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
 * @return       Newly allocated String.
 */
String *string_substr(String *s, int start, int n, int step_size);


/**
 * Splits "s" into substrings based on the provided delimiter and stores them as
 * newly allocated String pointers in an array.
 *
 * @param   s      Pointer to string.
 * @param   delim  The delimiter to use to split the string.
 * @param   n      Pointer to int which will be assigned the size of the array.
 *
 * @return         The array of pointers to String, each of which is a substring of "s".
 */
String **string_split(String *s, const char *delim, int *n);


/**
 * Frees the memory allocated by string_split.
 *
 * @param  arr  Array allocated by string_split.
 * @param  n    Size of the array.
 */
void string_split_free(String **arr, int n);


/**
 * @param   s  C-string.
 *
 * @return     Whether or not all characters in s are alphanumeric.
 */
bool isAlphaNum(const char *s);


/**
 * @param   s  C-string.
 *
 * @return     Whether or not all characters in s are letters.
 */
bool isAlpha(const char *s);


/**
 * @param   s  C-string.
 *
 * @return     Whether or not all characters in s are digits.
 */
bool isDigit(const char *s);


/**
 * Converts all characters in s to lowercase.
 *
 * @param  s  C-string.
 */
void toLowercase(char *s);


/**
 * Converts all characters in s to uppercase.
 *
 * @param  s  C-string.
 */
void toUppercase(char *s);

#endif // STR_H
