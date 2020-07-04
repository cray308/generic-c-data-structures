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


/**
 * The c-string representation of the provided String.
 *
 * @param   s  Pointer to string struct.
 */
#define string_c_str(s) ((s)->s)


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
#define string_ref(s, i) (((i) < (s)->len) ? &((s)->s[i]) : NULL)

/**
 * The char located at index i of the string.
 *
 * @param   s  Pointer to string struct.
 * @param   i  Index in string.
 */
#define string_at(s, i) (((i) < (s)->len) ? ((s)->s[i]) : 0)


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
 * @param  n      The number of characters to delete.
 */
void string_erase(String *s, size_t start, size_t n);


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
 * @param  len    Number of characters from other that will be used.
 */
void string_replace(String *s, size_t pos, const char *other, size_t len);


/**
 * Inserts "len" characters from the c-string "other" into "s" before "pos".
 *
 * @param  s      Pointer to string.
 * @param  pos    Index in "s" before which characters will be inserted.
 * @param  other  C-string from which characters will be inserted.
 * @param  len    Number of characters from "other" to insert.
 */
void string_insert(String *s, size_t pos, const char *other, size_t len);


/**
 * Appends "len" characters from the c-string "other" to the end of "s".
 *
 * @param  s      Pointer to string.
 * @param  other  C-string from which characters will be inserted.
 * @param  len    Number of characters from "other" to insert.
 */
void string_append(String *s, const char *other, size_t len);


/**
 * Inserts a format string into "s" before "pos". If "pos" is after the last index, the format
 *   string will be appended to "s".
 *
 * @param  s       Pointer to string.
 * @param  pos     Index in "s" before which characters will be inserted.
 * @param  format  Format string.
 */
void string_printf(String *s, size_t pos, const char *format, ...);


/**
 * Finds the first occurrence of "needle" in "s" starting at "start_pos".
 *
 * @param   s           Pointer to string.
 * @param   start_pos   First index in the string to consider for the search.
 * @param   needle      Substring to find.
 * @param   len_needle  Number of characters to match from needle.
 *
 * @return              The index in "s", corresponding to needle[0], where needle was found,
 *                          or -1 if it was not found.
 */
int string_find(String *s, size_t start_pos, const char *needle, size_t len_needle);


/**
 * Finds the last occurrence of "needle" in "s" ending at "end_pos".
 *
 * @param   s           Pointer to string.
 * @param   end_pos     Last index in the string to consider for the search.
 * @param   needle      Substring to find.
 * @param   len_needle  Number of characters to match from needle.
 *
 * @return              The index in "s", corresponding to needle[0], where needle was found,
 *                          or -1 if it was not found.
 */
int string_rfind(String *s, size_t end_pos, const char *needle, size_t len_needle);


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
int string_find_first_of(String *s, size_t pos, const char *chars);


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
int string_find_first_not_of(String *s, size_t pos, const char *chars);


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
 * Returns a dynamically allocated string starting at "pos", using "len" characters total.
 *
 * @param   s    Pointer to string.
 * @param   pos  Index where the substring should start.
 * @param   len  Number of characters in the substring. If this is -1, the substring will
 *                 comprise all characters from "pos" to the end of the string.
 *
 * @return       [description]
 */
char *string_substr(String *s, size_t pos, int len);


/**
 * Splits the c-string into substrings based on the provided delimiter.
 *
 * @param   str    C-string.
 * @param   delim  The delimiter to use to split the string.
 * @param   n      Pointer to int which will be assigned the size of the array.
 *
 * @return         The array of substrings.
 */
char **str_split(const char *str, const char *delim, int *n);


/**
 * Frees the memory allocated by str_split.
 *
 * @param  arr  Array allocated by str_split.
 * @param  n    Size of the array.
 */
void str_split_free(char **arr, int n);


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
