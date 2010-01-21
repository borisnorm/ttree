#ifndef _UTEST_H_
#define _UTEST_H_

#include <stdbool.h>

#define UTEST_FAILED(descr)                     \
    do {                                        \
        printf("[FAILED]\n");                   \
        printf("\tCOND: %s\n", #descr);         \
        return false;                           \
    } while (0)

#define UTEST_PASSED()                          \
    do {                                        \
        printf("[PASSED]\n");                   \
        return true;                            \
    } while (0)

#define UTEST_DESCRIBE(fmt, args...)            \
    do {                                        \
        printf(fmt, ##args);                    \
        fflush(stdout);                         \
    } while (0)

#define UTEST_ASSERT(cond)\
    if (!(cond)) {                              \
        UTEST_FAILED(cond);                     \
    };

#define UTEST_ERR -1
#define UTEST_OK   0

#endif /* !_UTEST_H_ */
