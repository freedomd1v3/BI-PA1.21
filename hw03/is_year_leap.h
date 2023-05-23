#ifndef IS_YEAR_LEAP_H
#define IS_YEAR_LEAP_H

#define IS_YEAR_LEAP(Y) (!(Y % 4) && (Y % 100 || !(Y % 400)) && Y % 4000)

#endif  /* IS_YEAR_LEAP_H */
