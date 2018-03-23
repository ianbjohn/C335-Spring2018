//Ian Johnson
//C335
//2/7/2018
//Lab 4 (modified from lab 2)
//Word Count

#include <f3d_uart.h>
#include <stdio.h>
#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#define SPACE 0x20
#define NEWLINE 0x0A

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}


int main()
{
  f3d_uart_init();

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  int c;
  int wordcount = 0, linecount = 1, charactercount = 0;

  while ((c = getchar()) != 0x1b) { //esc
    putchar(c);
    charactercount++;
    if (c == ' ')
      wordcount++;
    else if (c == '\n')
      linecount++;
  }

  printf("\n%d %d %d\n", linecount, wordcount, charactercount);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif
