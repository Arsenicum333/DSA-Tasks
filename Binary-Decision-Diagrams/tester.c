#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bdd.c"

char evaluateExpression(const char* expression, const char* inputs, const char* varOrder) {
    int index = 0;
    char orResult = '0';

    while (expression[index]) {
        char andResult = '1';

        while (expression[index] && expression[index] != '+') {
            char varValue;
            int negate = 0;

            if (expression[index] == '!') {
                negate = 1;
                index++;
            }

            if (expression[index] < 'A' || expression[index] > 'Z') break;
            char variable = expression[index++];
            int varIndex = -1;

            for (int i = 0; varOrder[i]; i++) {
                if (varOrder[i] == variable) {
                    varIndex = i;
                    break;
                }
            }

            if (varIndex == -1) {
                fprintf(stderr, "Variable %c not in varOrder\n", variable);
                return '0';
            }

            varValue = inputs[varIndex];

            if (negate)
                varValue = (varValue == '1') ? '0' : '1';

            if (varValue == '0')
                andResult = '0';
        }

        if (andResult == '1')
            orResult = '1';

        if (expression[index] == '+')
            index++;
    }

    return orResult;
}

char* generateRandomDNF(int numVariables, const char* varOrder) {
    char* expression = malloc(256);
    expression[0] = '\0';
    int numTerms = rand() % 4 + 1; // Generate random number of terms (1-4)

    for (int i = 0; i < numTerms; i++) {
        char term[32] = "";
        int numLiterals = rand() % (numVariables / 2 + 1) + 1; // Generate random number of literals per term

        for (int j = 0; j < numLiterals; j++) {
            if (rand() % 2) strcat(term, "!"); // Randomly add negation
            char variable = varOrder[rand() % numVariables];
            char varStr[2] = {variable, '\0'};
            strcat(term, varStr);
        }

        strcat(expression, term);

        if (i < numTerms - 1)
            strcat(expression, "+");
    }

    // Ensure expression is not empty
    if (strlen(expression) == 0) {
        char varStr[2] = {varOrder[0], '\0'};
        strcat(expression, varStr);
    }

    return expression;
}

void testRandomInputs(int n, const char* varOrder, const char* expression, int* errors) {
    char inputs[32] = {0};
    BDD* bdd = createBDD(expression, varOrder);

    if (!bdd) {
        fprintf(stderr, "Failed to create BDD for expression: %s\n", expression);
        (*errors)++;
        return;
    }

    for (unsigned int i = 0; i < (1u << n); i++) {
        for (int j = 0; j < n; j++)
            inputs[j] = (i & (1 << j)) ? '1' : '0';

        inputs[n] = '\0';
        char bddResult = useBDD(bdd, inputs);
        char exprResult = evaluateExpression(expression, inputs, varOrder);

        if (bddResult != exprResult) {
            fprintf(stderr, "Error: expression=%s, inputs=%s, bddResult=%c, exprResult=%c\n", expression, inputs, bddResult, exprResult);
            (*errors)++;
        }
    }

    freeBDD(bdd);
}

int fullBDDSize(int numVariables) {return (1 << numVariables) - 1;}

size_t estimateBDDMemory(BDD* bdd) {
    if (!bdd) return 0;
    size_t nodesMemory = bdd->numNodes * sizeof(Node);
    size_t hashtableMemory = 0;
    if (bdd->nodeCache) {
        hashtableMemory += bdd->nodeCache->size * sizeof(DataBDD*);
        hashtableMemory += bdd->nodeCache->count * sizeof(DataBDD);
    }
    size_t bddMemory = sizeof(BDD);
    size_t varOrderMemory = (bdd->numVariables + 1) * sizeof(char);
    return nodesMemory + hashtableMemory + bddMemory + varOrderMemory;
}

void printResults(int numVariables, double avgReduction, double avgExtraReduction, double avgTimeCreate, double avgTimeBestOrder, int totalErrors, double avgMemory) {
    printf("\n\n+-------------------------------------+\n");
    printf("|   BDD Test Results (%2d Variables)   |\n", numVariables);
    printf("+-------------------------------------+\n");
    printf("| Avg Reduction         |    %6.2f %% |\n", avgReduction);
    printf("| Avg Extra Reduction   |    %6.2f %% |\n", avgExtraReduction);
    printf("| Avg Time (Create)     | %8.2f ms |\n", avgTimeCreate * 1000.0);
    printf("| Avg Time (Best Order) | %8.2f ms |\n", avgTimeBestOrder * 1000.0);
    printf("| Avg Memory Usage      | %8.2f KB |\n", avgMemory / 1024.0);
    printf("+-------------------------------------+\n");
    printf("| Total Errors          |  %10d |\n", totalErrors);
    printf("+-------------------------------------+\n\n");
}

int main() {
    srand(time(NULL));
    int maxVariables = 13;
    int testsPerVars = 100;

    for (int numVariables = 4; numVariables <= maxVariables; numVariables++) {
        double totalReduction = 0.0;
        double totalExtraReduction = 0.0;
        double totalTimeCreate = 0.0;
        double totalTimeBestOrder = 0.0;
        size_t totalMemory = 0;
        int totalErrors = 0;

        printf("Testing for %d variables...\n", numVariables);

        for (int test = 0; test < testsPerVars; test++) {
            printf("Test %d/%d\r", test + 1, testsPerVars);
            fflush(stdout);

            char varOrder[32];

            for (int i = 0; i < numVariables; i++)
                varOrder[i] = 'A' + i;

            varOrder[numVariables] = '\0';
            char* expression = generateRandomDNF(numVariables, varOrder);

            clock_t start = clock();
            BDD* bdd = createBDD(expression, varOrder);
            clock_t end = clock();

            if (!bdd) {
                fprintf(stderr, "Failed to create BDD for expression: %s\n", expression);
                free(expression);
                continue;
            }

            totalTimeCreate += (double)(end - start) / CLOCKS_PER_SEC;
            int errors = 0;
            testRandomInputs(numVariables, varOrder, expression, &errors);
            totalErrors += errors;
            int fullSize = fullBDDSize(numVariables);
            double reduction = 100.0 * (fullSize - bdd->numNodes) / fullSize;
            if (reduction < 0) reduction = 0;
            totalReduction += reduction;

            start = clock();
            BDD* bestBDD = createBDDWithBestOrder(expression);
            end = clock();

            if (!bestBDD) {
                fprintf(stderr, "Failed to create best BDD for expression: %s\n", expression);
                freeBDD(bdd);
                free(expression);
                continue;
            }

            totalTimeBestOrder += (double)(end - start) / CLOCKS_PER_SEC;
            double extraReduction = 100.0 * (bdd->numNodes - bestBDD->numNodes) / bdd->numNodes;
            if (extraReduction < 0) extraReduction = 0;
            totalExtraReduction += extraReduction;
            totalMemory += estimateBDDMemory(bdd) + estimateBDDMemory(bestBDD);

            freeBDD(bdd);
            freeBDD(bestBDD);
            free(expression);
        }

        double avgReduction = totalReduction / testsPerVars;
        double avgExtraReduction = totalExtraReduction / testsPerVars;
        double avgTimeCreate = totalTimeCreate / testsPerVars;
        double avgTimeBestOrder = totalTimeBestOrder / testsPerVars;
        double avgMemory = (double)totalMemory / testsPerVars;

        printResults(numVariables, avgReduction, avgExtraReduction, avgTimeCreate, avgTimeBestOrder, totalErrors, avgMemory);
    }

    return 0;
}
