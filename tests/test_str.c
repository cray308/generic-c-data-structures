#include "str.h"
#include <assert.h>
#include <limits.h>

#define LEN 78
const char *testStr = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";

void compareStrs(String *s, const char *comparison, unsigned size) {
    unsigned i = 0, j = size;
    const char *it, *ref;
    assert(string_len(s) == size);
    if (size) {
        assert(!string_empty(s));
        assert(*string_front(s) == comparison[0] && *string_back(s) == comparison[size-1]);
    } else {
        assert(string_empty(s));
    }
    string_iter(s, it) {
        ref = string_at(s, size - j);
        assert(ref && *ref == comparison[i]);
        assert(string_index(s, i) == comparison[i]);
        assert(*it == comparison[i++]); --j;
    }
    assert(i == size);
    i = size - 1;
    j = 1;
    string_riter(s, it) {
        ref = string_at(s, size - j);
        assert(ref && *ref == comparison[i]);
        assert(string_index(s, i) == comparison[i]);
        assert(*it == comparison[i--]); ++j;
    }
    assert(i == UINT_MAX);
}

void test_empty_init(void) {
    String *s = string_new();
    compareStrs(s, "", 0);
    string_free(s);
}

void test_init_repeatingChar(void) {
    char *c = "55";
    String *s = string_new_repeatingChar(0, '5');
    compareStrs(s, c, 0);
    string_free(s);
    s = string_new_repeatingChar(1, '5');
    compareStrs(s, c, 1);
    string_free(s);
    s = string_new_repeatingChar(2, '5');
    compareStrs(s, c, 2);
    string_free(s);
}

void test_init_fromCStr(void) {
    String *s = string_new_fromCStr(NULL, 5);
    compareStrs(s, "", 0);
    string_free(s);
    s = string_new_fromCStr(testStr, 0);
    compareStrs(s, testStr, 0);
    string_free(s);
    s = string_new_fromCStr(testStr, 1);
    compareStrs(s, testStr, 1);
    string_free(s);
    s = string_new_fromCStr(testStr, 2);
    compareStrs(s, testStr, 2);
    string_free(s);
    s = string_new_fromCStr(testStr, DS_ARG_NOT_APPLICABLE);
    compareStrs(s, testStr, 78);
    string_free(s);
}

void test_create_copy(void) {
    String *b = string_new_fromCStr(testStr, 0);
    String *s = string_createCopy(b);
    compareStrs(s, testStr, 0);
    string_free(s);
    string_free(b);
    b = string_new_fromCStr(testStr, 5);
    s = string_createCopy(b);
    compareStrs(s, testStr, 5);
    string_free(s);
    string_free(b);
}

void test_init_fromString(void) {
    String *b = string_new_fromCStr(testStr, 0);
    String *s = string_new_fromString(b, 0, 0);
    compareStrs(s, testStr, 0);
    string_free(s);
    s = string_new_fromString(b, b->size - 1, 5);
    compareStrs(s, testStr, 0);
    string_free(s);
    string_free(b);
    b = string_new_fromCStr(testStr, 26);
    s = string_new_fromString(b, b->size - 24, DS_ARG_NOT_APPLICABLE);
    compareStrs(s, &testStr[2], 24);
    string_free(s);
    s = string_new_fromString(b, 5, 200);
    compareStrs(s, &testStr[5], 21);
    string_free(s);
    string_free(b);
}

void test_push_back(void) {
    int i;
    String *s = string_new();
    for (i = 0; i < 3; ++i) {
        string_push_back(s, testStr[i]);
    }
    compareStrs(s, testStr, 3);
    string_free(s);
}

void test_pop_back(void) {
    unsigned i = 2;
    String *s = string_new_fromCStr(testStr, 3);
    while (!string_empty(s)) {
        string_pop_back(s);
        compareStrs(s, testStr, i--);
    }
    string_pop_back(s);
    compareStrs(s, testStr, 0);
    string_free(s);
}

void test_resize(void) {
    char c[] = {'a','a','a','b','b',0,0};
    unsigned i = 2;
    String *s = string_new_fromCStr(testStr, 3);
    while (!string_empty(s)) {
        string_resize(s, i);
        compareStrs(s, testStr, i--);
    }
    string_resize(s, 0);
    compareStrs(s, testStr, 0);
    string_resize_usingChar(s, 3, 'a');
    string_resize_usingChar(s, 5, 'b');
    string_resize(s, 7);
    compareStrs(s, c, 7);
    string_free(s);
}

void test_reserve(void) {
    String *s = string_new();
    assert(string_capacity(s) == 64);
    string_reserve(s, 63);
    assert(string_capacity(s) == 64);
    string_reserve(s, 65);
    assert(string_capacity(s) == 128);
    string_free(s);
}

void test_shrink(void) {
    String *s = string_new();
    string_shrink_to_fit(s);
    assert(string_capacity(s) == 64);
    string_insert(s, 0, testStr, 63);
    string_shrink_to_fit(s);
    assert(string_capacity(s) == 64);
    string_insert(s, s->size - 1, &testStr[63], 1);
    assert(string_capacity(s) == 128);
    string_shrink_to_fit(s);
    assert(string_capacity(s) == 65);
    string_free(s);
}

void test_cstr(void) {
    String *s = string_new_fromCStr(testStr, 0);
    assert(streq(string_c_str(s), ""));
    string_push_back(s, '1');
    assert(streq(string_c_str(s), "1"));
    string_push_back(s, 0);
    assert(streq(string_c_str(s), "1"));
    string_free(s);
}

void test_insert_cStr(void) {
    String *s = string_new();
    string_insert(s, 0, NULL, 64);
    string_insert(s, 0, "", 64);
    string_insert(s, 0, testStr, 0);
    string_insert(s, s->size - 1, testStr, 64);
    string_insert(s, 10, testStr, 64);
    compareStrs(s, testStr, 0);
    string_insert(s, 0, "c", DS_ARG_NOT_APPLICABLE);
    string_insert(s, 0, "aaa", 2);
    string_insert(s, s->size - 1, "bc", DS_ARG_NOT_APPLICABLE);
    string_insert(s, string_len(s), "d", 1);
    compareStrs(s, "aabccd", 6);
    string_free(s);
}

void test_insert_repeatedChar(void) {
    String *s = string_new();
    string_insert_repeatingChar(s, 0, 0, 'x');
    string_insert_repeatingChar(s, s->size - 1, 5, 'x');
    string_insert_repeatingChar(s, 10, 5, 'x');
    compareStrs(s, testStr, 0);
    string_insert_repeatingChar(s, 0, 1, 'c');
    string_insert_repeatingChar(s, 0, 2, 'a');
    string_insert_repeatingChar(s, s->size - 1, 2, 'b');
    string_insert_repeatingChar(s, string_len(s), 2, 'd');
    compareStrs(s, "aabbcdd", 7);
    string_free(s);
}

void test_insert_fromString(void) {
    String *s = string_new(), *b = string_new_fromCStr(testStr, 0), *d = string_new_fromCStr("dd", 2), *a = string_new_fromCStr("aa", 2);
    string_insert_fromString(s, 0, b, 0, 0);
    string_insert_fromString(s, 0, b, 10, 64);
    string_insert_fromString(s, 0, b, b->size - 1, 5);
    string_insert(b, 0, "bbc", 3);
    string_insert_fromString(s, s->size - 1, b, 0, 1);
    string_insert_fromString(s, 10, b, 0, 1);
    compareStrs(s, testStr, 0);
    string_insert_fromString(s, 0, b, 2, 30);
    string_insert_fromString(s, 0, a, 0, DS_ARG_NOT_APPLICABLE);
    string_insert_fromString(s, s->size - 1, b, 0, 2);
    string_insert_fromString(s, string_len(s), d, 0, 3);
    compareStrs(s, "aabbcdd", 7);
    string_free(s);
    string_free(a);
    string_free(b);
    string_free(d);
}

void test_append_cStr(void) {
    String *s = string_new();
    string_append(s, NULL, 64);
    string_append(s, "", 64);
    string_append(s, testStr, 0);
    compareStrs(s, testStr, 0);
    string_append(s, "aaa", 2);
    string_append(s, "bc", DS_ARG_NOT_APPLICABLE);
    string_append(s, "c", DS_ARG_NOT_APPLICABLE);
    string_append(s, "d", 1);
    compareStrs(s, "aabccd", 6);
    string_free(s);
}

void test_append_fromString(void) {
    String *s = string_new(), *b = string_new_fromCStr(testStr, 0);
    string_append_fromString(s, b, 0, 0);
    string_append_fromString(s, b, 10, 64);
    string_append_fromString(s, b, b->size - 1, 5);
    compareStrs(s, testStr, 0);
    string_append(b, "aabbccdd", 8);
    string_append_fromString(s, b, 0, 2);
    string_append_fromString(s, b, 2, 2);
    string_append_fromString(s, b, b->size - 4, 1);
    string_append_fromString(s, b, b->size - 2, 50);
    compareStrs(s, "aabbcdd", 7);
    string_free(s);
    string_free(b);
}

void test_append_repeatingChar(void) {
    String *s = string_new();
    string_append_repeatingChar(s, 0, 'x');
    compareStrs(s, testStr, 0);
    string_append_repeatingChar(s, 2, 'a');
    string_append_repeatingChar(s, 2, 'b');
    string_append_repeatingChar(s, 1, 'c');
    string_append_repeatingChar(s, 2, 'd');
    compareStrs(s, "aabbcdd", 7);
    string_free(s);
}

void test_replace_cStr(void) {
    String *s = string_new();
    string_replace(s, 0, 0, NULL, 64);
    string_replace(s, 0, 0, "", 64);
    string_replace(s, 0, 0, testStr, 0);
    string_replace(s, s->size - 1, 0, testStr, 64);
    string_replace(s, 10, 0, testStr, 64);
    compareStrs(s, testStr, 0);
    string_replace(s, 0, 0, "xxxxx", 5);
    string_replace(s, 0, 1, "abc", 3);
    string_replace(s, string_len(s), 50, "hello", DS_ARG_NOT_APPLICABLE);
    string_replace(s, s->size - 5, 20, "y", 1);
    string_replace(s, s->size - 5, DS_ARG_NOT_APPLICABLE, "longString", 7);
    compareStrs(s, "abclongStr", 10);
    string_replace(s, 0, DS_ARG_NOT_APPLICABLE, ".", 1);
    compareStrs(s, ".", 1);
    string_free(s);
}

void test_replace_fromString(void) {
    String *s = string_new(), *b = string_new_fromCStr(testStr, 0);
    string_replace_fromString(s, 0, 0, b, 0, 0);
    string_replace_fromString(s, 0, 0, b, 10, 64);
    string_replace_fromString(s, 0, 0, b, b->size - 1, 5);
    string_append(b, "y.abclongStringhelloxxxxx", 25);
    string_replace_fromString(s, s->size - 1, 0, b, 0, 1);
    string_replace_fromString(s, 10, 0, b, 0, 1);
    compareStrs(s, testStr, 0);
    string_replace_fromString(s, 0, 0, b, b->size - 5, 50);
    string_replace_fromString(s, 0, 1, b, 2, 3);
    string_replace_fromString(s, string_len(s), 50, b, b->size - 10, 5);
    string_replace_fromString(s, s->size - 5, 20, b, 0, 1);
    string_replace_fromString(s, s->size - 5, DS_ARG_NOT_APPLICABLE, b, 5, 7);
    compareStrs(s, "abclongStr", 10);
    string_replace_fromString(s, 0, DS_ARG_NOT_APPLICABLE, b, 1, 1);
    compareStrs(s, ".", 1);
    string_free(s);
    string_free(b);
}

void test_replace_repeatingChar(void) {
    String *s = string_new();
    string_replace_repeatingChar(s, 0, 0, 0, 'x');
    string_replace_repeatingChar(s, s->size - 1, 0, 5, 'x');
    string_replace_repeatingChar(s, 10, 0, 5, 'x');
    compareStrs(s, testStr, 0);
    string_replace_repeatingChar(s, 0, 0, 5, 'x');
    string_replace_repeatingChar(s, 0, 1, 3, 'a');
    string_replace_repeatingChar(s, string_len(s), 50, 5, 'h');
    string_replace_repeatingChar(s, s->size - 5, 20, 1, 'y');
    string_replace_repeatingChar(s, s->size - 5, DS_ARG_NOT_APPLICABLE, 7, 'l');
    compareStrs(s, "aaalllllll", 10);
    string_replace_repeatingChar(s, 0, DS_ARG_NOT_APPLICABLE, 1, '.');
    compareStrs(s, ".", 1);
    string_free(s);
}

void test_remove_char(void) {
    String *s = string_new_fromCStr(testStr, 5);
    string_erase(s, 5, DS_ARG_NOT_APPLICABLE);
    string_erase(s, 4, 0);
    compareStrs(s, testStr, 5);
    string_erase(s, 0, 1);
    string_erase(s, s->size - 1, 1);
    string_erase(s, 1, 1);
    compareStrs(s, "bd", 2);
    string_free(s);
}

void test_erase_chars(void) {
    String *s = string_new_fromCStr(testStr, 10);
    string_erase(s, 0, 2);
    string_erase(s, s->size - 2, DS_ARG_NOT_APPLICABLE);
    string_erase(s, 2, 2);
    compareStrs(s, "cdgh", 4);
    string_erase(s, 0, DS_ARG_NOT_APPLICABLE);
    compareStrs(s, "", 0);
    string_free(s);
}

void test_substr(void) {
    String *s1 = string_new_fromCStr(testStr, 10), *s2 = string_new();
    assert(string_substr(s1, 0, 0, 1) == NULL);
    assert(string_substr(s2, s2->size - 1, DS_ARG_NOT_APPLICABLE, 1) == NULL);
    string_free(s2);
    s2 = string_substr(s1, 0, 200, 0);
    compareStrs(s2, testStr, 10);
    string_free(s2);
    s2 = string_substr(s1, s1->size - 2, 200, -1);
    compareStrs(s2, "ihgfedcba", 9);
    string_free(s2);
    s2 = string_substr(s1, 0, 5, 1);
    compareStrs(s2, testStr, 5);
    string_free(s2);
    s2 = string_substr(s1, s1->size - 2, 5, -1);
    compareStrs(s2, "ihgfe", 5);
    string_free(s2);
    s2 = string_substr(s1, s1->size - 2, DS_ARG_NOT_APPLICABLE, -2);
    compareStrs(s2, "igeca", 5);
    string_free(s2);
    s2 = string_substr(s1, s1->size - 1, 10, -9);
    compareStrs(s2, "ja", 2);
    string_free(s2);
    s2 = string_substr(s1, 0, 10, 9);
    compareStrs(s2, "aj", 2);
    string_free(s2);
    string_free(s1);
}

void test_find_first_of(void) {
    String *s = string_new_fromCStr("aabbccddeeffgghhiijjkkllmmnnooppqqrrssttuuvvwwxxyyzz", 52);
    assert(string_find_first_of(s, 0, NULL, 5) == STRING_ERROR);
    assert(string_find_first_of(s, 1, "", 5) == 1);
    assert(string_find_first_of(s, 1, "abc", 0) == 1);
    assert(string_find_first_of(s, 52, "abc", 3) == STRING_ERROR);
    assert(string_find_first_of(s, 0, "abc", DS_ARG_NOT_APPLICABLE) == 0);
    assert(string_find_first_of(s, 4, "abc", 2) == STRING_NPOS);
    assert(string_find_first_of(s, s->size - 1, "z", 1) == 51);
    assert(string_find_first_of(s, 0, "z", 1) == 50);
    assert(string_find_first_of(s, 0, ".", DS_ARG_NOT_APPLICABLE) == STRING_NPOS);
    string_free(s);
}

void test_find_last_of(void) {
    String *s = string_new_fromCStr("aabbccddeeffgghhiijjkkllmmnnooppqqrrssttuuvvwwxxyyzz", 52);
    assert(string_find_last_of(s, s->size - 1, NULL, 5) == STRING_ERROR);
    assert(string_find_last_of(s, 50, "", 5) == 50);
    assert(string_find_last_of(s, 50, "abc", 0) == 50);
    assert(string_find_last_of(s, s->size - 53, "abc", 3) == STRING_ERROR);
    assert(string_find_last_of(s, 0, "xyz", 3) == STRING_NPOS);
    assert(string_find_last_of(s, s->size - 1, "xyz", DS_ARG_NOT_APPLICABLE) == 51);
    assert(string_find_last_of(s, s->size - 5, "zyx", 2) == STRING_NPOS);
    assert(string_find_last_of(s, 0, "a", 1) == 0);
    assert(string_find_last_of(s, s->size - 1, "a", 1) == 1);
    assert(string_find_last_of(s, s->size - 1, ".", DS_ARG_NOT_APPLICABLE) == STRING_NPOS);
    string_free(s);
}

void test_find_first_not_of(void) {
    String *s = string_new_fromCStr("aabbccddeeffgghhiijjkkllmmnnooppqqrrssttuuvvwwxxyyzz", 52);
    assert(string_find_first_not_of(s, 0, NULL, 5) == STRING_ERROR);
    assert(string_find_first_not_of(s, 1, "", 5) == 1);
    assert(string_find_first_not_of(s, 1, "abc", 0) == 1);
    assert(string_find_first_not_of(s, 52, "abc", 3) == STRING_ERROR);
    assert(string_find_first_not_of(s, 0, "abc", DS_ARG_NOT_APPLICABLE) == 6);
    assert(string_find_first_not_of(s, 4, "abc", 2) == 4);
    assert(string_find_first_not_of(s, s->size - 1, "z", 1) == STRING_NPOS);
    assert(string_find_first_not_of(s, 0, "z", DS_ARG_NOT_APPLICABLE) == 0);
    assert(string_find_first_not_of(s, s->size - 6, "xy", DS_ARG_NOT_APPLICABLE) == 50);
    string_free(s);
}

void test_find_last_not_of(void) {
    String *s = string_new_fromCStr("aabbccddeeffgghhiijjkkllmmnnooppqqrrssttuuvvwwxxyyzz", 52);
    assert(string_find_last_not_of(s, s->size - 1, NULL, 5) == STRING_ERROR);
    assert(string_find_last_not_of(s, 50, "", 5) == 50);
    assert(string_find_last_not_of(s, 50, "abc", 0) == 50);
    assert(string_find_last_not_of(s, s->size - 53, "abc", 3) == STRING_ERROR);
    assert(string_find_last_not_of(s, 0, "xyz", 3) == 0);
    assert(string_find_last_not_of(s, s->size - 1, "xyz", DS_ARG_NOT_APPLICABLE) == 45);
    assert(string_find_last_not_of(s, s->size - 5, "zyx", 2) == 47);
    assert(string_find_last_not_of(s, 0, "a", 1) == STRING_NPOS);
    assert(string_find_last_not_of(s, s->size - 1, "a", 1) == 51);
    assert(string_find_last_not_of(s, 5, "bc", DS_ARG_NOT_APPLICABLE) == 1);
    string_free(s);
}

void test_find(void) {
    String *s = string_new_fromCStr("ABC ABCDAB ABCDABCDABDE", 23);
    assert(string_find(s, 0, NULL, 5) == STRING_ERROR);
    assert(string_find(s, 1, "", 5) == 1);
    assert(string_find(s, 1, "x", 0) == 1);
    assert(string_find(s, 30, "abc", 3) == STRING_ERROR);
    assert(string_find(s, 21, testStr, 3) == STRING_NPOS);
    assert(string_find(s, 0, "ABCDABD", 7) == 15);
    assert(string_find(s, 15, "ABCDABD", 7) == 15);
    assert(string_find(s, 16, "ABCDABD", 7) == STRING_NPOS);
    assert(string_find(s, s->size - 4, "ABCDABD", 2) == 19);
    string_free(s);
}

void test_rfind(void) {
    String *s = string_new_fromCStr("ABC ABCDAB ABCDABCDABDE", 23);
    assert(string_rfind(s, s->size - 1, NULL, 5) == STRING_ERROR);
    assert(string_rfind(s, 21, "", 5) == 21);
    assert(string_rfind(s, 21, "x", 0) == 21);
    assert(string_rfind(s, s->size - 30, "abc", 3) == STRING_ERROR);
    assert(string_rfind(s, 1, testStr, 3) == STRING_NPOS);
    assert(string_rfind(s, s->size - 1, "ABCDABD", 7) == 15);
    assert(string_rfind(s, 15, "ABCDABD", 7) == STRING_NPOS);
    assert(string_rfind(s, 21, "ABCDABD", 7) == 15);
    assert(string_rfind(s, 15, "ABC ABD", 4) == 0);
    string_free(s);
}

void test_split(void) {
    int i = 0, count = 0;
    unsigned len1[] = {22,33};
    unsigned len2[] = {4,3,7,6,10,4,4,5,5};
    const char *cmp1[] = {"this has several words"," definitely more than eight words"};
    const char *cmp2[] = {"this","has","several","words,","definitely","more","than","eight","words"};
    String **split, **it, *s = string_new();
    assert(string_split(s, NULL) == NULL);
    assert(string_split(s, "") == NULL);
    assert(string_split(s, ":") == NULL);
    string_append(s, "this has several words, definitely more than eight words", 56);

    split = string_split(s, ":");
    compareStrs(split[0], "this has several words, definitely more than eight words", 56);
    assert(split[1] == NULL);
    string_split_free(split);

    split = string_split(s, ",");
    while (split[count]) ++count;
    assert(count == 2);
    for (it = split; *it; ++it, ++i) {
        compareStrs(*it, cmp1[i], len1[i]);
    }
    string_split_free(split);

    split = string_split(s, " ");
    count = 0;
    while (split[count]) ++count;
    assert(count == 9);
    for (i = 0; i < 9; ++i) {
        compareStrs(split[i], cmp2[i], len2[i]);
    }
    string_split_free(split);
    string_free(s);
}

void test_has_certain_chars(void) {
    String *s = string_new();
    assert(!isAlphaNum(string_c_str(s)));
    assert(!isAlpha(string_c_str(s)));
    assert(!isDigit(string_c_str(s)));
    string_append(s, "hello", 5);
    assert(isAlphaNum(string_c_str(s)));
    assert(isAlpha(string_c_str(s)));
    assert(!isDigit(string_c_str(s)));
    string_append(s, "15", 2);
    assert(isAlphaNum(string_c_str(s)));
    assert(!isAlpha(string_c_str(s)));
    assert(!isDigit(string_c_str(s)));
    string_erase(s, 0, 5);
    assert(isAlphaNum(string_c_str(s)));
    assert(!isAlpha(string_c_str(s)));
    assert(isDigit(string_c_str(s)));
    string_free(s);
}

void test_case_conversion(void) {
    String *s = string_new_fromCStr("mOcKiNg HaHa", 12);
    toLowercase(string_c_str(s));
    compareStrs(s, "mocking haha", 12);
    toUppercase(string_c_str(s));
    compareStrs(s, "MOCKING HAHA", 12);
    string_free(s);
}

void test_format(void) {
#if __STDC_VERSION__ >= 199901L
    /* init */
    {
        String *a = string_new_repeatingChar(130, 'a'), *b = string_new_repeatingChar(130, 'b');
        String *s = string_new_withFormat("%s %d %s", a->s, 16000, b->s);
        compareStrs(s, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa 16000 bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb", 267);
        string_free(s);
        string_free(a);
        string_free(b);
    }

    /* insert */
    {
        String *s = string_new();
        string_insert_withFormat(s, s->size - 1, "%c", 'x');
        string_insert_withFormat(s, 0, "%c", 'c');
        string_insert_withFormat(s, 0, "%s", "aa");
        string_insert_withFormat(s, s->size - 1, "%s", "bc");
        string_insert_withFormat(s, string_len(s), "%c", 'd');
        compareStrs(s, "aabccd", 6);
        string_free(s);
    }

    /* append */
    {
        String *s = string_new();
        string_append_withFormat(s, "%s", "aa");
        string_append_withFormat(s, "%s", "bc");
        string_append_withFormat(s, "%c", 'c');
        string_append_withFormat(s, "%c", 0x64);
        compareStrs(s, "aabccd", 6);
        string_free(s);
    }

    /* replace */
    {   
        String *s = string_new();
        string_replace_withFormat(s, s->size - 1, 0, "%s", "xxxxx");
        string_replace_withFormat(s, 0, 0, "%s", "xxxxx");
        string_replace_withFormat(s, 0, 1, "%c%c%c", 0x61, 0x62, 0x63);
        string_replace_withFormat(s, string_len(s), 50, "%c%d", 'h', 3110);
        string_replace_withFormat(s, s->size - 5, 20, "%s", "y");
        string_replace_withFormat(s, s->size - 5, DS_ARG_NOT_APPLICABLE, "%s", "longStr\0\0\0");
        compareStrs(s, "abclongStr", 10);
        string_replace_withFormat(s, 0, DS_ARG_NOT_APPLICABLE, "%c", 46);
        compareStrs(s, ".", 1);
        string_free(s);
    }
#endif
}

int main(void) {
    test_empty_init();
    test_init_repeatingChar();
    test_init_fromCStr();
    test_create_copy();
    test_init_fromString();
    test_push_back();
    test_pop_back();
    test_resize();
    test_reserve();
    test_shrink();
    test_cstr();
    test_insert_cStr();
    test_insert_repeatedChar();
    test_insert_fromString();
    test_append_cStr();
    test_append_fromString();
    test_append_repeatingChar();
    test_replace_cStr();
    test_replace_fromString();
    test_replace_repeatingChar();
    test_remove_char();
    test_erase_chars();
    test_substr();
    test_find_first_of();
    test_find_last_of();
    test_find_first_not_of();
    test_find_last_not_of();
    test_find();
    test_rfind();
    test_split();
    test_has_certain_chars();
    test_case_conversion();
    test_format();
    return 0;
}
