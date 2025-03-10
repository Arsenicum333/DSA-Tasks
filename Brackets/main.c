#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 100000

char matching(char ch) {
    if (ch == ')') return '(';
    if (ch == ']') return '[';
    if (ch == '}') return '{';
    if (ch == '>') return '<';

    return 0;
}

int countErrors() {
    char *stack = malloc(MAX_LENGTH * sizeof(char));
    int top = -1, errors = 0;
    char ch;

    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (ch == '(' || ch == '[' || ch == '{' || ch == '<') {
            if (top < MAX_LENGTH - 1)
                stack[++top] = ch;
            else
                errors++;
        } else {
            if (top == -1)
                errors++;
            else if (stack[top] != matching(ch)) {
                errors++;
                top--;
            } else
                top--;
        }
    }

    free(stack);

    return errors + (top + 1);
}

int main() {
    int N;

    scanf("%d", &N);
    int *res = malloc(N * sizeof(int));
    getchar();

    for (int i = 0; i < N; i++)
        res[i] = countErrors();

    for (int i = 0; i < N; i++) {
        if (i == N - 1)
            printf("%d", res[i]);
        else
            printf("%d\n", res[i]);
    }

    free(res);

    return 0;
}