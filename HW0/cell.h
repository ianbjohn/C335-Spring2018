#ifndef CELL_H_INCLUDED
#define CELL_H_INCLUDED

typedef struct CELL *LIST;
struct CELL {
    int val;
    LIST next;
};

extern LIST stack;
extern void push(int val);
extern int pop();
extern void print_top_element();
extern void print_stack();
extern void clear_stack();
extern int check_stack_empty();

#endif // CELL_H_INCLUDED
