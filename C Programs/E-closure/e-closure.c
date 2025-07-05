#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int no_of_states;

void reset(int closure[no_of_states][no_of_states]){
    for(int i = 0; i<no_of_states; i++)
        for(int j=0; j<no_of_states; j++){
            if(i == j)
                closure[i][j] = 1;
            else
                closure[i][j] = 0;
        }
}

void display(int closure[no_of_states][no_of_states]){
    printf("Epsilon closure of all states:\n");

    for(int i=0; i<no_of_states; i++){
        printf("q%d: {",i);
        for(int j=0; j<no_of_states; j++){
            if(closure[i][j] == 1)
                printf("q%d, ", j);
        }
        printf("}\n");
    }
}

int main() {
    char state1[3], state2[3], inp[2];
    int end;

    printf("Enter total number of states: ");
    scanf("%d", &no_of_states);

    int closure[no_of_states][no_of_states];
    FILE *INPUT = fopen("NFA.txt", "r");

    reset(closure);

    for(int i=0; i<no_of_states; i++){
        int state = i;

        while((end = fscanf(INPUT, "%s %s %s", state1, inp, state2)) != EOF){
            if(inp[0] == 'e' && state == (state1[1] - '0') ){
                closure[i][ (state2[1] - '0') ] = 1;
                state = (state2[1] - '0');
            }
        }
        rewind(INPUT);
    }

    display(closure);

}