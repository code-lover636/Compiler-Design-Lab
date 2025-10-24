%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}



%token NUMBER


%left '+' '-'
%left '*' '/'
%right UMINUS  

%%

input:
      /* empty */
    | input line
    ;

line:
      expr '\n'   { printf("Result = %d\n", $1); }
    | '\n'        { /* ignore blank lines */ }
    ;

expr:
      expr '+' expr   { $$ = $1 + $3; }
    | expr '-' expr   { $$ = $1 - $3; }
    | expr '*' expr   { $$ = $1 * $3; }
    | expr '/' expr   {
                        if ($3 == 0) {
                            yyerror("Division by zero");
                            $$ = 0;   /* recover: set result to 0 */
                        } else {
                            $$ = $1 / $3;
                        }
                      }
    | '-' expr %prec UMINUS   { $$ = -$2; }   /* unary minus */
    | '(' expr ')'            { $$ = $2; }
    | NUMBER                  { $$ = $1; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(void) {
    printf("Enter expressions, one per line (Ctrl+D to exit):\n");
    return yyparse();
}
