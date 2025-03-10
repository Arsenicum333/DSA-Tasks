#include <stdio.h>
#include <stdlib.h>

#define SCALE 1000000000LL

typedef struct {
    long long intPart;
    long long fracPart;
} Number;

Number parseFraction(char *inp) {
    Number num = {0, 0};
    int i = 0, negative = 0;

    if (inp[i] == '-') {
        negative = 1;
        i++;
    }

    while (inp[i] != ',' && inp[i] != '\0') {
        num.intPart = num.intPart * 10 + (inp[i] - '0');
        i++;
    }

    if (negative)
        num.intPart = -num.intPart;

    if (inp[i] == ',') {
        i++;
        long long fracNum = 0;
        int fracDigits = 0;

        while (inp[i] != '\0' && fracDigits < 9) {
            fracNum = fracNum * 10 + (inp[i] - '0');
            fracDigits++;
            i++;
        }

        while (fracDigits < 9) {
            fracNum *= 10;
            fracDigits++;
        }

        num.fracPart = negative ? -fracNum : fracNum;
    }

    return num;
}

int binarySearch(Number arr[], int size, Number target) {
    int left = 0, right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        long long leftVal = arr[mid].intPart * SCALE + arr[mid].fracPart;
        long long rightVal = target.intPart * SCALE + target.fracPart;

        if (leftVal == rightVal)
            return mid + 1;
        if (leftVal < rightVal)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return 0;
}

int main() {
    unsigned int N, M;
    char input[30];

    scanf("%u", &N);
    Number *arr = malloc(N * sizeof(Number));

    for (int i = 0; i < N; i++) {
        scanf("%s", input);
        arr[i] = parseFraction(input);
    }

    scanf("%u", &M);
    int *res = malloc(M * sizeof(int));

    for (int i = 0; i < M; i++) {
        scanf("%s", input);
        res[i] = binarySearch(arr, N, parseFraction(input));
    }

    for (int i = 0; i < M; i++) {
        if (i == M - 1)
            printf("%d", res[i]);
        else
            printf("%d\n", res[i]);
    }

    free(arr);
    free(res);

    return 0;
}