%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

%token FOR ID NUM RELOP ASSIGN PLUSPLUS MINUSMINUS

%%
stmt:
    FOR '(' assign_stmt ';' cond ';' inc_stmt ')' end_stmt
    {
        printf("Valid FOR loop syntax\n");
        return 0;
    }
;

end_stmt:
    '{' '}'
  | ';'
;

assign_stmt:
  ID ID ASSIGN expr
  | ID ASSIGN expr
;


inc_stmt:
      ID ASSIGN expr
    | ID PLUSPLUS
    | ID MINUSMINUS
;

expr:
      ID
    | NUM
;

cond:
      ID RELOP ID
    | ID RELOP NUM
;
%%

void yyerror(const char *s) {
    printf("Syntax Error: %s\n", s);
}

int main() {
    printf("Enter a FOR loop:\n");
    yyparse();
    return 0;
}
