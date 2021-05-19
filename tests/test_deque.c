#include "deque.h"
#include <assert.h>

gen_deque(int, int)
gen_deque_customCopyDelete(str, char *, DSDefault_deepCopyStr, DSDefault_deepDelete)

#define LEN 151
#define FIRST 0
#define LAST 150
char strs[LEN][16] = {};

const int midFirst = (LEN / 2);
const int midSecond = ((LEN / 2) + 1);

void test_empty(void) {
    Deque_int *qi = deque_new(int);
    Deque_str *qs = deque_new(str);
    assert(deque_empty(qi) && deque_empty(qs));
    assert(deque_size(qi) == 0 && deque_size(qs) == 0);
    assert(deque_front(qi) == NULL && deque_front(qs) == NULL);
    assert(deque_back(qi) == NULL && deque_back(qs) == NULL);
    deque_free(int, qi);
    deque_free(str, qs);
}

void test_push_pop_front(void) {
    Deque_int *qi = deque_new(int);
    Deque_str *qs = deque_new(str);
    {
        int count = 1;
        for (int i = LEN - 1; i >= 0; --i) {
            deque_push_front(int, qi, i);
            deque_push_front(str, qs, strs[i]);
            assert(deque_size(qi) == count && deque_size(qs) == count++);
            assert(*deque_front(qi) == i && streq(*deque_front(qs), strs[i]));
            assert(*deque_back(qi) == LAST && streq(*deque_back(qs), strs[LAST]));
        }
    }

    assert(*deque_front(qi) == FIRST && streq(*deque_front(qs), strs[FIRST]));
    assert(*deque_back(qi) == LAST && streq(*deque_back(qs), strs[LAST]));

    {
        int i = 0;
        while (!deque_empty(qi)) {
            assert(*deque_front(qi) == i++);
            assert(deque_size(qi) == (LEN - i) + 1);
            assert(*deque_back(qi) == LAST);
            deque_pop_front(int, qi);
        }
        assert(i == LEN);
    }
    {
        int i = 0;
        while (!deque_empty(qs)) {
            assert(streq(*deque_front(qs), strs[i++]));
            assert(deque_size(qs) == (LEN - i) + 1);
            assert(streq(*deque_back(qs), strs[LAST]));
            deque_pop_front(str, qs);
        }
        assert(i == LEN);
    }

    assert(deque_size(qi) == 0 && deque_size(qs) == 0);
    assert(deque_front(qi) == NULL && deque_front(qs) == NULL);
    assert(deque_back(qi) == NULL && deque_back(qs) == NULL);
    deque_free(int, qi);
    deque_free(str, qs);
}

void test_push_pop_back(void) {
    Deque_int *qi = deque_new(int);
    Deque_str *qs = deque_new(str);
    for (int i = 0; i < LEN; ++i) {
        deque_push_back(int, qi, i);
        deque_push_back(str, qs, strs[i]);
        assert(deque_size(qi) == i + 1 && deque_size(qs) == i + 1);
        assert(*deque_front(qi) == FIRST && streq(*deque_front(qs), strs[FIRST]));
        assert(*deque_back(qi) == i && streq(*deque_back(qs), strs[i]));
    }

    assert(*deque_front(qi) == FIRST && streq(*deque_front(qs), strs[FIRST]));
    assert(*deque_back(qi) == LAST && streq(*deque_back(qs), strs[LAST]));

    {
        int i = LEN - 1;
        while (!deque_empty(qi)) {
            assert(*deque_back(qi) == i);
            assert(deque_size(qi) == i-- + 1);
            assert(*deque_front(qi) == FIRST);
            deque_pop_back(int, qi);
        }
        assert(i == -1);
    }
    {
        int i = LEN - 1;
        while (!deque_empty(qs)) {
            assert(streq(*deque_back(qs), strs[i]));
            assert(deque_size(qs) == i-- + 1);
            assert(streq(*deque_front(qs), strs[FIRST]));
            deque_pop_back(str, qs);
        }
        assert(i == -1);
    }

    assert(deque_size(qi) == 0 && deque_size(qs) == 0);
    assert(deque_front(qi) == NULL && deque_front(qs) == NULL);
    assert(deque_back(qi) == NULL && deque_back(qs) == NULL);
    deque_free(int, qi);
    deque_free(str, qs);
}

void test_push_front_pop_back(void) {
    Deque_int *qi = deque_new(int);
    Deque_str *qs = deque_new(str);
    {
        int count = 1;
        for (int i = LEN - 1; i >= 0; --i) {
            deque_push_front(int, qi, i);
            deque_push_front(str, qs, strs[i]);
            assert(deque_size(qi) == count && deque_size(qs) == count++);
            assert(*deque_front(qi) == i && streq(*deque_front(qs), strs[i]));
            assert(*deque_back(qi) == LAST && streq(*deque_back(qs), strs[LAST]));
        }
    }

    assert(*deque_front(qi) == FIRST && streq(*deque_front(qs), strs[FIRST]));
    assert(*deque_back(qi) == LAST && streq(*deque_back(qs), strs[LAST]));

    {
        int i = LEN - 1;
        while (!deque_empty(qi)) {
            assert(*deque_back(qi) == i);
            assert(deque_size(qi) == i-- + 1);
            assert(*deque_front(qi) == FIRST);
            deque_pop_back(int, qi);
        }
        assert(i == -1);
    }
    {
        int i = LEN - 1;
        while (!deque_empty(qs)) {
            assert(streq(*deque_back(qs), strs[i]));
            assert(deque_size(qs) == i-- + 1);
            assert(streq(*deque_front(qs), strs[FIRST]));
            deque_pop_back(str, qs);
        }
        assert(i == -1);
    }

    assert(deque_size(qi) == 0 && deque_size(qs) == 0);
    assert(deque_front(qi) == NULL && deque_front(qs) == NULL);
    assert(deque_back(qi) == NULL && deque_back(qs) == NULL);
    deque_free(int, qi);
    deque_free(str, qs);
}

void test_push_back_pop_front(void) {
    Deque_int *qi = deque_new(int);
    Deque_str *qs = deque_new(str);
    for (int i = 0; i < LEN; ++i) {
        deque_push_back(int, qi, i);
        deque_push_back(str, qs, strs[i]);
        assert(deque_size(qi) == i + 1 && deque_size(qs) == i + 1);
        assert(*deque_front(qi) == FIRST && streq(*deque_front(qs), strs[FIRST]));
        assert(*deque_back(qi) == i && streq(*deque_back(qs), strs[i]));
    }

    assert(*deque_front(qi) == FIRST && streq(*deque_front(qs), strs[FIRST]));
    assert(*deque_back(qi) == LAST && streq(*deque_back(qs), strs[LAST]));

    {
        int i = 0;
        while (!deque_empty(qi)) {
            assert(*deque_front(qi) == i++);
            assert(deque_size(qi) == (LEN - i) + 1);
            assert(*deque_back(qi) == LAST);
            deque_pop_front(int, qi);
        }
        assert(i == LEN);
    }
    {
        int i = 0;
        while (!deque_empty(qs)) {
            assert(streq(*deque_front(qs), strs[i++]));
            assert(deque_size(qs) == (LEN - i) + 1);
            assert(streq(*deque_back(qs), strs[LAST]));
            deque_pop_front(str, qs);
        }
        assert(i == LEN);
    }

    assert(deque_size(qi) == 0 && deque_size(qs) == 0);
    assert(deque_front(qi) == NULL && deque_front(qs) == NULL);
    assert(deque_back(qi) == NULL && deque_back(qs) == NULL);
    deque_free(int, qi);
    deque_free(str, qs);
}

void test_mixed(void) {
    Deque_int *qi = deque_new(int);
    Deque_str *qs = deque_new(str);
    {
        int count = 1;
        for (int i = midFirst; i >= 0; --i) {
            deque_push_front(int, qi, i);
            deque_push_front(str, qs, strs[i]);
            assert(deque_size(qi) == count && deque_size(qs) == count++);
            assert(*deque_front(qi) == i && streq(*deque_front(qs), strs[i]));
            assert(*deque_back(qi) == midFirst && streq(*deque_back(qs), strs[midFirst]));
        }
    }

    assert(*deque_front(qi) == FIRST && streq(*deque_front(qs), strs[FIRST]));
    assert(deque_size(qi) == midFirst + 1 && deque_size(qs) == midFirst + 1);

    for (int i = midSecond; i < LEN; ++i) {
        deque_push_back(int, qi, i);
        deque_push_back(str, qs, strs[i]);
        assert(deque_size(qi) == i + 1 && deque_size(qs) == i + 1);
        assert(*deque_front(qi) == FIRST && streq(*deque_front(qs), strs[FIRST]));
        assert(*deque_back(qi) == i && streq(*deque_back(qs), strs[i]));
    }

    assert(deque_size(qi) == LEN && deque_size(qs) == LEN);
    assert(*deque_back(qi) == LAST && streq(*deque_back(qs), strs[LAST]));

    for (int i = LEN - 1; i >= LEN - 81; --i) {
        assert(*deque_back(qi) == i);
        assert(deque_size(qi) == i + 1);
        assert(*deque_front(qi) == FIRST);
        deque_pop_back(int, qi);
    }
    for (int i = LEN - 1; i >= LEN - 81; --i) {
        assert(streq(*deque_back(qs), strs[i]));
        assert(deque_size(qs) == i + 1);
        assert(streq(*deque_front(qs), strs[FIRST]));
        deque_pop_back(str, qs);
    }

    const int lastIdx = LEN - 82;
    {
        const int count = deque_size(qi);
        int i = 0;
        while (!deque_empty(qi)) {
            assert(*deque_front(qi) == i++);
            assert(deque_size(qi) == (count - i) + 1);
            assert(*deque_back(qi) == lastIdx);
            deque_pop_front(int, qi);
        }
        assert(i == count);
    }
    {
        const int count = deque_size(qs);
        int i = 0;
        while (!deque_empty(qs)) {
            assert(streq(*deque_front(qs), strs[i++]));
            assert(deque_size(qs) == (count - i) + 1);
            assert(streq(*deque_back(qs), strs[lastIdx]));
            deque_pop_front(str, qs);
        }
        assert(i == count);
    }

    assert(deque_size(qi) == 0 && deque_size(qs) == 0);
    assert(deque_front(qi) == NULL && deque_front(qs) == NULL);
    assert(deque_back(qi) == NULL && deque_back(qs) == NULL);
    deque_free(int, qi);
    deque_free(str, qs);
}

void test_nonempty_free(void) {
    Deque_str *qs = deque_new(str);
    for (int i = 0; i < 50; ++i) {
        deque_push_back(str, qs, strs[i]);
    }
    for (int i = 0; i < 10; ++i) {
        deque_pop_front(str, qs);
    }
    deque_free(str, qs);
}

int main(void) {
    for (int i = 0; i < LEN; ++i) {
        snprintf(strs[i], 16, "%d", i);
    }
    test_empty();
    test_push_pop_front();
    test_push_pop_back();
    test_push_front_pop_back();
    test_push_back_pop_front();
    test_mixed();
    test_nonempty_free();
    return 0;
}
