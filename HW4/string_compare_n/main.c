//Ian Johnson
//C335
//HW4 - String_compare_n test suite
//4/5/18

#include <stdio.h>
#include <string.h>
#include "string_compare_n.h"

#define NUM_STRINGS 9

const char* strings[NUM_STRINGS] = {"the", "quick", "brown", "fox", "jumps", "over", "the", "lazy", "dog"};

int main()
{
  int error_count = 0;
  int i, j, k;

  for (i = 0; i < NUM_STRINGS; i++) {
    for (j = 0; j < NUM_STRINGS; j++) {
      for (k = 1; k <= 3; k++) {
	printf("%s VS %s, %d character(s): %d should be %d", strings[i], strings[j], k, string_compare_n(strings[i], strings[j], k),  strncmp(strings[i], strings[j], k));
	if (string_compare_n(strings[i], strings[j], k) != strncmp(strings[i], strings[j], k)) {
	  printf("\tERROR!!!\n");
	  error_count++;
	}
	else
	  printf("\n");
      }
    }
  }

  if (error_count == 0)
    printf("string_compare_n PASSED!\n");
  else
    printf("Number of errors: %d\n", error_count);

    //char* s1 = "the";
    //char* s2 = "quick";
    //printf("%d\n", string_compare_n(s1, s2, 3));

  return 0;
}
