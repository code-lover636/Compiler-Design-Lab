#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// STRUCTURES
typedef struct {
    char symbol;
    char expression[20];
    struct ProductionRule *next;
} ProductionRule;

typedef struct {
    char start;
    int nump;
    ProductionRule* pr;
} Grammar;

// GLOBAL VARIABLES
Grammar g;
char input[20];

void read_grammar() {
    char start[2];
    scanf("%s", start);
    g.start = start[0];
    scanf("%d", &(g.nump));

    g.pr = NULL;
    char lhs[2], rhs[20];
    for (int i = 0; i < g.nump; i++) {
        scanf("%s = %s", lhs, rhs);
        ProductionRule *rule = (ProductionRule *) malloc(sizeof(ProductionRule));
        rule->symbol = lhs[0];
        strcpy(rule->expression, rhs);
        rule->next = g.pr;
        g.pr = rule;
    }
}

void print(char expansion[50], int expIndex){
    printf("%.*s[%c]%s\t", expIndex, expansion, expansion[expIndex], expansion+expIndex+1);
}

bool parse(char expansion[50], int expIndex, char input[20], int inpIndex) {
    if (strlen(expansion) == expIndex) {
        return strcmp(expansion, input) == 0;
    }

    if (expansion[expIndex] == input[inpIndex]) {
        print(expansion, expIndex);
        printf("[%c matches with input]\n", expansion[expIndex]);
        return parse(expansion, expIndex + 1, input, inpIndex + 1);
    }

    if ('A' > expansion[expIndex] || expansion[expIndex] > 'Z') {
        print(expansion, expIndex);
        printf("[%c mismatches with input]\n", expansion[expIndex]);
        return false;
    }

    char nt = expansion[expIndex];
    char temp[50];
    strcpy(temp, expansion);
    expansion[expIndex] = '\0';
    ProductionRule *rule = g.pr;

    while (rule) {
        if (rule->symbol == nt) {
            print(temp, expIndex);
            printf("[Expand: %c -> %s]\n", rule->symbol, rule->expression);

            strcat(expansion, rule->expression);
            strcat(expansion, temp + expIndex + 1);

            bool success = parse(expansion, expIndex, input, inpIndex);
            if (success) {
                return true;
            }

            printf("\n======= Backtrack from %c -> %s ======\n\n", rule->symbol, rule->expression);
            strcpy(expansion, temp);
            expansion[expIndex] = '\0';
        }
        rule = rule->next;
    }

    return false;
}

int main() {
    read_grammar();
    scanf("%s", input);

    char expansion[50];
    expansion[0] = g.start;
    expansion[1] = '\0';

    printf("Input: %s\n", input);
    bool accepted = parse(expansion, 0, input, 0);

    printf("\nResult: %s\n", accepted ? "String ACCEPTED by grammar" : "String REJECTED by grammar");
    return 0;
}
