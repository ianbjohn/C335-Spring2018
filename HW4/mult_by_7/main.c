//Ian Johnson
//C335
//HW4 - Multiplication by 7 test suite
//4/4/18

#include <stdio.h>
#include "mult_by_7.h"

int main()
{
  //test with products 0-100
  int i, error_count = 0;
  for (i = 0; i <= 100; i++) {
    if (mult_by_7(i) != (i * 7)) {
      printf("Error when multiplying %d by 7. %d should be %d\n", i, mult_by_7(i), i * 7);
      error_count++;
    }
  }

  if (error_count == 0)
    printf("mult_by_7 passed!\n");
  else
    printf("Number of mult_by_7 errors: %d\n", error_count);

  return 0;
}
