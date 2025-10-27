#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main() {
    // Reading file name
    char filename[100];
    printf("Enter file name: ");
    scanf("%s", filename);
    FILE *file = fopen(filename, "r");

    // Reading file content
    char program[10000];
    while( fgets(program, sizeof(program), file) != NULL){
        for( char *cursor = program; *cursor != '\0'; cursor++){
            // Skip white space
            while (*cursor != '\n' && *cursor != '\0' && isspace(*cursor))
                cursor++;


            if(*cursor == '/' && *(cursor+1)=='/')
                while(*cursor != '\n' && *cursor != '\0')
                    cursor++;

            // Check keywords
            char keywords[][10] = {"auto", "break", "case", "char", "const", "continue", "default",
                                    "do", "double", "else", "enum", "extern", "float", "for", "goto",
                                    "if", "int", "long", "register", "return", "short", "signed",
                                    "sizeof", "static", "struct", "switch", "typedef", "union",
                                    "unsigned", "void", "volatile", "while"
                                  };
            if(isalpha(*cursor) || *cursor == '_'){
                char word[100];
                int i=0;

                while(isalpha(*cursor) || isdigit(*cursor) || *cursor == '_'){
                    word[i++] = *cursor;
                    cursor++;
                }

                word[i] = '\0';
                int flag = 0;
                for(int i=0; i<sizeof(keywords)/sizeof(keywords[0]); i++){
                    if(strcmp(word, keywords[i]) == 0){
                        printf("%s KEYWORD\n", word);
                        flag = 1;
                        break;
                    }
                }
                if(!flag){
                    printf("%s IDENTIFIER\n", word);
                }
            }

            // Check Number
            if(isdigit(*cursor)){
                char number[100];
                int i=0;
                while(*cursor != '\n' && *cursor != '\0' && isdigit(*cursor)) {
                    number[i++] = *cursor;
                    cursor++;
                }
                number[i] = '\0';
                printf("%s NUMBER\n", number);
            }

            // Check String
            if(*cursor == '"'){
                char string[100];
                int i=0;
                cursor++;
                while(*cursor != '\n' && *cursor != '\0' && *cursor != '"') {
                    string[i++] = *cursor;
                    cursor++;
                }
                string[i] = '\0';
                printf("%s STRING\n", string);
            }

            // Check Character
            if(*cursor == '\''){
                cursor++;
                printf("%c CHARACTER\n", *cursor);
                cursor += 2;
            }

            // Check Special Symbols
            if(strchr("{}();[],#", *cursor)!=NULL){
                printf("%c SEPERATOR\n", *cursor);
            }

            // Check for operators
            if(strchr("+-/%<>!*&|^=", *cursor)!=NULL){
                printf("%c OPERATOR\n", *cursor);
            }


        }      
    }
 

    fclose(file);
}