#include "stack.h"
#include <assert.h>

typedef struct {
    double val;
    char *str;
} DoubleStr;

void doubleStr_copy(void *_dst, const void *_src) {
    DoubleStr *dst = (DoubleStr*) _dst;
    DoubleStr *src = (DoubleStr*) _src;
    dst->val = src->val;
    if (src->str) {
        dst->str = malloc(strlen(src->str) + 1);
        strcpy(dst->str, src->str);
    } else {
        dst->str = NULL;
    }
}

void doubleStr_del(void *_elt) {
    DoubleStr *elt = (DoubleStr*) _elt;
    if (elt && elt->str) {
        free(elt->str);
    }
}

DSHelper dblstr_helper = {sizeof(DoubleStr), doubleStr_copy, doubleStr_del, NULL};
char *strs[] = {"One", "Two", "Three", "Four", "Five", "Six",
                      "Seven", "Eight", "Nine", "Ten"};

void test_push(Stack *s) {
    assert(stack_empty(s));
    assert(stack_size(s) == 0);
    DoubleStr dbs;
    DoubleStr *ptr = NULL;

    for (int i = 0; i < 10; ++i) {
        dbs.val = (double) i + 0.14159;
        dbs.str = strs[i];
        stack_push(s, &dbs);
        ptr = stack_top(s);
        assert(streq(ptr->str, strs[i]));
        assert(ptr->val >= i && ptr->val <= (i + 1));
        assert(stack_size(s) == i + 1);
    }
    assert(!stack_empty(s));
}

void test_pop(Stack *s) {
    DoubleStr dbs;
    DoubleStr *ptr = NULL;

    int i = 9;
    for (; i > 4; --i) {
        stack_pop(s, &dbs);
        assert(streq(dbs.str, strs[i]));
        assert(dbs.val >= i && dbs.val <= (i + 1));
        free(dbs.str);
        ptr = stack_top(s);
        assert(streq(ptr->str, strs[i-1]));
        assert(stack_size(s) == i);
    }
    assert(stack_size(s) == 5);

    while (stack_pop(s, &dbs)) {
        assert(streq(dbs.str, strs[i]));
        assert(dbs.val >= i && dbs.val <= (i + 1));
        free(dbs.str);
        i--;
    }

    assert(stack_empty(s));
    assert(stack_size(s) == 0);
}

int main(void) {
    Stack *s = stack_new(&dblstr_helper);
    test_push(s);
    test_pop(s);
    stack_free(s);
    return 0;
}
