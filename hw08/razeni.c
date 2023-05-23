#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct TItem
{
    struct TItem    * m_Next;
    char            * m_Name;
    char            m_Secret[24];
} TITEM;

int
cmpName(const TITEM * a,
        const TITEM * b) {
    return strcmp(a->m_Name, b->m_Name);
}

int
cmpNameInsensitive(const TITEM * a,
                   const TITEM * b) {
    return strcasecmp(a->m_Name, b->m_Name);
}

int
cmpNameLen(const TITEM * a,
           const TITEM * b) {
    size_t la = strlen(a->m_Name), lb = strlen(b->m_Name);
    return (lb < la) - (la < lb);
}
#endif  /* __PROGTEST__ */

TITEM *
newItem(const char * name, TITEM * next ) {
    TITEM * new_item = (TITEM *) malloc(sizeof(*new_item));

    size_t const NAME_SIZE = strlen(name);
    new_item->m_Name = (char *) malloc((NAME_SIZE + 1) * sizeof(new_item->m_Name));

    strcpy(new_item->m_Name, name);
    new_item->m_Name[NAME_SIZE] = '\0';

    for (size_t i = 0; i < 24; i++)
        new_item->m_Secret[i] = '\0';

    new_item->m_Next = next;

    return new_item;
}

/* Thanks a lot to (c) GeeksforGeeks!
 * Basic idea of "naive algorithm" is taken from there */
TITEM *
sortListCmp(TITEM * l, int ascending,
            int (* cmpFn)(const TITEM *, const TITEM *)) {
    /* Empty list */
    if (l == NULL)
        return NULL;

    int swp;
    do {
        swp = 0;

        TITEM * cur = l, * cur_parent = NULL;

        while (cur->m_Next != NULL) {
            int compar = cmpFn(cur, cur->m_Next);
            if (ascending == 0)
                compar = -compar;

            if (compar > 0) {
                if (cur_parent == NULL) {
                    TITEM * swap1 = cur, * swap2 = cur->m_Next->m_Next;
                    cur = cur->m_Next;
                    cur->m_Next = swap1;
                    cur->m_Next->m_Next = swap2;

                    l = cur;
                }
                else {
                    TITEM * swap = cur->m_Next->m_Next;

                    cur_parent->m_Next = cur->m_Next;
                    cur_parent->m_Next->m_Next = cur;
                    cur->m_Next = swap;
                }

                swp = 1;
                break;
            }

            cur_parent = cur;
            cur = cur->m_Next;
        }
    } while(swp);

    return l;
}

void
freeList(TITEM * src)
{
    while (src != NULL) {
        free(src->m_Name);
        TITEM * p = src;
        src = src->m_Next;
        free(p);
    }
}

#ifndef __PROGTEST__
int
main(void) {
    TITEM * l;
    char tmp[50];

    assert(sizeof(TITEM) == sizeof(TITEM *) + sizeof(char *) + 24 * sizeof(char));

    l = NULL;
    l = newItem("BI-PA1", l);
    l = newItem("BIE-PA2", l);
    l = newItem("NI-PAR", l);
    l = newItem("lin", l);
    l = newItem("AG1", l);
    assert(l != NULL
           && strcmp(l->m_Name, "AG1") == 0);
    assert(l->m_Next != NULL
           && strcmp(l->m_Next-> m_Name, "lin") == 0);
    assert(l->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Name, "NI-PAR") == 0);
    assert(l->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Name, "BIE-PA2") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Name, "BI-PA1") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);

    l = sortListCmp(l, 1, cmpName);
    assert(l != NULL
           && strcmp(l->m_Name, "AG1") == 0);
    assert(l->m_Next != NULL
           && strcmp(l->m_Next->m_Name, "BI-PA1") == 0);
    assert(l->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Name, "BIE-PA2") == 0);
    assert(l->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Name, "NI-PAR") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Name, "lin") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);

    l = sortListCmp(l, 1, cmpNameInsensitive);
    assert(l != NULL
           && strcmp(l->m_Name, "AG1") == 0);
    assert(l->m_Next != NULL
           && strcmp(l->m_Next->m_Name, "BI-PA1") == 0);
    assert(l->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Name, "BIE-PA2") == 0);
    assert(l->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Name, "lin") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Name, "NI-PAR") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);

    l = sortListCmp(l, 1, cmpNameLen);
    assert(l != NULL
           && strcmp(l->m_Name, "AG1") == 0);
    assert(l->m_Next != NULL
           && strcmp(l->m_Next->m_Name, "lin") == 0);
    assert(l->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Name, "BI-PA1") == 0);
    assert(l->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Name, "NI-PAR") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Name, "BIE-PA2") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);

    l = newItem("AAG.3", l);
    assert(l != NULL
           && strcmp(l->m_Name, "AAG.3") == 0);
    assert(l->m_Next != NULL
           && strcmp(l->m_Next->m_Name, "AG1") == 0);
    assert(l->m_Next->m_Next != NULL
           && strcmp(l->m_Next -> m_Next -> m_Name, "lin") == 0);
    assert(l->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Name, "BI-PA1") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Name, "NI-PAR") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Name, "BIE-PA2") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next == NULL );

    l = sortListCmp(l, 0, cmpNameLen);
    assert(l != NULL
           && strcmp(l->m_Name, "BIE-PA2") == 0);
    assert(l->m_Next != NULL
           && strcmp(l->m_Next->m_Name, "BI-PA1") == 0);
    assert(l->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Name, "NI-PAR") == 0);
    assert(l->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Name, "AAG.3") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Name, "AG1") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Name, "lin") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);

    freeList(l);
    l = NULL;
    strncpy(tmp, "BI-PA1", sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';
    l = newItem(tmp, l);

    strncpy(tmp,"BIE-PA2", sizeof(tmp)- 1);
    tmp[sizeof(tmp) - 1] = '\0';
    l = newItem(tmp,l);

    strncpy(tmp, "NI-PAR", sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';
    l = newItem(tmp, l);

    strncpy(tmp, "lin", sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';
    l = newItem(tmp, l);

    strncpy(tmp, "AG1", sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';
    l = newItem(tmp, l);
    assert(l != NULL
           && strcmp(l->m_Name, "AG1") == 0);
    assert(l->m_Next != NULL
           && strcmp(l->m_Next->m_Name, "lin") == 0);
    assert(l->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Name, "NI-PAR") == 0);
    assert(l->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Name, "BIE-PA2") == 0);
    assert(l->m_Next->m_Next->m_Next-> m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Name, "BI-PA1") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);

    l = sortListCmp(l, 1, cmpName);
    assert(l != NULL
           && strcmp(l->m_Name, "AG1") == 0);
    assert(l->m_Next != NULL
           && strcmp(l->m_Next->m_Name, "BI-PA1") == 0);
    assert(l->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Name, "BIE-PA2") == 0);
    assert(l->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Name, "NI-PAR") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Name, "lin") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);

    l = sortListCmp(l, 1, cmpNameInsensitive);
    assert(l != NULL
           && strcmp(l->m_Name, "AG1") == 0);
    assert(l->m_Next != NULL
           && strcmp(l->m_Next->m_Name, "BI-PA1") == 0);
    assert(l->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Name, "BIE-PA2") == 0);
    assert(l->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Name, "lin") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Name, "NI-PAR") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);

    l = sortListCmp(l, 1, cmpNameLen);
    assert(l != NULL
           && strcmp(l -> m_Name, "AG1") == 0);
    assert(l->m_Next != NULL
           && strcmp(l->m_Next->m_Name, "lin") == 0);
    assert(l->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Name, "BI-PA1") == 0);
    assert(l->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Name, "NI-PAR") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Name, "BIE-PA2") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);

    strncpy(tmp, "AAG.3", sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';
    l = newItem(tmp, l);
    assert(l != NULL
           && strcmp(l->m_Name, "AAG.3") == 0);
    assert(l->m_Next != NULL
           && strcmp(l->m_Next->m_Name, "AG1") == 0);
    assert(l->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Name, "lin") == 0);
    assert(l->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Name, "BI-PA1") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Name, "NI-PAR") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Name, "BIE-PA2") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);

    l = sortListCmp(l, 0, cmpNameLen);
    assert(l != NULL
           && strcmp(l->m_Name, "BIE-PA2") == 0);
    assert(l -> m_Next != NULL
           && strcmp(l->m_Next->m_Name, "BI-PA1") == 0);
    assert(l -> m_Next -> m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Name, "NI-PAR") == 0);
    assert(l -> m_Next -> m_Next -> m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Name, "AAG.3") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Name, "AG1") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next->m_Next != NULL
           && strcmp(l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Name, "lin") == 0);
    assert(l->m_Next->m_Next->m_Next->m_Next->m_Next->m_Next == NULL);

    freeList(l);
    return 0;
}
#endif  /* __PROGTEST__ */
