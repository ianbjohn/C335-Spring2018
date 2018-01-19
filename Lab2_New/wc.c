#include <stdio.h>
#define SPACE 0x20
#define NEWLINE 0x0A

int main()
{
  int c;
  int wordcount = 0, linecount = 1, charactercount = 0;
  while ((c = getchar()) != EOF) {
    charactercount++;
    if (c == SPACE)
      wordcount++;
    else if (c == NEWLINE)
      linecount++;
  }

  printf("%d\t%d\t%d\n", linecount, wordcount, charactercount);
  return 0;
}
