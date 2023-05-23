#ifndef IS_END_LATER_H
#define IS_END_LATER_H

#define IS_END_LATER(CURRENT, END)  (!(((END.y - CURRENT.y)     \
                                        - ((END.m - CURRENT.m   \
                                        - ((END.d - CURRENT.d   \
                                        - ((END.h - CURRENT.h   \
                                        - ((END.i - CURRENT.i) < 0)) < 0)) < 0)) < 0)) < 0))

#endif  /* IS_END_LATER_H */
