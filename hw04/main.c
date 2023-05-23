#include <stdio.h>

#define SIZE        2000    /* + 1 for EOF to register */
#define ALL_INTS    199900  /* All possible intervals, when i = 2000 */

int
main(void) {
    printf("Posloupnost:\n");
    int sequence[SIZE];
    int count = 0;
    while (!feof(stdin)) {
        if (count == SIZE) {
            printf("Nespravny vstup.\n");
            return 2;
        }

        if (!scanf(" %d", sequence + count++)) {
            printf("Nespravny vstup.\n");
            return 3;
        }
    }
    if (!--count) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    int intervals[ALL_INTS], current_index = 0;
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            for (int k = i; k < j + 1; k++)
                /* All elements in any array, allocated from stack,
                 * are always initialized to 0 */
                intervals[current_index] += sequence[k];
            current_index += 1;
        }
    }

    int pairs = 0, ij = 0;
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            int kl = ij + 1;
            for (int k = i; k < count - 1; k++) {
                if (k == i) {
                    for (int l = j + 1; l < count; l++) {
                        if (intervals[ij] == intervals[kl++])
                                pairs += 1;
                    }
                }
                else {
                    for (int l = k + 1; l < count; l++) {
                        if (intervals[ij] == intervals[kl++])
                                pairs += 1;
                    }
                }
            }
            ij += 1;
        }
    }

    printf("Pocet dvojic: %d\n", pairs);

    return 0;
}
