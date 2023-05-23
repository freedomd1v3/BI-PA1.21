#include <stdio.h>

int
main(void) {
    int n;
    printf("ml' nob:\n");
    if (!scanf(" %d", &n)) {
        printf("luj\n");
        return 1;
    }

    if (!(n >= 1 && n <= 9)) {
        printf("luj\n");
        return 2;
    }

    printf("Qapla'\n");

    const char * citaty[] = {"noH QapmeH wo' Qaw'lu'chugh yay chavbe'lu' 'ej wo' choqmeH may' DoHlu'chugh lujbe'lu'.",
                             "bortaS bIr jablu'DI' reH QaQqu' nay'.",
                             "Qu' buSHa'chugh SuvwI', batlhHa' vangchugh, qoj matlhHa'chugh, pagh ghaH SuvwI''e'.",
                             "bISeH'eghlaH'be'chugh latlh Dara'laH'be'.",
                             "qaStaHvIS wa' ram loS SaD Hugh SIjlaH qetbogh loD.",
                             "Suvlu'taHvIS yapbe' HoS neH.",
                             "Ha'DIbaH DaSop 'e' DaHechbe'chugh yIHoHQo'.",
                             "Heghlu'meH QaQ jajvam.",
                             "leghlaHchu'be'chugh mIn lo'laHbe' taj jej."};
    printf("%s\n", citaty[n - 1]);

    return 0;
}
