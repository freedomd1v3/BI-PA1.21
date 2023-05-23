/* Exit codes: 1 - invalid procent value,
 *             2 - invalid day and / or withdrawal,
 *             3 - day of next transaction is earlier or the same
 *                 as the previous one */

#include <stdio.h>
#include <math.h>

#define PRES    2

int
main(void) {
    printf("Zadejte kreditni urok [%%]:\n");
    double kredit;
    if (!scanf(" %lf", &kredit)) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    printf("Zadejte debetni urok [%%]:\n");
    double debet;
    if (!scanf(" %lf", &debet)) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    printf("Zadejte transakce:\n");
    double zustatek = 0;
    int minuly_den = -1;
    for (;;) {
        int den;
        int vklad;
        if (scanf(" %d , %d", &den, &vklad) != 2) {
            printf("Nespravny vstup.\n");
            return 2;
        }
        else if (minuly_den >= den) {
            printf("Nespravny vstup.\n");
            return 3;
        }

        if (minuly_den == -1)
            minuly_den = 0;

        if (zustatek < 0) {
            for (int i = 0; i < den - minuly_den; i++) {
                zustatek *= 1 + debet / pow(10, PRES);
                zustatek = 1.0 * (int)(zustatek * pow(10, PRES)) / pow(10, PRES);
            }
        }
        else if (zustatek > 0) {
            for (int i = 0; i < den - minuly_den; i++) {
                zustatek *= 1 + kredit / pow(10, PRES);
                zustatek = 1.0 * (int)(zustatek * pow(10, PRES)) / pow(10, PRES);
            }
        }

        zustatek += vklad;

        if (!vklad)
            break;

        minuly_den = den;
    }

    printf("Zustatek: %.2f\n", zustatek);
    return 0;
}
