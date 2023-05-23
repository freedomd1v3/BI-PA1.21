#include <stdbool.h>
#include "is_date_valid.h"
#include "struct_date.h"
#include "is_day_out_of_high_bound.h"

int const MONTH_DAYS[] = {31, 28, 31, 30, 31, 30,
                          31, 31, 30, 31, 30, 31};

bool
is_date_valid(struct Date date) {
    if (date.y < 1600)
        return false;
    else if (date.m < 1 || date.m > 12)
        return false;
    else if (date.d < 1 || IS_DAY_OUT_OF_HIGH_BOUND(date))
        return false;
    else if (date.h < 0 || date.h > 23)
        return false;
    else if (date.i < 0 || date.i > 59)
        return false;
    return true;
}
