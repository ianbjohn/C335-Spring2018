//Ian Johnson
//C335
//HW4 - C implementation of member_array
//4/4/18

#include "member_array.h"

unsigned int member_array_c(unsigned int *array, int e, int size)
{
  int i;
  for (i = 0; i < size; i++) {
    if (array[i] == e)
      return 1;
  }
  return 0;
}
