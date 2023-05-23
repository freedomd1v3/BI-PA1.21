#ifndef IS_DAY_OUT_OF_HIGH_BOUND_H
#define IS_DAY_OUT_OF_HIGH_BOUND_H

#include "is_year_leap.h"
#include "is_date_valid.h"
#define IS_DAY_OUT_OF_HIGH_BOUND(DATE)  ((IS_YEAR_LEAP(DATE.y) && DATE.m == FEB)    \
                                         ? DATE.d > MONTH_DAYS[DATE.m - 1] + 1      \
                                         : DATE.d > MONTH_DAYS[DATE.m - 1])

#endif  /* IS_DAY_OUT_OF_HIGH_BOUND_H */
