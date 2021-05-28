#include "deque.h"
#include <assert.h>

gen_deque(int, int, DSDefault_shallowCopy, DSDefault_shallowDelete)
gen_deque(str, char *, DSDefault_deepCopyStr, DSDefault_deepDelete)

#define LEN 151
#define FIRST 0
#define LAST 150

char *strs[LEN] = {"000","001","002","003","004","005","006","007","008","009","010","011","012","013","014",
"015","016","017","018","019","020","021","022","023","024","025","026","027","028","029","030","031","032","033",
"034","035","036","037","038","039","040","041","042","043","044","045","046","047","048","049","050","051","052",
"053","054","055","056","057","058","059","060","061","062","063","064","065","066","067","068","069","070","071",
"072","073","074","075","076","077","078","079","080","081","082","083","084","085","086","087","088","089","090",
"091","092","093","094","095","096","097","098","099","100","101","102","103","104","105","106","107","108","109",
"110","111","112","113","114","115","116","117","118","119","120","121","122","123","124","125","126","127","128",
"129","130","131","132","133","134","135","136","137","138","139","140","141","142","143","144","145","146","147","148","149","150"};

void test_empty(void) {
    Deque_int *qi = deque_new(int);
    Deque_str *qs = deque_new(str);
    assert(deque_empty(qi));
    assert(deque_empty(qs));
    assert(deque_size(qi) == 0);
    assert(deque_size(qs) == 0);
    assert(deque_front(qi) == NULL);
    assert(deque_front(qs) == NULL);
    assert(deque_back(qi) == NULL);
    assert(deque_back(qs) == NULL);
    deque_free(int, qi);
    deque_free(str, qs);
}

void test_push_pop_front(void) {
    Deque_int *qi = deque_new(int);
    Deque_str *qs = deque_new(str);
    int count = 1, i;

    for (i = LEN - 1; i >= 0; --i) {
        deque_push_front(int, qi, i);
        deque_push_front(str, qs, strs[i]);
        assert(deque_size(qi) == count);
        assert(deque_size(qs) == count++);
        assert(*deque_front(qi) == i);
        assert(streq(*deque_front(qs), strs[i]));
        assert(*deque_back(qi) == LAST);
        assert(streq(*deque_back(qs), strs[LAST]));
    }
    assert(*deque_front(qi) == FIRST);
    assert(streq(*deque_front(qs), strs[FIRST]));
    assert(*deque_back(qi) == LAST);
    assert(streq(*deque_back(qs), strs[LAST]));

    i = 0;
    while (!deque_empty(qi)) {
        assert(*deque_front(qi) == i++);
        assert(deque_size(qi) == (LEN - i) + 1);
        assert(*deque_back(qi) == LAST);
        deque_pop_front(int, qi);
    }
    assert(i == LEN);

    i = 0;
    while (!deque_empty(qs)) {
        assert(streq(*deque_front(qs), strs[i++]));
        assert(deque_size(qs) == (LEN - i) + 1);
        assert(streq(*deque_back(qs), strs[LAST]));
        deque_pop_front(str, qs);
    }
    assert(i == LEN);

    assert(deque_size(qi) == 0);
    assert(deque_size(qs) == 0);
    assert(deque_front(qi) == NULL);
    assert(deque_front(qs) == NULL);
    assert(deque_back(qi) == NULL);
    assert(deque_back(qs) == NULL);
    deque_free(int, qi);
    deque_free(str, qs);
}

void test_push_pop_back(void) {
    Deque_int *qi = deque_new(int);
    Deque_str *qs = deque_new(str);
    int i;
    for (i = 0; i < LEN; ++i) {
        deque_push_back(int, qi, i);
        deque_push_back(str, qs, strs[i]);
        assert(deque_size(qi) == i + 1);
        assert(deque_size(qs) == i + 1);
        assert(*deque_front(qi) == FIRST);
        assert(streq(*deque_front(qs), strs[FIRST]));
        assert(*deque_back(qi) == i);
        assert(streq(*deque_back(qs), strs[i]));
    }
    assert(*deque_front(qi) == FIRST);
    assert(streq(*deque_front(qs), strs[FIRST]));
    assert(*deque_back(qi) == LAST);
    assert(streq(*deque_back(qs), strs[LAST]));

    i = LEN - 1;
    while (!deque_empty(qi)) {
        assert(*deque_back(qi) == i);
        assert(deque_size(qi) == i-- + 1);
        assert(*deque_front(qi) == FIRST);
        deque_pop_back(int, qi);
    }
    assert(i == -1);

    i = LEN - 1;
    while (!deque_empty(qs)) {
        assert(streq(*deque_back(qs), strs[i]));
        assert(deque_size(qs) == i-- + 1);
        assert(streq(*deque_front(qs), strs[FIRST]));
        deque_pop_back(str, qs);
    }
    assert(i == -1);

    assert(deque_size(qi) == 0);
    assert(deque_size(qs) == 0);
    assert(deque_front(qi) == NULL);
    assert(deque_front(qs) == NULL);
    assert(deque_back(qi) == NULL);
    assert(deque_back(qs) == NULL);
    deque_free(int, qi);
    deque_free(str, qs);
}

void test_push_front_pop_back(void) {
    Deque_int *qi = deque_new(int);
    Deque_str *qs = deque_new(str);
    int i, count = 1;
    for (i = LEN - 1; i >= 0; --i) {
        deque_push_front(int, qi, i);
        deque_push_front(str, qs, strs[i]);
        assert(deque_size(qi) == count);
        assert(deque_size(qs) == count++);
        assert(*deque_front(qi) == i);
        assert(streq(*deque_front(qs), strs[i]));
        assert(*deque_back(qi) == LAST);
        assert(streq(*deque_back(qs), strs[LAST]));
    }
    assert(*deque_front(qi) == FIRST);
    assert(streq(*deque_front(qs), strs[FIRST]));
    assert(*deque_back(qi) == LAST);
    assert(streq(*deque_back(qs), strs[LAST]));

    i = LEN - 1;
    while (!deque_empty(qi)) {
        assert(*deque_back(qi) == i);
        assert(deque_size(qi) == i-- + 1);
        assert(*deque_front(qi) == FIRST);
        deque_pop_back(int, qi);
    }
    assert(i == -1);

    i = LEN - 1;
    while (!deque_empty(qs)) {
        assert(streq(*deque_back(qs), strs[i]));
        assert(deque_size(qs) == i-- + 1);
        assert(streq(*deque_front(qs), strs[FIRST]));
        deque_pop_back(str, qs);
    }
    assert(i == -1);

    assert(deque_size(qi) == 0);
    assert(deque_size(qs) == 0);
    assert(deque_front(qi) == NULL);
    assert(deque_front(qs) == NULL);
    assert(deque_back(qi) == NULL);
    assert(deque_back(qs) == NULL);
    deque_free(int, qi);
    deque_free(str, qs);
}

void test_push_back_pop_front(void) {
    Deque_int *qi = deque_new(int);
    Deque_str *qs = deque_new(str);
    int i;
    for (i = 0; i < LEN; ++i) {
        deque_push_back(int, qi, i);
        deque_push_back(str, qs, strs[i]);
        assert(deque_size(qi) == i + 1);
        assert(deque_size(qs) == i + 1);
        assert(*deque_front(qi) == FIRST);
        assert(streq(*deque_front(qs), strs[FIRST]));
        assert(*deque_back(qi) == i);
        assert(streq(*deque_back(qs), strs[i]));
    }
    assert(*deque_front(qi) == FIRST);
    assert(streq(*deque_front(qs), strs[FIRST]));
    assert(*deque_back(qi) == LAST);
    assert(streq(*deque_back(qs), strs[LAST]));

    i = 0;
    while (!deque_empty(qi)) {
        assert(*deque_front(qi) == i++);
        assert(deque_size(qi) == (LEN - i) + 1);
        assert(*deque_back(qi) == LAST);
        deque_pop_front(int, qi);
    }
    assert(i == LEN);

    i = 0;
    while (!deque_empty(qs)) {
        assert(streq(*deque_front(qs), strs[i++]));
        assert(deque_size(qs) == (LEN - i) + 1);
        assert(streq(*deque_back(qs), strs[LAST]));
        deque_pop_front(str, qs);
    }
    assert(i == LEN);

    assert(deque_size(qi) == 0);
    assert(deque_size(qs) == 0);
    assert(deque_front(qi) == NULL);
    assert(deque_front(qs) == NULL);
    assert(deque_back(qi) == NULL);
    assert(deque_back(qs) == NULL);
    deque_free(int, qi);
    deque_free(str, qs);
}

void test_mixed(void) {
    const int midFirst = (LEN / 2), midSecond = ((LEN / 2) + 1), lastIdx = LEN - 82;
    Deque_int *qi = deque_new(int);
    Deque_str *qs = deque_new(str);
    int count = 1, i;

    for (i = midFirst; i >= 0; --i) {
        deque_push_front(int, qi, i);
        deque_push_front(str, qs, strs[i]);
        assert(deque_size(qi) == count);
        assert(deque_size(qs) == count++);
        assert(*deque_front(qi) == i);
        assert(streq(*deque_front(qs), strs[i]));
        assert(*deque_back(qi) == midFirst);
        assert(streq(*deque_back(qs), strs[midFirst]));
    }
    assert(*deque_front(qi) == FIRST);
    assert(streq(*deque_front(qs), strs[FIRST]));
    assert(deque_size(qi) == midFirst + 1);
    assert(deque_size(qs) == midFirst + 1);

    for (i = midSecond; i < LEN; ++i) {
        deque_push_back(int, qi, i);
        deque_push_back(str, qs, strs[i]);
        assert(deque_size(qi) == i + 1);
        assert(deque_size(qs) == i + 1);
        assert(*deque_front(qi) == FIRST);
        assert(streq(*deque_front(qs), strs[FIRST]));
        assert(*deque_back(qi) == i);
        assert(streq(*deque_back(qs), strs[i]));
    }
    assert(deque_size(qi) == LEN);
    assert(deque_size(qs) == LEN);
    assert(*deque_back(qi) == LAST);
    assert(streq(*deque_back(qs), strs[LAST]));

    for (i = LEN - 1; i >= LEN - 81; --i) {
        assert(*deque_back(qi) == i);
        assert(deque_size(qi) == i + 1);
        assert(*deque_front(qi) == FIRST);
        deque_pop_back(int, qi);
    }
    for (i = LEN - 1; i >= LEN - 81; --i) {
        assert(streq(*deque_back(qs), strs[i]));
        assert(deque_size(qs) == i + 1);
        assert(streq(*deque_front(qs), strs[FIRST]));
        deque_pop_back(str, qs);
    }

    count = deque_size(qi), i = 0;
    while (!deque_empty(qi)) {
        assert(*deque_front(qi) == i++);
        assert(deque_size(qi) == (count - i) + 1);
        assert(*deque_back(qi) == lastIdx);
        deque_pop_front(int, qi);
    }
    assert(i == count);

    count = deque_size(qs), i = 0;
    while (!deque_empty(qs)) {
        assert(streq(*deque_front(qs), strs[i++]));
        assert(deque_size(qs) == (count - i) + 1);
        assert(streq(*deque_back(qs), strs[lastIdx]));
        deque_pop_front(str, qs);
    }
    assert(i == count);
    assert(deque_size(qi) == 0);
    assert(deque_size(qs) == 0);
    assert(deque_front(qi) == NULL);
    assert(deque_front(qs) == NULL);
    assert(deque_back(qi) == NULL);
    assert(deque_back(qs) == NULL);

    /* for valgrind: test non-empty free */
    for (i = 0; i < 20; ++i) {
        deque_push_back(int, qi, i);
        deque_push_back(str, qs, strs[i]);
    }
    for (i = 0; i < 10; ++i) {
        deque_pop_front(int, qi);
        deque_pop_front(str, qs);
    }
    deque_free(int, qi);
    deque_free(str, qs);
}

int main(void) {
    test_empty();
    test_push_pop_front();
    test_push_pop_back();
    test_push_front_pop_back();
    test_push_back_pop_front();
    test_mixed();
    return 0;
}
