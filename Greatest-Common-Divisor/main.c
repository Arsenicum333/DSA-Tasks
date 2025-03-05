#include <stdlib.h>
#include <stdio.h>

typedef struct {
    long unsigned int a;
    long unsigned int b;
    long unsigned int res;
} Numbers;

int main() {
    long unsigned int N;

    scanf("%lu", &N);

    Numbers *arr = malloc(N * sizeof(Numbers));

    for (long unsigned int i = 0; i < N; i++) {
        scanf("%lu %lu", &arr[i].a, &arr[i].b);

        while (arr[i].a && arr[i].b) {
            if (arr[i].a > arr[i].b)
                arr[i].a = arr[i].a % arr[i].b;
            else
                arr[i].b = arr[i].b % arr[i].a;
        }

        if (arr[i].a)
            arr[i].res = arr[i].a;
        else
            arr[i].res = arr[i].b;
    }

    for (long unsigned int i = 0; i < N; i++) {
        if (i == N - 1)
            printf("%lu", arr[i].res);
        else
            printf("%lu\n", arr[i].res);
    }

    free(arr);

    return 0;
}