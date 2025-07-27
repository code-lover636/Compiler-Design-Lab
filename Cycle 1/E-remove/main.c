#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int n, a, f;

struct node {
    int state;
    struct node *next;
};

int printTransition(struct node* transition[n][a]){
    printf("\nTransition table:\n");
    for(int i=0; i<n; i++){
        for(int j=0; j<a; j++){
            struct node * head = transition[i][j];
            printf("{");
            while(head != NULL){
                printf("q%d, ", head->state);
                head = head->next;
            }
            printf("}\t");
        }
        printf("\n");
    }
}

int findalpha(char inp, int a, char alphabet[a]){
    for(int i=0; i<a; i++){
        if(alphabet[i] == inp){
            return i;
        }
    }
    return -1;
}

int reset(struct node* transition[n][a]){
    for(int i=0; i<n; i++){
        for(int j=0; j<a; j++){
            transition[i][j] = NULL;
        }
    }
}

void findClosure(int start, int state, int set[n], struct node * transition[n][a], int e_closure[n][n]){
    if(set[state] == 1)
        return;
    set[state] = 1;
    e_closure[start][state] = 1;
    struct node * temp = transition[state][a-1];
    while(temp != NULL){
        findClosure(start, temp->state, set, transition, e_closure);
        temp = temp->next;
    }
}

int print_e_closure(int e_closure[n][n], int i){
    printf("{");
    for(int j=0; j<n; j++){
        if(e_closure[i][j] == 1){
            printf("q%d, ", j);
        }
    }
    printf("} ");
}

int main(){
    printf("Enter no of states: ");
    scanf("%d", &n);
    printf("Enter no of input symbols: ");
    scanf("%d", &a);

    struct node  *transition[n][a];
    char alphabet[a];
    int e_closure[n][n], set[n];
    reset(transition);

    printf("Enter input symbols without space (if epsilon is present, it should be the last symbol): ");
    scanf("%s", alphabet);

    printf("Enter total no. of final states: ");
    scanf("%d", &f);
    int final[f];

    printf("Enter final states:\n");
    for(int i=0; i<f; i++){
        scanf("%d", &final[i]);
    }

    char state1[3], state2[3], inp[2];
    int from, to, index, end;

    //Build Transition Matrix
    FILE *INPUT = fopen("NFA.txt", "r");
    while((end = fscanf(INPUT, "%s %s %s", state1, inp, state2)) != EOF){
        from = state1[1] - '0';
        to = state2[1] - '0';
        index = findalpha(inp[0], a, alphabet);

        struct node * temp = (struct node *)malloc(sizeof(struct node));
        temp->state = to;
        temp->next = transition[from][index];
        transition[from][index] = temp;
    }

    printTransition(transition);

    //Find Epsilon Closure
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            set[j] = 0;
            e_closure[i][j] = 0;
        }
        findClosure(i, i, set, transition, e_closure);
    }

    // Print Epsilon Closure
    printf("\nStates: ");
    for(int i = 0; i < n; i++)
        print_e_closure(e_closure, i);
    printf("\n");

    // Print Epsilon Closure with input symbols
    printf("\nNFA  without epsilon transitions:\n");
    for(int i=0; i<n; i++){
        for(int j=0; j<a-1; j++){
            for(int x = 0; x < n; x++) set[x] = 0;

            for(int k=0; k<n; k++){
                if(e_closure[i][k] == 1){
                    struct node * temp = transition[k][j];
                    while(temp != NULL){
                        set[temp->state] = 1;
                        temp = temp->next;
                    }
                }
            }

            print_e_closure(e_closure, i);
            printf("\t%c\t", alphabet[j]);
            printf("{");
            for(int k=0; k<n; k++)
                if(set[k] == 1)
                    printf("q%d, ", k);
            printf("}\n");
        }
    }

    //Final states
    printf("\nFinal states:\n");
    for(int i=0; i<f; i++){
        printf("{");
        for(int j=0; j<n; j++){
            if(e_closure[final[i]][j] == 1){
                printf("q%d ", j);
            }
        }
        printf("}\n");
    }
}