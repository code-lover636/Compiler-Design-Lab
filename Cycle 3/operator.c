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

typedef struct {
    char symbol;
    char firstT;
    struct Stack * next;
} Stack;

// GLOBAL VARIABLES
Grammar g;
char input[20];
Stack *inpStack = NULL, *outStack = NULL;

char prec[4][4] = {
//   id    +    *    $
    {'>', '>', '>', '>'},
    {'<', '>', '<', '>'},
    {'<', '>', '>', '>'},
    {'<', '<', '<', '>'}
};


void read_grammar(){
    char start[2];
    scanf("%s", start);
    g.start = start[0];
    scanf("%d", &(g.nump));

    char lhs[2], rhs[20];
    for(int i=0; i<g.nump; i++){
        scanf("%s = %s", lhs, rhs);
        ProductionRule * rule = (ProductionRule *) malloc(sizeof(ProductionRule));
        rule->symbol = lhs[0];
        strcpy(rule->expression, rhs);
        rule->next = g.pr;
        g.pr = rule;
    }
}

void push(Stack *stack, char element, int st){
    Stack *node = (Stack *) malloc(sizeof(Stack));
    node->symbol = element;
    node->next = stack;

    if('A' <= element && element <= 'Z')
        node->firstT = (stack != NULL)? stack->firstT : '$';
    else
        node->firstT = element;

    if(st == 0)
        inpStack = node;
    else
        outStack = node;
}

char pop(Stack *stack, int st){
    if(stack == NULL)
        return;

    char element = stack->symbol;
    if(st == 0)
        inpStack = inpStack->next;
    else
        outStack = outStack->next;
    return element;
}

void printStack(Stack *stack){
    while(stack){
        printf("%c", stack->symbol);
        stack = stack->next;
    }
    printf("$");
}

bool shift(){
    if(inpStack == NULL)
        return false;
    
    push(outStack, pop(inpStack, 0), 1);

    printf("ACTION: SHIFT \t OUTPUT STACK: ");
    printStack(outStack);
    printf("\t INPUT STACK: ");
    printStack(inpStack);
    printf("\n");

    return true;
}

bool match(char expression[]){
    Stack *ptr = outStack;
    int i=strlen(expression)-1;
    while(i>=0){
        if(ptr == NULL)
            return false;

        if(ptr->symbol != expression[i])
            return false;

        ptr = ptr->next;
        i--;
    }
    return true;
}

bool reduce(){
    ProductionRule *rule = g.pr;
    while(rule){
        if(match(rule->expression)){
            for(int _=0; _<strlen(rule->expression); _++) pop(outStack, 1);
            push(outStack, rule->symbol, 1);
            
            printf("ACTION: REDUCE(%c -> %s) \t OUTPUT STACK: ", rule->symbol, rule->expression);
            printStack(outStack);
            printf("\t INPUT STACK: ");
            printStack(inpStack);
            printf("\n");

            return true;
        }
        rule = rule->next;
    }
    return false;
}

int findIndex(char t){
    if(t == '$')
        return 3;
    else if(t == '+')
        return 1;
    else if(t == '*')
        return 2;
    else
        return 0;
}


void parse(){
    for(int i=strlen(input)-1; i>=0; i--)
        push(inpStack, input[i], 0);
    
    printf("INPUT STRING: ");
    printStack(inpStack);
    printf("\n");

    for(int x = 0; x<100; x++){
        int first = (outStack == NULL) ? 3: findIndex(outStack->firstT);
        int second = (inpStack == NULL) ? 3: findIndex(inpStack->firstT);
        char op = prec[first][second];

        if(op == '<'){
            if(!shift()){
                break;
            }
            continue;
        }
        else if(op == '>'){
            if(!reduce()){
                break;
            }
            continue;
        }
    }
    
    if(inpStack == NULL && outStack != NULL && outStack->symbol == g.start)
        printf("ACTION: ACCEPT\n");
    else
        printf("ACTION: REJECT\n");
}

int main(){
    read_grammar();
    scanf("%s", input);
    parse();
}