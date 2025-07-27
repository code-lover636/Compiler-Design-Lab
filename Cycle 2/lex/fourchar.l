%{
    #include <stdio.h>
%}

%%
exit { printf("Exiting the program.\n"); exit(0); }
.*[Aa][Rr][Aa][Vv].* { printf("Rejected: %s\n", yytext); }
.* { printf("Accepted: %s\n", yytext); }
%%

int main() {
    yylex();
}