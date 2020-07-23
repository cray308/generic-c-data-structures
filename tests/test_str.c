#include "str.h"
#include <assert.h>

void test_macros(void) {
    String *s = string_new(STR_INIT_NONE);
    assert(string_len(s) == 0);
    assert(string_capacity(s) != 0);
    assert(string_empty(s));

    const char *cstr = string_c_str(s);
    assert(strlen(cstr) == 0);

    char c = string_at(s, 10);
    assert(c == 0);

    char *ptr = string_front(s);
    assert(ptr == NULL);
    ptr = string_back(s);
    assert(ptr == NULL);

    int counter = 0;
    string_iter(s, ptr) {
        counter++;
    }
    assert(counter == 0);

    counter = 0;
    string_riter(s, ptr) {
        counter++;
    }
    assert(counter == 0);

    string_append(s, "hello", 5);

    assert(string_len(s) == 5);
    assert(!string_empty(s));

    cstr = string_c_str(s);
    assert(strlen(cstr) > 0);

    c = string_at(s, 10);
    assert(c == 0);
    c = string_at(s, 1);
    assert(c == 'e');

    ptr = string_front(s);
    assert(*ptr == 'h');
    ptr = string_back(s);
    assert(*ptr == 'o');

    counter = 0;
    string_iter(s, ptr) {
        counter++;
    }
    assert(counter == 5);

    counter = 0;
    string_riter(s, ptr) {
        counter++;
    }
    assert(counter == 5);

    string_free(s);
}

void test_resizing(void) {
    String *s = string_new(STR_INIT_NONE);

    int old = string_capacity(s);

    string_reserve(s, old >> 1);
    assert(string_capacity(s) == old);

    string_reserve(s, old + 2);
    assert(string_capacity(s) > old);

    const char *mystring = "abcdefghijklmnopqrstuvwxyz";
    string_append(s, mystring, 26);

    int counter = 0;
    const char *ptr;
    string_iter(s, ptr) {
        counter++;
    }
    assert(counter == 26);

    string_resize(s, 7, 0);
    assert(string_len(s) == 7);
    ptr = string_front(s);
    assert(*ptr == 'a');
    ptr = string_back(s);
    assert(*ptr == 'g');

    counter = 0;
    string_iter(s, ptr) {
        counter++;
    }
    assert(counter == 7);

    string_resize(s, 10, 'x');
    assert(string_len(s) == 10);
    ptr = string_front(s);
    assert(*ptr == 'a');
    ptr = string_back(s);
    assert(*ptr == 'x');

    char c;
    for (int i = 0; i < 10; ++i) {
        c = string_at(s, i);
        if (i < 7) {
            assert(c == mystring[i]);
        } else {
            assert(c == 'x');
        }
    }

    string_clear(s);
    assert(string_len(s) == 0);
    ptr = string_c_str(s);
    assert(strlen(ptr) == 0);
    string_free(s);
}

void test_custom_initializers(void) {
    String *s = string_new(STR_INIT_CSTR, "abcdefghijklmnopqrstuvwxyz");
    String *s2 = string_new(STR_INIT_STRING, s);

    assert(streq(string_c_str(s), string_c_str(s2)));
    string_free(s);
    string_free(s2);
}

void test_erase(void) {
    String *s = string_new(STR_INIT_NONE);

    for (unsigned char i = 0x30; i < 0x3A; ++i) {
        string_push_back(s, i);
    }
    assert(string_len(s) == 10);

    string_erase(s, 15, 2);
    assert(string_len(s) == 10);
    string_erase(s, 2, 0);
    assert(string_len(s) == 10);

    string_erase(s, 0, 2);
    assert(string_len(s) == 8);
    char *ptr = string_front(s);
    assert(*ptr == '2');

    string_erase(s, 6, 20);
    assert(string_len(s) == 6);
    ptr = string_back(s);
    assert(*ptr == '7');

    string_erase(s, 2, 2);
    assert(string_len(s) == 4);
    char c = string_at(s, 2);
    assert(c == '6');

    string_pop_back(s);
    assert(string_len(s) == 3);
    ptr = string_back(s);
    assert(*ptr == '6');

    string_free(s);
}

void test_replace(void) {
    String *s = string_new(STR_INIT_CSTR, "abcdefghijklmnopqrstuvwxyz");
    const char *mystring = "abcdefghijklmnopqrstuvwxyz";

    const char *comparison[] = {
        "xxxdefghijklmnopqrstuvwxyz",
        "abcxxxghijklmnopqrstuvwxyz",
        "abcdefghijklmnopqrstuvwxxx",
        "abcdefghijklmnopqrxxxxxxxxxxxx",
        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"};

    string_replace(s, 0, "xxx", 3);
    assert(string_len(s) == 26);
    assert(streq(s->s, comparison[0]));
    string_clear(s);

    string_append(s, mystring, 26);
    string_replace(s, 3, "xxx", 3);
    assert(string_len(s) == 26);
    assert(streq(s->s, comparison[1]));
    string_clear(s);

    string_append(s, mystring, 26);
    string_replace(s, 23, "xxx", 3);
    assert(string_len(s) == 26);
    assert(streq(s->s, comparison[2]));
    string_clear(s);

    string_append(s, mystring, 26);
    string_replace(s, 18, "xxxxxxxxxxxx", 12);
    assert(string_len(s) == 30);
    assert(streq(s->s, comparison[3]));
    string_clear(s);

    string_append(s, mystring, 26);
    string_replace(s, 0, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 43);
    assert(string_len(s) == 43);
    assert(streq(s->s, comparison[4]));

    string_free(s);
}

void test_insert(void) {
    String *s = string_new(STR_INIT_CSTR, "abcdefghijklmnopqrstuvwxyz");
    const char *mystring = "abcdefghijklmnopqrstuvwxyz";

    const char *comparison[] = {
        "xxxabcdefghijklmnopqrstuvwxyz",
        "abcxxxdefghijklmnopqrstuvwxyz",
        "abcdefghijklmnopqrstuvwxxxxyz",
        "abcdefghijklmnopqrxxxxxxxxxxxxstuvwxyz",
        "abcdefghijklmnopqrstuvwxyxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxz"
    };

    string_insert(s, 0, "xxx", 3);
    assert(string_len(s) == 29);
    assert(streq(s->s, comparison[0]));
    string_clear(s);

    string_append(s, mystring, 26);
    string_insert(s, 3, "xxx", 3);
    assert(string_len(s) == 29);
    assert(streq(s->s, comparison[1]));
    string_clear(s);

    string_append(s, mystring, 26);
    string_insert(s, 23, "xxx", 3);
    assert(string_len(s) == 29);
    assert(streq(s->s, comparison[2]));
    string_clear(s);

    string_append(s, mystring, 26);
    string_insert(s, 18, "xxxxxxxxxxxx", 12);
    assert(string_len(s) == 38);
    assert(streq(s->s, comparison[3]));
    string_clear(s);

    string_append(s, mystring, 26);
    string_insert(s, -1, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 43);
    assert(string_len(s) == 69);
    assert(streq(s->s, comparison[4]));

    string_free(s);
}

void test_printf(void) {
    String *s = string_new(STR_INIT_NONE);
    const char *comparison[] = {"10 is a cool number.",
    "pi is about 3.14, and 10 is a cool number."};

    string_printf(s, STRING_END(s), "%d is a cool number.", 10);
    assert(streq(s->s, comparison[0]));

    string_printf(s, 0, "pi is about %.2f, and ", 3.14);
    assert(streq(s->s, comparison[1]));

    string_free(s);
}

void test_find(void) {
    String *s = string_new(STR_INIT_CSTR, "pi is about 3.14, and 10 is a cool number");

    int pos = string_find(s, 0, "3.14", 4);
    assert(pos == 12);

    pos = string_find(s, pos + 4, "3.14", 4);
    assert(pos == -1);

    pos = string_find(s, 2, "3.14", 0);
    assert(pos == 2);

    pos = string_find(s, 0, "is a", 4);
    assert(pos == 3);

    pos = string_find(s, pos + 4, "is a", 4);
    assert(pos == 25);

    pos = string_find(s, pos + 4, "is a", 4);
    assert(pos == -1);

    string_clear(s);
    string_append(s, "onionionspl", 15);

    pos = string_find(s, 0, "onions", 6);
    assert(pos == 3);

    pos = string_rfind(s, -1, "onion", 6);
    assert(pos == 3);

    pos = string_rfind(s, 5, "onion", 6);
    assert(pos == 0);

    pos = string_rfind(s, pos, "onion", 6);
    assert(pos == -1);

    string_free(s);
}

void test_find_x_of(void) {
    const char *comparison[] = {
        "Pl**s*, r*pl*c* th* v*w*ls *n th*s s*nt*nc* by *st*r*sks.",
        "*usr*bin*man",
        "Please, erase trailing white-spaces"
    };


    String *s = string_new(STR_INIT_CSTR, "Please, replace the vowels in this sentence by asterisks.");

    char *ptr;
    int found = 0;
    found = string_find_first_of(s, found, "aeiou");
    while (found != -1) {
        ptr = string_ref(s, found);
        *ptr = '*';
        found = string_find_first_of(s, found + 1, "aeiou");
    }

    assert(streq(string_c_str(s), comparison[0]));

    string_clear(s);
    string_append(s, "/usr/bin/man", 20);

    found = string_len(s);
    found = string_find_last_of(s, found, "/");
    while (found != -1) {
        ptr = string_ref(s, found);
        *ptr = '*';
        found = string_find_last_of(s, found - 1, "/");
    }

    assert(streq(string_c_str(s), comparison[1]));

    string_clear(s);
    string_append(s, "look for non-alphabetic characters...", 40);

    found = string_find_first_not_of(s, 0, "abcdefghijklmnopqrstuvwxyz ");
    assert(found != -1);
    assert(found == 12);

    string_clear(s);
    string_append(s, "Please, erase trailing white-spaces \t \n", 50);

    found = string_find_last_not_of(s, string_len(s), " \t\f\v\n\r");
    assert(found != -1);

    string_erase(s, found, string_len(s));
    assert(streq(string_c_str(s), comparison[2]));
    string_free(s);
}

void test_substr(void) {
    String *s = string_new(STR_INIT_CSTR, "We think in generalities, but we live in details.");

    String *s2 = string_substr(s, 3, 5, 1);
    assert(streq(string_c_str(s2), "think"));

    int pos = string_find(s, 0, "live", 4);
    assert(pos >= 0);

    String *s3 = string_substr(s, pos, -1, 1);
    assert(streq(string_c_str(s3), "live in details."));

    string_clear(s);
    string_printf(s, 0, "%s %s", string_c_str(s2), string_c_str(s3));

    string_free(s2);
    string_free(s3);

    assert(streq(string_c_str(s), "think live in details."));
    String *s4 = string_substr(s, 7, -1, -2);
    assert(streq(string_c_str(s4), "kite"));
    string_free(s4);

    string_free(s);
}

void test_split(void) {
    const char *cmp1[] = {
        "this has several words",
        " definitely more than eight words"
    };
    const char *cmp2[] = {
        "this",
        "has",
        "several",
        "words,",
        "definitely",
        "more",
        "than",
        "eight",
        "words"
    };

    String *s = string_new(STR_INIT_CSTR, "this has several words, definitely more than eight words");

    int len = 0;
    String **split = string_split(s, ":", &len);
    assert(len == 1);
    assert(streq(string_c_str(split[0]), string_c_str(s)));
    string_split_free(split, len);

    split = string_split(s, ",", &len);
    assert(len == 2);
    for (int i = 0; i < len; ++i) {
        assert(streq(string_c_str(split[i]), cmp1[i]));
    }
    string_split_free(split, len);

    split = string_split(s, " ", &len);
    assert(len == 9);
    for (int i = 0; i < len; ++i) {
        assert(streq(string_c_str(split[i]), cmp2[i]));
    }
    string_split_free(split, len);

    string_free(s);
}

void test_has_certain_chars(void) {
    String *s = string_new(STR_INIT_CSTR, "hello");
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
    String *s = string_new(STR_INIT_CSTR, "mOcKiNg HaHa");

    toLowercase(string_c_str(s));
    const char *ptr;
    string_iter(s, ptr) {
        if (isalpha(*ptr)) {
            assert(islower(*ptr));
        }
    }

    toUppercase(string_c_str(s));
    string_iter(s, ptr) {
        if (isalpha(*ptr)) {
            assert(isupper(*ptr));
        }
    }

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
    test_split();
    test_has_certain_chars();
    test_case_conversion();
    return 0;
}
