//Ian Johnson
//C335
//HW4 - Member array test suite
//4/4/18

#include <stdio.h>
#include "member_array.h"

int main()
{
  unsigned int my_array[10] = {5,1,4,2,6,3,7,9,8,0};

  int i, error_count = 0;
  for (i = 0; i < 10; i++) {
    if (member_array(my_array, my_array[i], 10) != member_array_c(my_array, my_array[i], 10)) {
      printf("Error when comparing the %d'th element. %d should be %d.\n", i, member_array(my_array, my_array[i], 10), member_array_c(my_array, my_array[i], 10));
      error_count++;
    }
  }

  //make sure we don't get any false positives
  if (member_array(my_array, 10, 10) != 0) {
    printf("Error, false positive.\n");
    error_count++;
  }

  if (error_count == 0)
    printf("member_array passed!\n");
  else
    printf("Number of errors: %d\n", error_count);
}
