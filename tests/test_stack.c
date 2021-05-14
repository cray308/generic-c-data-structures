#include "stack.h"
#include <assert.h>

typedef struct {
    double val;
    char *str;
} DoubleStr;

gen_stack(dbs, DoubleStr)

char *strs[] = {"One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten"};

int main(void) {
    Stack_dbs *s = stack_new(dbs);

    assert(stack_empty(s));
    assert(stack_size(s) == 0);

    DoubleStr *ptr = NULL;
    for (int i = 0; i < 10; ++i) {
        stack_push(dbs, s, ((DoubleStr){.val = (double) i + 0.14159, .str = strs[i]}));
        ptr = stack_top(s);
        assert(streq(ptr->str, strs[i]));
        assert(ptr->val >= i && ptr->val <= (i + 1));
        assert(stack_size(s) == i + 1);
    }
    assert(!stack_empty(s));

    DoubleStr dbs = {0, NULL};
    int i = 9;
    for (; i > 4; --i) {
        stack_pop(dbs, s, &dbs);
        assert(streq(dbs.str, strs[i]));
        assert(dbs.val >= i && dbs.val <= (i + 1));
        ptr = stack_top(s);
        assert(streq(ptr->str, strs[i-1]));
        assert(stack_size(s) == i);
    }
    assert(stack_size(s) == 5);

    while (stack_pop(dbs, s, &dbs)) {
        assert(streq(dbs.str, strs[i]));
        assert(dbs.val >= i && dbs.val <= (i + 1));
        i--;
    }
    assert(stack_empty(s));
    assert(stack_size(s) == 0);

    stack_free(dbs, s);
    return 0;
}
