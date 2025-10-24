#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Reading Grammar
typedef struct {
    char symbol;
    char expression[20];
} ProductionRule;

typedef struct { 
    char start;
    int nump;
    ProductionRule rules[20];
} Grammar;

typedef struct {
    char symbol;
    struct Stack * next;
} Stack;

// GLOBAL VARIBALES
Grammar g;
char input[20];
Stack *inputStack, *outputStack;

void read_grammar(){
    char start[2];
    scanf("%s", start);
    g.start = start[0];
    scanf("%d", &(g.nump));

    char lhs[2], rhs[20];
    for(int i=0; i<g.nump; i++){
        scanf("%s = %s", lhs, rhs);
        ProductionRule rule;
        rule.symbol = lhs[0];
        strcpy(rule.expression, rhs);
        g.rules[i] = rule;
    }
}

Stack * push(Stack * st, char inp){
        Stack * node = (Stack * ) malloc(sizeof(Stack *));
        node->symbol = inp;
        node->next = st;
        return node;
}

char pop(Stack *st){
    if(st){
        char symbol = st->symbol;
        return symbol;
    }
    return '$';
}

int printStack(Stack *st){
    while(st){
        printf("%c",st->symbol);
        st = st->next;
    }
}

int shift(){
    if(inputStack){
        char symbol = pop(inputStack);
        outputStack = push(outputStack, symbol);
        inputStack = inputStack->next;
        printf("ACTION: SHIFT \t INPUT STACK: ");
        printStack(inputStack);
        printf("$ \t OUTPUT STACK: ");
        printStack(outputStack);
        printf("$\n");
        return 1;
    }
    return 0;
}

int reduce(){
    for(int i=0; i<g.nump; i++){
        ProductionRule rule = g.rules[i];

        // FIND MATCH
        int len = strlen(rule.expression);
        int flag = 1;
        Stack *ptr = outputStack;

        for(int j=len-1; j>=0; j--){
            if(ptr == NULL){
                flag = 0;
                break;
            }
            if(rule.expression[j] != ptr->symbol){
                flag = 0;
                break;
            }
            ptr = ptr->next;
        }
        
        if(!flag){
            continue;
        }
        printf("ACTION: REDUCE(%c -> % s) \t ", rule.symbol, rule.expression);
        for(int j=0; j<len; j++){
            pop(outputStack);
            outputStack = outputStack->next;
        }

        outputStack = push(outputStack, rule.symbol);

        printf("INPUT STACK: ");
        printStack(inputStack);
        printf("$ \t OUTPUT STACK: ");
        printStack(outputStack);
        printf("$\n");

        return 1;

    }
    return 0;
}

void parse(){
    inputStack = NULL;
    outputStack = NULL;

    for(int i = strlen(input)-1; i>=0; i--){
        inputStack = push(inputStack, input[i]);
    }

    for(int i=0; i<100; i++){
        if(reduce()){
            continue;
        }

        if(shift()){
            continue;
        }

        if(inputStack == NULL && outputStack != NULL && outputStack->symbol == g.start){
            printf("ACTION: ACCEPTED\n");
            break;
        }
        else{
            printf("ACTION: REJECTED\n");
            break;
        }
    }
}

int main(){
    read_grammar();
    scanf("%s", input);
    printf("INPUT STRING: %s \n", input);
    parse();

}