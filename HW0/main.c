//Ian Johnson
//C335
//HW0
//1/23/2018
//Simple RPN calculator

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "cell.h"

int main()
{
    printf("Welcome to RPN Calculator\n");

    //main loop, will continue until the user either manually exits program or types EOF
    while (1) {
        char command[256];
        if (scanf("%s", command) == EOF)
		break;	//exits the program at the end of the file/input
        //parse the input
        //check to see if string represents a number
        int isnum = 1;
        for (int i = 0; command[i] != 0 && i < 256; i++) {
            if (!isdigit(command[i])) {
                isnum = 0;
                break;
            }
        }
        if (isnum)
            push(atoi(command));
        else if (command[0] == '+') {
            //addition
            //only perform operation if there are at least two elements already on the stack
            if (!check_stack_empty()) {
                int num1 = pop();
                if (!check_stack_empty())
                    push(num1 + pop());
                else
                    printf("Not enough operands on stack.\n");
            }
            else
                printf("Not enough operands on stack.\n");
        }
        else if (command[0] == '-') {
            //subtraction
            //only perform operation if there are at least two elements already on the stack
            if (!check_stack_empty()) {
                int num1 = pop();
                if (!check_stack_empty())
                    push(num1 - pop());
                else
                    printf("Not enough operands on stack.\n");
            }
            else
                printf("Not enough operands on stack.\n");
        }
        else if (command[0] == '*') {
            //multiplication
            //only perform operation if there are at least two elements already on the stack
            if (!check_stack_empty()) {
                int num1 = pop();
                if (!check_stack_empty())
                    push(num1 * pop());
                else
                    printf("Not enough operands on stack.\n");
            }
            else
                printf("Not enough operands on stack.\n");
        }
        else if (command[0] == '/') {
            //division
            //only perform operation if there are at least two elements already on the stack
            if (!check_stack_empty()) {
                int num1 = pop();
                if (!check_stack_empty())
                    push(num1 / pop());
                else
                    printf("Not enough operands on stack.\n");
            }
            else
                printf("Not enough operands on stack.\n");
        }
        else if (command[0] == 'p')
            print_top_element();
        else if (command[0] == 'f')
            print_stack();
        else if (command[0] == 'c')
            clear_stack();
        else if (command[0] == 'q')
            break;                  //quit the program
        else
            printf("Unknown command. (%s)\n", command);
    }
    return 0;
}
