#include <stdio.h>
#include <stdlib.h>

int main() {
    long unsigned int min, max;

    scanf("%lu %lu", &min, &max);

    if (min == 0 || min == 1)
        min = 2;

    char *arr = malloc((max + 1) * sizeof(char));

    for (long unsigned int i = 0; i <= max; i++)
        arr[i] = 1;

    for (long unsigned int i = 2; i * i <= max; i++) {
        if (arr[i]) {
            for (long unsigned int j = i * i; j <= max; j += i) {
                arr[j] = 0;
            }
        }
    }

    int first = 0;

    for (long unsigned int i = min; i <= max; i++) {
        if (arr[i]) {
            if (first == 0) {
                printf("%lu", i);
                first = 1;
            } else
                printf(", %lu", i);
        }
    }

    free(arr);

    return 0;
}