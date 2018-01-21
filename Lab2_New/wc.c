//Ian Johnson
//C335
//1/20/2018
//Lab 2
//Word Count

#include <stdio.h>
#define SPACE 0x20
#define NEWLINE 0x0A

int main()
{
  char filename[256];
  int c;
  int wordcount = 0, linecount = 1, charactercount = 0;
  FILE* f;

  printf("Enter file name: ");
  scanf("%s", filename);
  printf("\n");
  if ((f = fopen(filename, "r")) == NULL) {
    printf("ERROR: File does not exist.\n");
    return 0;
  }

  while ((c = fgetc(f)) != EOF) {
    charactercount++;
    if (c == SPACE)
      wordcount++;
    else if (c == NEWLINE)
      linecount++;
  }

  printf("%d\t%d\t%d\n", linecount, wordcount, charactercount);
  fclose(f);
  return 0;
}
