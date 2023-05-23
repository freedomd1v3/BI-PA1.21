/* As always, it's INSANE that ProgTest limitations force us
 * to use g++ instead of gcc! C is NOT a subset of C++!!! */

#ifndef __PROGTEST__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#endif  /* __PROGTEST__ */

/* Functions copy_some_text() and realloc_new_text() may be rewritten
 * as macro functions to get rid of some function overhead,
 * but I don't want to lose type security.
 * ... Plus I won't pass bonus tests anyway :p */
#define BASE_INC    10
#define SIZE_MULT   1.5

/** look_for_replacement(): Check, if there is a word from
 *                          char const * (*replace)[2] to replace in
 *                          char const * text
 * @in:     char const * text;
 *          char const * (*replace)[2];
 *          char ** word_to_replace;
 *          size_t const REPLACE_SIZE
 * @out:    char ** word_to_replace - the word from char const * (*replace)[2]
 *                                    to replace
 * @return: Pointer to the earliest occurence of any word from
 *          const char * (*replace)[2] in char const * text.
 *          If there is no such word, NULL is returned */
char const *
look_for_replacement(char const * text, char const * (*replace)[2],
                     char const ** word_to_replace, size_t const REPLACE_SIZE) {
    char const * to_replace = NULL;
    size_t size_to_copy = 0;    /* Initialization here doesn't matter,
                                 * it's done just to bypass g++ warnings */
    for (size_t i = 0; i < REPLACE_SIZE; i++) {
        char const * found = strstr(text, replace[i][0]);

        if (found != NULL
            && ((found - text) * sizeof(*text) < size_to_copy
                || to_replace == NULL)) {
            to_replace = found;
            size_to_copy = (found - text) * sizeof(*text);
            *word_to_replace = replace[i][0];
        }
    }
    return to_replace;
}

/** realloc_new_text(): Realloc new_text to a bigger size
 * @in / @out:  char ** new_text;
 *              size_t * new_text_size */
void
realloc_new_text(char ** new_text, size_t * new_text_size) {
    *new_text_size = BASE_INC + *new_text_size * SIZE_MULT;
    *new_text = (char *) realloc(*new_text,
                                 *new_text_size * sizeof(**new_text));
}

/** copy_some_text(): Copy one symbol from char const ** text
 *                    to char ** new_text
 * @in / @out:  char ** new_text;
 *              char const ** text;
 *              size_t * new_text_i - index of current character in new_text;
 *              size_t * new_text_size */
void
copy_some_text(char ** new_text, char const ** text,
               size_t * new_text_i, size_t * new_text_size) {
    if (*new_text_i >= *new_text_size)
        realloc_new_text(new_text, new_text_size);

    (*new_text)[(*new_text_i)++] = *(*text)++;
}

/** find_replacement_word(): Find the replacement word for the provided word
 *                           in const char * (*replace)[2]
 * @in:     const char * pair_to_find - word to look for in replace
 *          const char * (*replace)[2]
 *          size_t const REPLACE_SIZE - size of replace
 * @return: Pointer to replacement word, if the pair is found;
 *          NULL otherwise */
char const *
find_replacement_word(const char * pair_to_find, const char * (*replace)[2],
                      size_t const REPLACE_SIZE) {
    char const * replacement_word = NULL;
    for (size_t i = 0; i < REPLACE_SIZE; i++)
        if (!strcmp(pair_to_find, replace[i][0])) {
            replacement_word = replace[i][1];
            break;
        }
    return replacement_word;
}

char *
newSpeak(const char * text, const char * (*replace)[2])
{
    if (text == NULL || replace == NULL)
        return NULL;

    size_t replace_size = 0;

    while (replace[replace_size][0] != NULL
           && replace[replace_size][1] != NULL)
        replace_size++;

    /* I guess it's not an error, so I commented it out.
     * Yes, replace is empty, but it's not missing */
    /* if (!replace_size)
        return NULL; */

    /* No text for replacement can be prefix of another text for replacement */
    for (size_t i = 0; i < replace_size; i++)       /* needle */
        for (size_t j = 0; j < replace_size; j++) { /* haystack */
            if (j == i)
                continue;

            else if (!(strstr(*replace[j], *replace[i]) - *replace[j]))
                return NULL;
        }

    size_t const TEXT_SIZE = strlen(text);
    size_t new_text_size = BASE_INC + TEXT_SIZE * SIZE_MULT;
    char * new_text = (char *) malloc(new_text_size * sizeof(*new_text));

    /* All the preparations are done, now the copying procedure itself */
    size_t new_text_i = 0;
    while (*text != '\0') {
        char const * to_replace, * which_word;
        to_replace = look_for_replacement(text, replace,
                                          &which_word, replace_size);

        /* Nothing to replace, just copy all of the remaining text */
        if (to_replace == NULL) {
            while (*text != '\0')
                copy_some_text(&new_text, &text, &new_text_i, &new_text_size);
        }

        else {
            size_t const HOW_MUCH_TO_COPY = (to_replace - text)
                                            * sizeof(*text);
            for (size_t i = 0; i < HOW_MUCH_TO_COPY; i++)
                copy_some_text(&new_text, &text, &new_text_i, &new_text_size);

            char const * replacement_word
                       = find_replacement_word(which_word, replace,
                                               replace_size);
            while (*replacement_word != '\0')
                copy_some_text(&new_text, &replacement_word,
                               &new_text_i, &new_text_size);

            for (size_t i = 0; which_word[i] != '\0'; i++)
                text++;
        }
    }
    new_text[new_text_i] = '\0';

    return new_text;
}

#ifndef __PROGTEST__
int
main(int argc, char * argv[])
{
    char * res;

    const char * d1[][2] = {
        { "murderer", "termination specialist" },
        { "failure", "non-traditional success" },
        { "specialist", "person with certified level of knowledge" },
        { "dumb", "cerebrally challenged" },
        { "teacher", "voluntary knowledge conveyor" },
        { "evil", "nicenest deprived" },
        { "incorrect answer", "alternative answer" },
        { "student", "client" },
        { NULL, NULL }
    };


    const char * d2[][2] = {
        { "fail", "suboptimal result" },
        { "failure", "non-traditional success" },
        { NULL, NULL }
    };


    res = newSpeak("Everybody is happy.", d1);
    assert(!strcmp(res, "Everybody is happy."));
    free(res);

    res = newSpeak("The student answered an incorrect answer.", d1);
    assert(!strcmp(res, "The client answered an alternative answer."));
    free(res);

    res = newSpeak("He was dumb, his failure was expected.", d1);
    assert(!strcmp(res, "He was cerebrally challenged, his non-traditional success was expected."));
    free(res);

    res = newSpeak("The evil teacher became a murderer.", d1);
    assert(!strcmp(res, "The nicenest deprived voluntary knowledge conveyor became a termination specialist."));
    free ( res );

    res = newSpeak("Devil's advocate.", d1);
    assert(!strcmp(res, "Dnicenest deprived's advocate."));
    free(res);

    res = newSpeak("Hello.", d2);
    assert(!res);


    /* Some tests from ProgTest this program was failing before */
    const char * tbl1[][2] = {
        { "son", "offspring with both X and Y chromosomes" },
        { "prisoner", "client of the correctional system" },
        { NULL, NULL }
    };
    res = newSpeak("son of a prisoner", tbl1);
    assert(!strcmp(res, "offspring with both X and Y chromosomes of a client of the correctional system"));
    free(res);

    const char * tbl2[][2] = {
        { "answer", "conventional answer" },
        { "wrong answer", "alternative answer" },
        { NULL, NULL }
    };
    res = newSpeak("correct answer and wrong answer are opposites", tbl2);
    assert(!strcmp(res, "correct conventional answer and alternative answer are opposites"));
    free(res);

    return 0;
}
#endif  /* __PROGTEST__ */
