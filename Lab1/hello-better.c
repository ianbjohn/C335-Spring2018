//C335
//Lab 1
//Ian Johnson
//1/12/18
#include <stdio.h>
#include <unistd.h>

int main()
{
  int i = 1;
  while (1) {
    printf("Hello, world!\n");
    for (int k = 0; k < i; k++) {
      for (int l = 0; l < i; l++) {
	printf("*");
	printf("\0007"); //ring the bell
      }
      printf("\n");
    }
    sleep(3);
    i++;
  }

  return 0;
}
