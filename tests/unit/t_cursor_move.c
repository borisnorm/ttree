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

UTEST_FUNCTION(ut_cursor_insert, args)
{
    Ttree tree;
    TtreeCursor cursor;
    int num_keys, ret, num_items, i;
    struct item *item;
    void *result;

    num_keys = utest_get_arg(args, 0, INT);
    num_items = utest_get_arg(args, 1, INT);
    UTEST_ASSERT(num_items > 1);
    ret = ttree_init(&tree, num_keys, __cmpfunc, struct item, key);
    UTEST_ASSERT(ret >= 0);

    /* Fill the very first node with items, then check the cursor state */
    for (i = num_items; i > 0; i--) {
        item = alloc_item(i);
        result = ttree_lookup(&tree, &item->key, &cursor);
        if (result) {
            UTEST_FAILED("ttree_lookup found item by unexistent key: %d",
                         item->key);
        }

        ttree_insert_at_cursor(&cursor, item);
    }

    /*
     * Now cursor should point to the very first item in a node.
     * I.e. to the position of the last inserted key. In order
     * to check if cursor still iteratible we'll try to iterate
     * backward expecting to get TCSR_END, then we'll try to iterate
     * forward expecting to iterate through exactly <num_keys> items.
     */
    UTEST_ASSERT(ttree_cursor_prev(&cursor) == TCSR_END);
    i = 1;
    do {
        item = ttree_item_from_cursor(&cursor);
        if (!item) {
            UTEST_FAILED("Failed to get item from cursor in step %d!",
                         i - 1);
        }
        if (item->key != i) {
            UTEST_FAILED("[step %d] Expected key %d, but got %d!",
                         i - 1, i, item->key);
        }

        ret = ttree_cursor_next(&cursor);
        i++;
    } while (ret == TCSR_OK);

    UTEST_PASSED();
}

UTEST_FUNCTION(ut_cursor_move, args)
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
        "UTEST_CURSOR_MOVE",
        "Cursor move forward and backward test",
        ut_cursor_move,
        UTEST_ARGS_LIST {
            { "keys", UT_ARG_INT, "Number of keys per T*-tree node" },
            { "total items", UT_ARG_INT, "Number of items in a tree" },
            UTEST_ARGS_LIST_END,
        },
    },
    {
        "UTEST_CURSOR_INSERT",
        "Insertion at cursor test",
        ut_cursor_insert,
        UTEST_ARGS_LIST {
            { "keys", UT_ARG_INT, "Number of keys per T*-tree node" },
            { "items", UT_ARG_INT, "Items" },
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
