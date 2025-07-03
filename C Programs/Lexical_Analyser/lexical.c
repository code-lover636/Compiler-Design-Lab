#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int count = 0;

int isKeyword(char buffer[]) {
    char keywords[32][10] = {"auto", "break", "case", "char", "const", "continue", "default",
                             "do", "double", "else", "enum", "extern", "float", "for", "goto",
                             "if", "int", "long", "register", "return", "short", "signed",
                             "sizeof", "static", "struct", "switch", "typedef", "union",
                             "unsigned", "void", "volatile", "while"
                            };

    for(int i = 0; i<32; i++)
        if(strcmp(keywords[i],buffer)==0)
            return 1;

    return 0;
}

int isOperator(char c) {
    char operator[] = "+-*%/=";
    for(int i=0; i<6; i++) {
        if(c == operator[i]) {
            if(c == '/'){
                count ++;
                return 0;
            }
            return 1;
        }
    }
    return 0;
}

int isSeparator(char c){
    char separator[] = "(){}[];#,";
    for(int i=0; i<9; i++)
        if(c == separator[i])
            return 1;

    return 0;
}

int main() {
    
    char filename[20], buffer[100];
    printf("Enter file name: ");
    scanf("%s", filename);
    FILE *fp = fopen(filename, "r");
    char c;
    int j= 0;

    while((c = fgetc(fp)) != EOF) {
        if(count == 1 && c != '/') {
            count = 0;
            printf("/ is an operator.\n");

        }
        if(count == 2) {
            c = fgetc(fp);
            while(c != '\n' && c != EOF) {
                printf("%c", c);
                c = fgetc(fp);
            }
            j = 0;
            count = 0;
            continue;
        }
        if((c == ' ' || c == '\t' || c == '\n' || c == '/' ||isSeparator(c) || isOperator(c)) && j!=0){
            buffer[j] = '\0';
            if(isKeyword(buffer)){
                printf("%s is a keyword.\n", buffer);
            }
            else{
                printf("%s is an identifier.\n", buffer);
            }
            j = 0;
        }

        if(isOperator(c)){
            printf("%c is an operator.\n", c);
            continue;
        }

        if(isSeparator(c)){
            printf("%c is a separator.\n", c);
            continue;
        }
        if(c != ' ' && c !=  '\n' && c!='\t' && c!='/'){
            buffer[j++] = c;
        }

    }
    fclose(fp);
}

