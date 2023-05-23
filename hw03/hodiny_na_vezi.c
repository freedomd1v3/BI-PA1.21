#include <stdbool.h>
#include "struct_date.h"
#include "is_date_valid.h"
#include "is_end_later.h"
#include "is_day_out_of_high_bound.h"
#include "is_it_sunday.h"
#ifndef __PROGTEST__
#include <assert.h>
#endif  /* __PROGTEST__ */

int
bells(int y1, int m1, int d1, int h1, int i1,
      int y2, int m2, int d2, int h2, int i2,
      long long * b1, long long * b2) { /* b1 - minutes, b2 - hours */
    struct Date current, end;
    current.y = y1, current.m = m1, current.d = d1, current.h = h1, current.i = i1;
    end.y = y2, end.m = m2, end.d = d2, end.h = h2, end.i = i2;

    if (!(is_date_valid(current) && is_date_valid(end)) || !IS_END_LATER(current, end))
        return 0;

    *b1 = 0;
    *b2 = 0;

    /* We always want to have current.i dividable by 15 */
    if (current.i % 15)
        current.i += 15 - current.i % 15;

    while (IS_END_LATER(current, end)) {
        /* Fixing the date */
        if (current.i > 59) {
            current.i = 0;
            current.h += 1;
        }
        if (current.h > 23) {
            current.h = 0;
            current.d += 1;
        }
        if (IS_DAY_OUT_OF_HIGH_BOUND(current)) {
            current.d = 1;
            current.m += 1;
        }
        if (current.m > 12) {
            current.m = 1;
            current.y += 1;
        }

        /* We don't have any bells on sundays */
        if (is_it_sunday(current)) {
            current.h = 0;
            current.i = 0;
            current.d += 1;
            continue;
        }

        /* Skipping the entire day (HUGE speedup!)
         * (btw I trust the compiler to optimize those variables
         * and NOT to initialize them during every loop instance) */
        int full_day_b1 = 240, full_day_b2 = 156;
        if ((end.y != current.y || end.m != current.m || end.d != current.d) && !current.h && !current.i) {
            *b1 += full_day_b1;
            *b2 += full_day_b2;
            current.d += 1;
            continue;
        }

        /* Trivial algorithm, used when we can't skip the day */
        if (!(current.i / 15)) {
            *b1 += 4;
            *b2 += !(current.h % 12) ? 12 : current.h % 12;
        }
        else
            *b1 += current.i / 15;

        current.i += 15;
    }

    return 1;
}

#ifndef __PROGTEST__
int
main(void) {
    long long b1, b2;

    assert(bells(2022, 10, 1, 13, 15,
                 2022, 10, 1, 18, 45, &b1, &b2) == 1
           && b1 == 56
           && b2 == 20);
    assert(bells(2022, 10, 3, 13, 15,
                 2022, 10, 4, 11, 20, &b1, &b2) == 1
           && b1 == 221
           && b2 == 143);
    assert(bells(2022, 10, 1, 13, 15,
                 2022, 10, 2, 11, 20, &b1, &b2) == 1
           && b1 == 106
           && b2 == 65);
    assert(bells (2022, 10, 2, 13, 15,
                  2022, 10, 3, 11, 20, &b1, &b2) == 1
           && b1 == 115
           && b2 == 78);
    assert(bells(2022, 10, 1, 13, 15,
                 2022, 10, 3, 11, 20, &b1, &b2) == 1
           && b1 == 221
           && b2 == 143);
    assert(bells(2022, 1, 1, 13, 15,
                 2022, 10, 5, 11, 20, &b1, &b2) == 1
           && b1 == 56861
           && b2 == 36959);
    assert(bells(2019, 1, 1, 13, 15,
                 2019, 10, 5, 11, 20, &b1, &b2) == 1
           && b1 == 57101
           && b2 == 37115);
    assert(bells(2024, 1, 1, 13, 15,
                 2024, 10, 5, 11, 20, &b1, &b2) == 1
           && b1 == 57341
           && b2 == 37271);
    assert(bells(1900, 1, 1, 13, 15,
                 1900, 10, 5, 11, 20, &b1, &b2) == 1
           && b1 == 57101
           && b2 == 37115);
    assert(bells(2022, 10, 1, 0, 0,
                 2022, 10, 1, 12, 0, &b1, &b2) == 1
           && b1 == 124
           && b2 == 90);
    assert(bells(2022, 10, 1, 0, 15,
                 2022, 10, 1, 0, 25, &b1, &b2) == 1
           && b1 == 1
           && b2 == 0);
    assert(bells(2022, 10, 1, 12, 0,
                 2022, 10, 1, 12, 0, &b1, &b2) == 1
           && b1 == 4
           && b2 == 12);
    assert(bells(2022, 11, 1, 12, 0,
                 2022, 10, 1, 12, 0, &b1, &b2) == 0
           && b1 == 4
           && b2 == 12);
    assert(bells(2022, 10, 32, 12, 0,
                 2022, 11, 10, 12, 0, &b1, &b2) == 0
           && b1 == 4
           && b2 == 12);
    assert(bells(2100, 2, 29, 12, 0,
                 2100, 2, 29, 12, 0, &b1, &b2) == 0
           && b1 == 4
           && b2 == 12);
    assert(bells(2000, 2, 29, 12, 0,
                 2000, 2, 29, 12, 0, &b1, &b2) == 1
           && b1 == 4
           && b2 == 12);
    assert(bells(2004, 2, 29, 12, 0,
                 2004, 2, 29, 12, 0, &b1, &b2) == 1
           && b1 == 0
           && b2 == 0);
    return 0;
}
#endif  /* __PROGTEST__ */
