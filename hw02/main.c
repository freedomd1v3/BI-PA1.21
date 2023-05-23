#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>

#define SOUR    2
#define X       0
#define Y       1

/* Thank goodness macros exist! They just made my life WAY easier :) */
#define VZDALENOST_AB(a, b) sqrt(pow(b[X] - a[X], 2) + pow(b[Y] - a[Y], 2))
#define JSOU_STEJNE(a, b)   (a[X] == b[X] && a[Y] == b[Y]) ? true : false

int
main(void) {
    double a[SOUR], b[SOUR], c[SOUR];
    printf("Bod A:\n");
    if (scanf(" %lf %lf", a + X, a + Y) != 2) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    printf("Bod B:\n");
    if (scanf(" %lf %lf", b + X, b + Y) != 2) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    printf("Bod C:\n");
    if (scanf(" %lf %lf", c + X, c + Y) != 2) {
        printf("Nespravny vstup.\n");
        return 1;
    }

    if (JSOU_STEJNE(a, b) || JSOU_STEJNE(a, c) || JSOU_STEJNE(b,c)) {
        printf("Nektere body splyvaji.\n");
        return 0;
    }

    double ab = VZDALENOST_AB(a, b);
    double ac = VZDALENOST_AB(a, c);
    double bc = VZDALENOST_AB(b, c);
    if (fabs(ac - (ab + bc)) < DBL_EPSILON * fabs(ac) ||
        fabs(bc - (ac + ab)) < DBL_EPSILON * fabs(bc) ||
        fabs(ab - (ac + bc)) < DBL_EPSILON * fabs(ab)) {
        printf("Body lezi na jedne primce.\n");

        printf("Prostredni je ");
        if (fabs(bc - (ab + ac)) < DBL_EPSILON * fabs(bc))
            printf("bod A.\n");
        else if (fabs(ac - (ab + bc)) < DBL_EPSILON * fabs(ac))
            printf("bod B.\n");
        else
            printf("bod C.\n");
    }
    else
        printf("Body nelezi na jedne primce.\n");
    return 0;
}
