//Ian Johnson
//C335
//HW0
//1/23/2018
//Simple implementation of a stack

#include <stdio.h>
#include "cell.h"

LIST stack;

void push(int val)
{
    //pushes an element onto the stack, alerting the user if there was an overflow
    LIST c = (LIST)malloc(sizeof(struct CELL));
    if (c) {
        c->val = val;
        c->next = stack;
        stack = c;
    }
    else
        printf("Stack overflow.\n");
}

int pop()
{
    //pops an element off the stack, alerting the user if there was an underflow
    LIST c = stack;
    int val;
    if (c) {
        val = c->val;
        stack = c->next;
        free(c);
    }
    else
        printf("Stack underflow.\n");
    return val;
}

void print_top_element()
{
    //prints the top element in the stack without altering the stack itself
    if (stack)
        printf("%d\n", stack->val);
    else
        printf("Stack is currently empty.\n");
}

void print_stack()
{
    //prints all the contents of the stack without altering it
    LIST c = stack;
    if (!c)
        printf("Stack is currently empty.\n");
    else {
        while (c) {
            printf("%d\n", c->val);
            c = c->next;
        }
    }
}

void clear_stack()
{
    //deallocates all the contents of the stack
    LIST c = stack;
    while (c) {
        stack = c->next;
        free(c);
        c = stack;
    }
    printf("Stack cleared.\n");
}

int check_stack_empty()
{
    return (stack == NULL);
}
