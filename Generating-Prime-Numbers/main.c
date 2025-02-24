#include <stdlib.h>
#include <stdio.h>

int main () {
    long unsigned int min, max;

    scanf("%lu %lu", &min, &max);

    if (min == 0 || min == 1) {
        min = 2;
    }

    char *arr = malloc((max + 1) * sizeof(char));

    for (long unsigned int i = 0; i <= max; i++) {
        arr[i] = 0;
    }

    for (long unsigned int i = 2; i * i <= max; i++) {
        if (arr[i] == 0) {
            for (long unsigned int j = i * i; j <= max; j += i) {
                arr[j] = 1;
            }
        }
    }

    int first = 1;

    for (long unsigned int i = min; i <= max; i++) {
        if (arr[i] == 0) {
            if (first) {
                printf("%lu", i);
                first = 0;
            } else {
                printf(", %lu", i);
            }
        }
    }

    free(arr);

    return 0;
}