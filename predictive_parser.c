#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

// Stack structure
char stack[MAX];
int top = -1;

// Push element into stack
void push(char c) {
    if(top >= MAX - 1) {
        printf("Stack Overflow\n");
        return;
    }
    stack[++top] = c;
}

// Pop element from stack
char pop() {
    if(top == -1) return '\0';
    return stack[top--];
}

// Peek top element
char peek() {
    if(top == -1) return '\0';
    return stack[top];
}

// Check if character is terminal
int isTerminal(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == 'i' || c == '$');
}

// Terminal index in parsing table
int getTerminalIndex(char c) {
    switch(c) {
        case 'i': return 0;
        case '+': return 1;
        case '-': return 2;
        case '*': return 3;
        case '/': return 4;
        case '(': return 5;
        case ')': return 6;
        case '$': return 7;
        default: return -1;
    }
}

// Non-terminal index in parsing table
int getNonTerminalIndex(char c) {
    switch(c) {
        case 'E': return 0;
        case 'X': return 1; // E'
        case 'T': return 2;
        case 'Y': return 3; // T'
        case 'F': return 4;
        default: return -1;
    }
}

// Convert input to 'i' for identifier or number
void preprocessInput(char *input, char *output) {
    int j = 0;
    for(int i = 0; input[i] != '\0'; i++) {
        if(isalpha(input[i]) || isdigit(input[i])) {
            if(j == 0 || output[j-1] != 'i') output[j++] = 'i';
        } else {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';
}

int main() {
    char input[100], processed[100];
    printf("Enter arithmetic expression: ");
    scanf("%s", input);

    preprocessInput(input, processed);
    strcat(processed, "$");

    push('$');
    push('E');

    // LL(1) Parsing Table
    // Rows: Non-terminals E,X,T,Y,F
    // Columns: Terminals i,+,-,*,/,(,),$
    char *table[5][8] = {
        /* E */ {"TX", NULL, NULL, NULL, NULL, "TX", NULL, NULL},
        /* X */ {NULL, "+TX", "-TX", NULL, NULL, NULL, "e", "e"},
        /* T */ {"FY", NULL, NULL, NULL, NULL, "FY", NULL, NULL},
        /* Y */ {NULL, "e", "e", "*FY", "/FY", NULL, "e", "e"},
        /* F */ {"i", NULL, NULL, NULL, NULL, "(E)", NULL, NULL}
    };

    int i = 0;
    char a = processed[i];
    char X;

    printf("\nParsing Steps:\n");
    printf("Stack\t\tInput\t\tAction\n");

    while(top != -1) {
        X = peek();
        printf("%s\t\t%s\t\t", stack, processed + i);

        if(isTerminal(X)) {
            if(X == a) {
                printf("Match %c\n", a);
                pop();
                i++;
                a = processed[i];
            } else {
                printf("Syntax Error!\n");
                return 0;
            }
        } else {
            int row = getNonTerminalIndex(X);
            int col = getTerminalIndex(a);

            if(table[row][col] != NULL) {
                printf("%c -> %s\n", X, table[row][col]);
                pop();
                char *prod = table[row][col];
                if(strcmp(prod, "e") != 0) {
                    int len = strlen(prod);
                    for(int k = len - 1; k >= 0; k--) {
                        push(prod[k]);
                    }
                }
            } else {
                printf("Syntax Error!\n");
                return 0;
            }
        }
    }

    if(a == '$') {
        printf("\nExpression is syntactically correct.\n");
    } else {
        printf("\nSyntax Error in expression.\n");
    }

    return 0;
}
