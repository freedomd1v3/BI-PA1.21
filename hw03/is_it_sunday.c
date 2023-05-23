#include <stdbool.h>
#include "struct_date.h"

#define SUN 1

bool
is_it_sunday(struct Date date) {
    /* (c) 1882, Christian Zeller */

    switch (date.m) {
        case 1:
            date.m = 13;
            date.y -= 1;
            break;
        case 2:
            date.m = 14;
            date.y -= 1;
    }
    return ((date.d + 13 * (date.m + 1) / 5
             + date.y + date.y / 4 - date.y / 100 + date.y / 400) % 7 == SUN);
}
