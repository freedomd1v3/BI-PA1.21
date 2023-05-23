#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

/* Exit codes */
#define ALLOC_FAIL      1
#define BAD_INPUT       2

#define DBL_EPS_MULT    250 /* This should be specified very carefully */

#define NAME_SIZE       200 /* 199 + '\0' */

struct Plane {
    double x, y;
    char name[NAME_SIZE];
};

/* This function returns:
 *  1, if all elements are successfully read,
 *  0, if got bad input
 *  or 2, if got EOF */
int
read_plane(struct Plane * plane) {
    char colon;
    if (scanf(" %lf , %lf %c ", &(plane->x), &(plane->y), &colon) != 3
        || colon != ':') {
        if (feof(stdin))
            return 2;
        return 0;
    }

    /* We don't check the input completely. That is REALLY bad */
    if(!fgets(plane->name, NAME_SIZE, stdin))
        return 0;

    plane->name[strcspn(plane->name, "\n")] = '\0';
    return 1;
}

int
main(void) {
    /* Dynamic arrays will be extended in geometric sequence */
    size_t planes_size = 50;
    struct Plane * planes = (struct Plane *) malloc(planes_size
                                                    * sizeof(*planes));
    if (planes == NULL) {
        printf("malloc() fail.\n");
        return ALLOC_FAIL;
    }

    printf("Pozice letadel:\n");
    size_t planes_count = 0;
    for (;;) {
        int result;
        if (!(result = read_plane(planes + planes_count++))) {
            printf("Nespravny vstup.\n");
            return BAD_INPUT;
        }
        else if (result == 2)
            break;

        if (planes_count == planes_size) {
            planes = (struct Plane *) realloc(planes, planes_size * 2
                                                      * sizeof(*planes));
            if (planes == NULL) {
                printf("realloc() fail.\n");
                return ALLOC_FAIL;
            }
            planes_size *= 2;
        }
    }
    planes_count -= 1;  /* It will always be at least 1, so no overflow */
    size_t const MIN = 2;
    if (planes_count < MIN) {
        printf("Nespravny vstup.\n");
        return BAD_INPUT;
    }
    
    /* Set the minimum distance to the first 2 planes */
    double nearest = sqrt(pow(planes[1].x - planes[0].x, 2)
                          + pow(planes[1].y - planes[0].y, 2));

    for (size_t i = 0; i + 1 < planes_count; i++) { /* No overflow!! */
        for (size_t j = i + 1; j < planes_count; j++) {
            double distance = sqrt(pow(planes[j].x - planes[i].x, 2)
                                   + pow(planes[j].y - planes[i].y, 2));
            if (distance < nearest)
                nearest = distance;
        }
    }
    printf("Vzdalenost nejblizsich letadel: %f\n", nearest);

    size_t count = 0, pairs_size = 50;
    size_t ** pairs = (size_t **) malloc(pairs_size * sizeof(*pairs));
    if (pairs == NULL) {
        printf("malloc() fail.\n");
        return ALLOC_FAIL;
    }

    size_t const PAIR = 2;
    for (size_t i = 0; i < pairs_size; i++) {
        pairs[i] = (size_t *) malloc(PAIR * sizeof(**pairs));
        if (pairs[i] == NULL) {
            printf("malloc() fail.\n");
            return ALLOC_FAIL;
        }
    }
    for (size_t i = 0; i + 1 < planes_count; i++) {
        for (size_t j = i + 1; j < planes_count; j++) {
            double distance = sqrt(pow(planes[j].x - planes[i].x, 2)
                                   + pow(planes[j].y - planes[i].y, 2));

            if (distance == nearest
                || fabs(nearest - distance) < DBL_EPS_MULT * DBL_EPSILON * fabs(nearest)) {
                pairs[count][0] = i;
                pairs[count][1] = j;
                count += 1;

                if (count == pairs_size) {
                    pairs = (size_t **) realloc(pairs, pairs_size * 2 * sizeof(*pairs));
                    if (pairs == NULL) {
                        printf("realloc() fail.\n");
                        return ALLOC_FAIL;
                    }

                    for (size_t i = pairs_size; i < pairs_size * 2; i++) {
                        pairs[i] = (size_t *) malloc(PAIR * sizeof(**pairs));
                        if (pairs[i] == NULL) {
                            printf("malloc() fail.\n");
                            return ALLOC_FAIL;
                        }
                    }
                    pairs_size *= 2;
                }
            }
        }
    }
    printf("Nalezenych dvojic: %zu\n", count);
    for (size_t i = 0; i < count; i++)
        printf("%s - %s\n", planes[pairs[i][0]].name, planes[pairs[i][1]].name);

    /* Let the system free memory */
    /* free(planes);
    for(size_t i = 0; i < pairs_size; i++)
        free(pairs[i]);
    free(pairs); */
    return 0;
}
