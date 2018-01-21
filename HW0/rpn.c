#include <stdio.h>

typedef struct CELL *LIST;
struct CELL {
  int val;
  LIST next;
};

extern LIST stack;

void push(int val)
{
  LIST c = (LIST) malloc(sizeof(struct CELL));
  if (c) {
    c->val = val;
    c->next = stack;
    stack = c;
  } else { /* handle error */ }
}

int main()
{
  return 0;
}
