//Ian Johnson
//C335
//HW4 - Register name test suite
//4/5/18

#include <stdio.h>
#include "reg_name.h"

const char* names[16] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "sp", "lr", "pc"};

int main()
{
  printf("String:\tString returned by function call:\n");
  int i;
  for (i = 0; i < 16; i++)
    printf("%s\t%s\n", names[i], reg_name(i));

  return 0;
}
