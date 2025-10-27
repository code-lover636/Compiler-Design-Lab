%{ 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
 
void yyerror(const char *s); 
int yylex(void); 
 
typedef struct node { 
    struct node *left, *right; 
    char *val; 
    int label; 
} NODE;

NODE* makeNode(char *val, NODE* left, NODE* right) { 
    NODE *temp = (NODE*)malloc(sizeof(NODE)); 
    if (!temp) { 
fprintf(stderr, "Memory allocation failed\n"); 
          exit(1); 
    } 
    temp->val = strdup(val); 
    temp->left = left; 
    temp->right = right; 
    temp->label = 0; 
    return temp; 
} 
 
NODE* synTree = NULL; 
%} 
 
%union { 
    char *str; 
    struct node *node; 
} 
 
%token <str> PL MI MUL DIV OP CL EQ ID VAL SC UNR POW 
%type <node> s e t f g 
 
%left PL MI 
%left MUL DIV 
%right POW 
%nonassoc UMINUS 
 
%% 
 
s   : e '\n'        { $$ = $1; synTree = $$; return 0; } 
    | e             { $$ = $1; synTree = $$; return 0; } 
    | ID EQ e       { synTree = makeNode($2, makeNode($1, NULL, NULL), $3); return 0;}
    ; 
 
e   : e PL t        { $$ = makeNode($2, $1, $3); } 
    | e MI t        { $$ = makeNode($2, $1, $3); } 
    | t             { $$ = $1; } 
    ; 
 
t   : t MUL f       { $$ = makeNode($2, $1, $3); } 
    | t DIV f       { $$ = makeNode($2, $1, $3); } 
    | f             { $$ = $1; } 
    ; 
 
f   : g POW f       { $$ = makeNode($2, $1, $3); } 
    | g             { $$ = $1; } 
    ; 
 
g   : OP e CL       { $$ = $2; } 
    | ID            { $$ = makeNode($1, NULL, NULL); } 
    | VAL           { $$ = makeNode($1, NULL, NULL); } 
    | MI g %prec UMINUS { $$ = makeNode($1, NULL, $2); }; 
%% 
 
void inOrder(NODE* root) { 
    if (root) { 
        inOrder(root->left); 
        printf("%s ", root->val); 
        inOrder(root->right); 
    } 
} 
 
void postOrder(NODE* root) { 
    if (root) { 
        postOrder(root->left); 
        postOrder(root->right); 
        printf("%s ", root->val); 
    } 
} 
 
void preOrder(NODE* root) { 
    if (root) { 
        printf("%s ", root->val); 
        preOrder(root->left); 
        preOrder(root->right); 
    } 
} 
 
void freeTree(NODE* root) { 
    if (root) { 
        freeTree(root->left); 
        freeTree(root->right); 
        free(root->val); 
        free(root); 
    } 
} 

void printTree(NODE *root, int depth){
    if(root == NULL){
        return;
    }
    if(depth>=0){
        for(int i=0; i<depth; i++) printf("|  ");
        printf("|- ");
    }
    printf("%s\n", root->val);
    printTree(root->left, depth+1);
    printTree(root->right, depth+1);
}
 
void yyerror(const char *s) { 
    fprintf(stderr, "Error: %s\n", s); 
} 
 
int main(void) { 
    printf("Enter expression: "); 
    if (yyparse() == 0 && synTree) { 
        printf("In Order: "); 
        inOrder(synTree); 
        printf("\nPre Order: "); 
        preOrder(synTree); 
        printf("\nPost Order: "); 
        postOrder(synTree); 
        printf("\n"); 
        printTree(synTree, -1);
        freeTree(synTree);
    } 
    else 
        printf("Parse failed\n"); 
    return 0; 
    } 