#include "defaults.h"
#include "map.h"

char *days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

char *months[] = {"January", "February", "March", "April", "May", "June", "July", "August",
                  "September", "October", "November", "December"};

char *words[] = {"cellar", "fold", "criminal", "strict", "trend", "assume", "union", "tropical",
                 "abundant", "fraction", "key", "me", "freighter", "wrong", "investigation",
                 "construct", "colleague", "formulate", "simplicity", "fever", "discipline",
                 "genetic", "shorts", "understand", "recording", "hobby", "prove", "bucket",
                 "revenge", "ego", "tablet", "minute", "teenager", "crevice", "sympathetic",
                 "crutch", "rib", "confession", "right", "star", "fun", "exclude", "senior",
                 "suspect", "reliable", "harm", "consciousness", "loud", "necklace", "elaborate",
                 "desk", "plagiarize", "restless", "middle", "federation", "mosque", "sweet", "red",
                 "bring", "lemon", "thick", "unlikely", "store", "plot", "census", "prevent",
                 "paralyzed", "movie", "winner", "mild", "foundation", "exploit", "dismiss", "die",
                 "owl", "studio", "represent", "feminine", "school", "mail", "disaster", "salmon",
                 "vegetation", "stage", "hate", "ear", "staff", "enjoy", "arm", "reservoir", "appeal",
                 "scatter", "lounge", "cucumber", "ally", "faithful", "chapter", "frown", "sell"};

void test_insert_find_string(void) {
    Map *m = map_new(STRING, &int_helper);
    assert(map_empty(m));

    for (int i = 0; i < 7; ++i) {
        map_insert(m, days[i], &i);
        assert(map_size(m) == i + 1);
    }
    assert(!map_empty(m));
    int *ptr = NULL;

    for (int i = 0; i < 7; ++i) {
        ptr = map_find(m, days[i]);
        assert(ptr != NULL);
        assert((*ptr) == i);
    }

    char *s = "hi";
    ptr = map_find(m, s);
    assert(ptr == NULL);

    map_clear(m);
    assert(map_size(m) == 0);
    assert(map_empty(m));

    map_free(m);
}

void test_insert_find_int(void) {
    Map *m = map_new(NUMBER, &str_ptr_helper);
    char **ptr = NULL;

    for (int i = 0; i < 12; ++i) {
        map_insert(m, &i, &months[i]);
        assert(map_size(m) == i + 1);
    }

    for (int i = 0; i < 12; ++i) {
        ptr = map_find(m, &i);
        assert(streq(*ptr, months[i]));
    }

    map_clear(m);
    assert(map_size(m) == 0);
    assert(map_empty(m));

    map_free(m);
}

void test_resizing_deletion(void) {
    Map *m = map_new(STRING, &int_helper);

    const int len = 99;
    for (int i = 0; i < len; ++i) {
        map_insert(m, words[i], &i);
    }

    assert(m->cap != 32);
    int *ptr;

    for (int i = 0; i < len; ++i) {
        ptr = map_find(m, words[i]);
        assert(*ptr == i);
    }

    int key = len - 1;
    while (!map_empty(m)) {
        map_erase_key(m, words[key]);
        assert(map_size(m) == key);
        ptr = map_find(m, words[key]);
        assert(ptr == NULL);
        key--;
    }

    assert(map_size(m) == 0);

    map_free(m);
}

void test_iter(void) {
    Map *m = map_new(NUMBER, &str_ptr_helper);

    for (int i = 0; i < 12; ++i) {
        map_insert(m, &i, &months[i]);
        assert(map_size(m) == i + 1);
    }
    int counter = 0;
    char **ptr;

    map_iter(m, ptr) {
        assert(ptr != NULL);
        assert(strlen(*ptr) > 0);
        ++counter;
    }
    assert(counter == 12);

    map_free(m);
}

int main(void) {
    test_insert_find_string();
    test_insert_find_int();
    test_resizing_deletion();
    test_iter();
    return 0;
}
