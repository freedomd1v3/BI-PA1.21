#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Matches */
#define STILL 1
#define CROSS 2
#define EMPTY 0

/* Where */
#define TO_LEFT     1
#define TO_RIGHT    2

/**
 * Check whether or not the matches are solved
 * @param n
 * @param matches
 * @return Whether or not matches are solved
 */
bool
have_won(int n, int * matches) {
    for (int i = 0; i < n; i++)
        if (matches[i] == STILL)
            return false;
    return true;
}

/**
 * Find out if we can move the match to WHERE
 * @param n
 * @param k
 * @param matches
 * @param i         Which match
 * @param WHERE     TO_LEFT or TO_RIGHT
 * @param cross     If can we move the match, where will be cross.
 *                  If we can't move the match, it will be -1
 * @see Where #defines
 * @return Whether or not we can move the watch to WHERE
 */
bool
can_move(int n, int k, int * matches, int i, int const WHERE, int * cross) {
    /* We can't move anything else than STILL :p */
    if (matches[i] != STILL)
        return false;

    if (WHERE == TO_LEFT) {
        for (i--; i > 0 && k > 0; i--) {
           k -= matches[i];
        }
        if (k == 0)
            for (; i >= 0 && matches[i] == EMPTY; i--)
                ;
    }

    else if (WHERE == TO_RIGHT) {
        for (i++; i < n - 1 && k > 0; i++) {
            k -= matches[i];
        }
        if (k == 0)
            for (; i < n && matches[i] == EMPTY; i++)
                ;
    }

    /* Unexpected WHERE value */
    else
        return false;

    if (cross != NULL) {
        if (k == 0 && matches[i] == STILL)
            *cross = i;
        else
            *cross = -1;
    }
    return (k == 0 && matches[i] == STILL);
}

/**
 * Check whether or not matches can't be solved
 * @param n
 * @param k
 * @param matches
 * @return Whether or not matches can't be solved
 */
bool
have_lost(int n, int k, int * matches) {
    for (int i = 0; i < n; i++)
        if (matches[i] == STILL
            && (can_move(n, k, matches, i, TO_LEFT, NULL)
                || can_move(n, k, matches, i, TO_RIGHT, NULL)))
            return false;
    return true;
}

/**
 * Prepare matches and turn_history to the next solve iteration
 * @param matches
 * @param match         Match to move
 * @param cross         Match, where will be cross
 * @param WHERE         TO_LEFT or TO_RIGHT
 * @param turn_history
 * @param th_size       Size of turn_history
 * @param th_i          Current element of turn history
 */
void
prepare_branch(int * matches, int match, int cross, int const WHERE,
               int *** turn_history, int * th_size, int * th_i) {
    matches[match] = EMPTY;
    matches[cross] = CROSS;

    if (*th_i >= *th_size) {
        size_t const TH_ELEM_SIZE = 2;
        *turn_history = (int **) realloc(*turn_history,
                                         (*th_size + 1) * sizeof(*turn_history));
        (*turn_history)[(*th_size)++] = (int *) malloc(TH_ELEM_SIZE
                                                       * sizeof(**turn_history));
    }
    (*turn_history)[*th_i][0] = match;
    if (WHERE == TO_LEFT)
        (*turn_history)[(*th_i)++][1] = 'L';
    else if (WHERE == TO_RIGHT)
        (*turn_history)[(*th_i)++][1] = 'R';
    else
        (*turn_history)[(*th_i)++][1] = '\0';   /* Error handling */
}

/**
 * Clear all mess from matches from the previous solve iterion
 * @param matches
 * @param match     Match that was moved
 * @param cross     Match, where is the cross
 * @param th_i      Index of current turn_history element
 */
void
cleanup_branch(int * matches, int match, int cross,
               int * th_i) {
    matches[match] = STILL;
    matches[cross] = STILL;

    (*th_i)--;
}

/**
 * A recursive function to count amount of matches' solutions.
 * Also prints all possible solutions
 * @param n             Amount of matches
 * @param k             Jump size
 * @param matches       How matches lay now
 * @param turn_history  Turn history
 * @param th_size       Size of turn_history
 * @param th_i          Current element of turn_history
 * @return Amount of all possible solutions
 */
int
solve(int n, int k, int * matches,
      int *** turn_history, int * th_size, int * th_i) {
    if (have_won(n, matches)) {
        for (int i = 0; i < *th_i; i++) {
            if (i)
                putchar(',');
            printf("%d%c", (*turn_history)[i][0], (*turn_history)[i][1]);
        }
        putchar('\n');
        return 1;
    }
    else if (have_lost(n, k, matches))
        return 0;

    int sol = 0;
    for (int i = 0; i < n; i++) {
        if (matches[i] != STILL)
            continue;

        /* At this point we can move every STILL match
         * towards left, right or both, because else we'd have lost */
        int cross;
        if (can_move(n, k, matches, i, TO_LEFT, &cross)) {
            prepare_branch(matches, i, cross, TO_LEFT, turn_history, th_size, th_i);
            sol += solve(n, k, matches, turn_history, th_size, th_i);
            cleanup_branch(matches, i, cross, th_i);
        }
        if (can_move(n, k, matches, i, TO_RIGHT, &cross)) {
            prepare_branch(matches, i, cross, TO_RIGHT, turn_history, th_size, th_i);
            sol += solve(n, k, matches, turn_history, th_size, th_i);
            cleanup_branch(matches, i, cross, th_i);
        }
    }
    return sol;
}

int
main(void) {
    int n, k;
    puts("Pocet sirek:");
    if (!scanf("%d", &n) || !(n > 1)) {
        puts("Nespravny vstup.");
        return 1;
    }

    puts("Velikost skoku:");
    if (!scanf("%d", &k) || k < 0) {
        puts("Nespravny vstup.");
        return 1;
    }

    int sol;
    /* We can't solve odd amount of matches */
    if (n % 2 != 0)
        sol = 0;
    else {
        int * matches = (int *) malloc(n * sizeof(*matches));
        for (int i = 0; i < n; i++)
            matches[i] = STILL;

        int ** turn_history = NULL;
        int th_size = 0;
        int th_i = 0;
        sol = solve(n, k, matches, &turn_history, &th_size, &th_i);
        free(matches);
        for (int i = 0; i < th_size; i++)
            free(turn_history[i]);
        free(turn_history);
    }

    printf("Celkem reseni: %d\n", sol);

    return 0;
}
