#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// STRUCTURES
typedef struct {
    char symbol;
    char expression[20];
} ProductionRule;

typedef struct {
    char start;
    int nump;
    char nonterminals[50];
    char terminals[50];
    int first[50][50];
    int follow[50][50];
    ProductionRule pr[20];
} Grammar;

// GLOBAL VARIABLES
Grammar g;
bool changed;
int t, nt;

void read_grammar() {
    char start[2];
    scanf("%s", start);
    g.start = start[0];
    scanf("%d", &(g.nump));
    scanf("%s", g.nonterminals);
    scanf("%s", g.terminals);

    char lhs[2], rhs[20];
    for (int i = 0; i < g.nump; i++) {
        scanf("%s = %s", lhs, rhs);
        ProductionRule rule;
        rule.symbol = lhs[0];
        strcpy(rule.expression, rhs);
        g.pr[i] = rule;
    }
}


int findT(char sym){
    for(int i=0; i<strlen(g.terminals); i++){
        if(g.terminals[i] == sym)
            return i;
    }
    return -1;
}


int findNT(char sym){
    for(int i=0; i<strlen(g.nonterminals); i++){
        if(g.nonterminals[i] == sym)
            return i;
    }
    return -1;
}

void reset(int arr[][50], int value){
    for(int i=0; i<nt; i++)
        for(int j=0; j<=t; j++)
            arr[i][j] = value;
}

void print(int arr[50][50], char ch){
    int m = strlen(g.nonterminals);
    int n = strlen(g.terminals);
    for(int i=0; i<m; i++){
        printf("%c: {", g.nonterminals[i]);
        for(int j=0; j<n; j++){
            if(arr[i][j] != 0){
                printf("%c, ", g.terminals[j]);
            }
        }
        if(arr[i][n] == 1){
            printf("%c", ch);
        }
        printf("}\n");
    }
}

void merge(int arr1[][50], int x, int arr2[][50], int y){
    for(int i=0; i<t; i++){
        if(arr2[y][i] && !arr1[x][i]){
            arr1[x][i] = 1;
            changed = 1;
        }
    }
}

void findFirst() {
    changed = 1;
    int indexnt, index;

    while(changed){
        changed = 0;

        for (int i = 0; i < g.nump; i++) {
            ProductionRule pr = g.pr[i];
            indexnt = findNT(pr.symbol);
            if(pr.expression[0] == 'e'){
                changed |= !g.first[indexnt][t];
                g.first[indexnt][t] = 1;
                continue;
            }

            int k = 0;
            for(k = 0; k<strlen(pr.expression); k++){
                if((index = findT(pr.expression[k])) != -1){
                    changed |= !g.first[indexnt][index];
                    g.first[indexnt][index] = 1;
                    break;
                }

                index = findNT(pr.expression[k]);
                merge(g.first, indexnt, g.first, index);
                if(g.first[index][t] == 0){
                    break;
                }
            }
            if(k == strlen(pr.expression)){
                changed |= !g.first[indexnt][t];
                g.first[indexnt][t] = 1;
            }
        }

    }

}

void findFollow() {
    changed = 1;
    int indexnt, index;
    g.follow[findNT(g.start)][t] = 1;
    while(changed){
        changed = 0;

        for (int i = 0; i < g.nump; i++) {
            ProductionRule pr = g.pr[i];

            if(pr.expression[0] == 'e')
                continue;

            int j;

            for(int k = 0; k<strlen(pr.expression); k++){
                if((indexnt = findNT(pr.expression[k])) == -1)
                    continue;   
                for(j=k+1; j<strlen(pr.expression); j++){
                    if((index = findT(pr.expression[j])) != -1){
                        changed |= !g.follow[indexnt][index];
                        g.follow[indexnt][index] = 1;
                        break;
                    }

                    index = findNT(pr.expression[j]);
                    merge(g.follow, indexnt, g.first, index);
                    if(g.first[index][t] == 0){
                        break;
                    }
                }

                if(j == strlen(pr.expression)){
                    merge(g.follow, indexnt, g.follow, findNT(pr.symbol));
                    if(g.follow[findNT(pr.symbol)][t] == 1){
                        changed |=!g.follow[indexnt][t];
                        g.follow[indexnt][t] = 1;
                    }
                }
            }
        }

    }

}


int main(){
    read_grammar();
    t = strlen(g.terminals);
    nt = strlen(g.nonterminals);
    reset(g.first, 0);
    reset(g.follow, 0);

    findFirst();
    printf("First:\n");
    print(g.first, 'e');

    findFollow();
    printf("\nFollow:\n");
    print(g.follow, '$');
    
}