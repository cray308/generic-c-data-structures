#include "str.h"
#include <assert.h>

void compareStrs(String *s, const char *comparison, int size) {
    int i = 0;
    const char *it;
    assert(string_len(s) == size);
    if (size) {
        assert(*string_front(s) == comparison[0] && *string_back(s) == comparison[size-1]);
    } else {
        assert(!string_front(s) && !string_back(s));
    }
    string_iter(s, it) {
        assert(*it == comparison[i++]);
    }
    assert(i == size);
    i = size - 1;
    string_riter(s, it) {
        assert(*it == comparison[i--]);
    }
    assert(i == -1);
}

void test_macros(void) {
    String *s = string_new();

    assert(string_len(s) == 0);
    assert(string_capacity(s) != 0);
    assert(string_empty(s));
    assert(strlen(string_c_str(s)) == 0);
    assert(string_at(s, 10) == NULL);
    compareStrs(s, NULL, 0);

    string_append(s, "hello", 5);
    assert(!string_empty(s));
    assert(strlen(string_c_str(s)) > 0);
    assert(string_at(s, 10) == NULL);
    assert(*(string_at(s, 1)) == 'e');
    compareStrs(s, "hello", 5);
    string_free(s);
}

void test_resizing(void) {
    String *s = string_new();

    string_reserve(s, 32);
    assert(string_capacity(s) == 64);
    string_reserve(s, 66);
    assert(string_capacity(s) == 128);

    string_append(s, "abcdefghijklmnopqrstuvwxyz", -1);
    compareStrs(s, "abcdefghijklmnopqrstuvwxyz", 26);

    string_resize(s, 7, 0);
    compareStrs(s, "abcdefg", 7);

    string_resize(s, 10, 'x');
    compareStrs(s, "abcdefgxxx", 10);

    string_clear(s);
    compareStrs(s, NULL, 0);
    string_free(s);
}

void test_custom_initializers(void) {
    String *s = string_new_fromCStr("abcdefghijklmnopqrstuvwxyz");
    String *s2 = string_createCopy(s);
    compareStrs(s, "abcdefghijklmnopqrstuvwxyz", 26);
    compareStrs(s2, "abcdefghijklmnopqrstuvwxyz", 26);
    string_free(s);
    string_free(s2);
}

void test_erase(void) {
    String *s = string_new_fromCStr("0123456789");

    string_erase(s, 15, 2);
    string_erase(s, 2, 0);
    assert(string_len(s) == 10);
    string_erase(s, 0, 2); /* 23456789 */
    string_erase(s, 6, -1); /* 234567 */
    string_erase(s, 2, 2); /* 2367 */
    string_pop_back(s); /* 236 */
    compareStrs(s, "236", 3);

    string_free(s);
}

void test_replace(void) {
    const char *mystring = "abcdefghijklmnopqrstuvwxyz";
    String *s = string_new_fromCStr(mystring);

    string_replace(s, 0, "xxx", 3);
    compareStrs(s, "xxxdefghijklmnopqrstuvwxyz", 26);
    string_clear(s);

    string_append(s, mystring, -1);
    string_replace(s, 3, "xxx", -1);
    compareStrs(s, "abcxxxghijklmnopqrstuvwxyz", 26);
    string_clear(s);

    string_append(s, mystring, 26);
    string_replace(s, 23, "xxx", 3);
    compareStrs(s, "abcdefghijklmnopqrstuvwxxx", 26);
    string_clear(s);

    string_append(s, mystring, 26);
    string_replace(s, 18, "xxxxxxxxxxxx", -1);
    compareStrs(s, "abcdefghijklmnopqrxxxxxxxxxxxx", 30);
    string_clear(s);

    string_append(s, mystring, 26);
    string_replace(s, 0, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", -1);
    compareStrs(s, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 43);

    string_free(s);
}

void test_insert(void) {
    const char *mystring = "abcdefghijklmnopqrstuvwxyz";
    String *s = string_new_fromCStr(mystring);

    string_insert(s, 0, "xxx", 3);
    compareStrs(s, "xxxabcdefghijklmnopqrstuvwxyz", 29);
    string_clear(s);

    string_append(s, mystring, 26);
    string_insert(s, 3, "xxx", -1);
    compareStrs(s, "abcxxxdefghijklmnopqrstuvwxyz", 29);
    string_clear(s);

    string_append(s, mystring, 26);
    string_insert(s, 23, "xxx", 3);
    compareStrs(s, "abcdefghijklmnopqrstuvwxxxxyz", 29);
    string_clear(s);

    string_append(s, mystring, 26);
    string_insert(s, 18, "xxxxxxxxxxxx", -1);
    compareStrs(s, "abcdefghijklmnopqrxxxxxxxxxxxxstuvwxyz", 38);
    string_clear(s);

    string_append(s, mystring, 26);
    string_insert(s, -1, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", -1);
    compareStrs(s, "abcdefghijklmnopqrstuvwxyxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxz", 69);

    string_free(s);
}

void test_printf(void) {
#if __STDC_VERSION__ >= 199901L
    String *s = string_new();
    string_printf(s, string_len(s), "%d is a cool number.", 10);
    compareStrs(s, "10 is a cool number.", 20);
    string_printf(s, 0, "pi is about %.2f, and ", 3.14);
    compareStrs(s, "pi is about 3.14, and 10 is a cool number.", 42);
    string_free(s);
#endif
}

void test_find(void) {
    String *s = string_new_fromCStr("pi is about 3.14, and 10 is a cool number");

    assert(string_find(s, 0, "3.14", 4) == 12);
    assert(string_find(s, 16, "3.14", 4) == -1);
    assert(string_find(s, 2, "3.14", 0) == 2);
    assert(string_find(s, 0, "is a", -1) == 3);
    assert(string_find(s, 7, "is a", -1) == 25);
    assert(string_find(s, 29, "is a", -1) == -1);
    assert(string_find(s, 0, "is x", 2) == 3);

    string_clear(s);
    string_append(s, "onionionspl", -1);

    assert(string_find(s, 0, "onions", 6) == 3);
    assert(string_rfind(s, -1, "onitx", 3) == 3);
    assert(string_rfind(s, -1, "onion", 5) == 3);
    assert(string_rfind(s, 5, "onion", -1) == 0);
    assert(string_rfind(s, 0, "onion", -1) == -1);

    string_free(s);
}

void test_find_x_of(void) {
    String *s = string_new_fromCStr("Please, replace the vowels in this sentence by asterisks.");
    int found = string_find_first_of(s, 0, "aeiou");
    while (found != -1) {
        *string_at(s, found) = '*';
        found = string_find_first_of(s, found + 1, "aeiou");
    }
    compareStrs(s, "Pl**s*, r*pl*c* th* v*w*ls *n th*s s*nt*nc* by *st*r*sks.", 57);

    string_clear(s);
    string_append(s, "/usr/bin/man", 12);
    found = string_find_last_of(s, -1, "/");
    while (found != -1) {
        *string_at(s, found) = '*';
        found = string_find_last_of(s, found - 1, "/");
    }
    compareStrs(s, "*usr*bin*man", 12);

    string_clear(s);
    string_append(s, "look for non-alphabetic characters...", -1);
    assert(string_find_first_not_of(s, 0, "abcdefghijklmnopqrstuvwxyz ") == 12);

    string_clear(s);
    string_append(s, "Please, erase trailing white-spaces \t \n", 39);
    found = string_find_last_not_of(s, -1, " \t\f\v\n\r");
    assert(found != -1);
    string_erase(s, found + 1, -1);
    compareStrs(s, "Please, erase trailing white-spaces", 35);

    string_free(s);
}

void test_substr(void) {
    String *s2, *s3, *s4;
    String *s = string_new_fromCStr("We think in generalities, but we live in details.");

    s2 = string_substr(s, 3, 5, 1);
    compareStrs(s2, "think", 5);

    s3 = string_substr(s, string_find(s, 0, "live", 4), -1, 1);
    compareStrs(s3, "live in details.", 16);

    string_clear(s);
    string_append(s, string_c_str(s2), string_len(s2));
    string_push_back(s, ' ');
    string_append(s, string_c_str(s3), string_len(s3));
    string_free(s2);
    string_free(s3);
    compareStrs(s, "think live in details.", 22);

    s4 = string_substr(s, -2, 3, -2);
    compareStrs(s4, "sit", 3);
    string_free(s4);
    string_free(s);
}

void test_split(void) {
    int i = 0;
    int len1[] = {22,33};
    int len2[] = {4,3,7,6,10,4,4,5,5};
    const char *cmp1[] = {"this has several words"," definitely more than eight words"};
    const char *cmp2[] = {"this","has","several","words,","definitely","more","than","eight","words"};
    String **split, **it;

    String *s = string_new_fromCStr("this has several words, definitely more than eight words");
    split = string_split(s, ":");
    compareStrs(split[0], "this has several words, definitely more than eight words", 56);
    string_split_free(split);

    split = string_split(s, ",");
    for (it = split; *it; ++it, ++i) {
        assert(i < 2);
        compareStrs(*it, cmp1[i], len1[i]);
    }
    string_split_free(split);

    split = string_split(s, " ");
    i = 0;
    for (it = split; *it; ++it, ++i) {
        assert(i < 9);
        compareStrs(*it, cmp2[i], len2[i]);
    }
    string_split_free(split);

    string_free(s);
}

void test_has_certain_chars(void) {
    String *s = string_new_fromCStr("hello");
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
    String *s = string_new_fromCStr("mOcKiNg HaHa");
    toLowercase(string_c_str(s));
    compareStrs(s, "mocking haha", 12);
    toUppercase(string_c_str(s));
    compareStrs(s, "MOCKING HAHA", 12);
    string_free(s);
}

int main(void) {
    test_macros();
    test_resizing();
    test_custom_initializers();
    test_erase();
    test_replace();
    test_insert();
    test_printf();
    test_find();
    test_find_x_of();
    test_substr();
    test_split();
    test_has_certain_chars();
    test_case_conversion();
    return 0;
}
