#include <stdio.h>
#include <stdlib.h>
#include "utest.h"
#include "test_utils.h"
#include "ttree.h"

struct item {
    int key;
};

static int __cmpfunc(void *key1, void *key2)
{
    return (*(int *)key1 - *(int *)key2);
}

static struct item *alloc_item(int val)
{
    struct item *item;

    item = malloc(sizeof(*item));
    if (!item) {
        utest_error("Failed to allocate %zd bytes!", sizeof(*item));
    }

    item->key = val;
    return item;
}

UTEST_FUNCTION(ut_cursor_next, args)
{
    Ttree tree;
    TtreeCursor cursor;
    int num_keys, num_items, i, ret;
    struct item *item;

    num_keys = utest_get_arg(args, 0, INT);
    num_items = utest_get_arg(args, 1, INT);
    UTEST_ASSERT(num_items >= 1);

    ret = ttree_init(&tree, num_keys, __cmpfunc, struct item, key);
    UTEST_ASSERT(ret == 0);
    for (i = 0; i < num_items; i++) {
        item = alloc_item(i + 1);
        UTEST_ASSERT(ttree_insert(&tree, item) == 0);
    }

    UTEST_ASSERT(ret = ttree_cursor_open(&cursor, &tree) == 0);
    UTEST_ASSERT((ret = ttree_cursor_first(&cursor)) == TCSR_OK);
    i = 1;
    while (ret == TCSR_OK) {
        item = ttree_item_from_cursor(&cursor);
        if (!item) {
            UTEST_FAILED("[forward] Failed to get item from cursor on step %d!",
                         i);
        }
        if (item->key != i) {
            UTEST_FAILED("[forward ] Unexpected item with key %d. "
                         "But %d was expected!", item->key, i);
        }

        i++;
        ret = ttree_cursor_next(&cursor);
    }
    if ((i - 1) != num_items) {
        UTEST_FAILED("[forward] Invalid number of iterated items: "
                     "%d. %d was expected!", i - 1, num_items);
    }

    i--;
    UTEST_ASSERT((ret = ttree_cursor_last(&cursor)) == TCSR_OK);
    while (ret == TCSR_OK) {
        item = ttree_item_from_cursor(&cursor);
        if (!item) {
            UTEST_FAILED("[backward] Failed to get item from cursor "
                         "on step %d!", num_items - i);
        }

        printf("-> %d\n", item->key);
        if (item->key != i) {
            UTEST_FAILED("[backward] Unexpected item with key %d. "
                         "But %d was expected!", item->key, i);
        }

        i--;
        ret = ttree_cursor_prev(&cursor);
    }
    if (i != 0) {
        UTEST_FAILED("Unexpected number of iterated items: %d. "
                     "%d was expected", num_items, num_items - i);
    }

    UTEST_PASSED();
}

DEFINE_UTESTS_LIST(tests) = {
    {
        "UTEST_CURSOR_NEXT",
        "Cursor move forward test",
        ut_cursor_next,
        UTEST_ARGS_LIST {
            { "keys", UT_ARG_INT, "Number of keys per T*-tree node" },
            { "total items", UT_ARG_INT, "Number of items in a tree" },
            UTEST_ARGS_LIST_END,
        },
    },

    UTESTS_LIST_END,
};

int main(int argc, char *argv[])
{
    utest_main(tests, argc, argv);
    return 0;
}
