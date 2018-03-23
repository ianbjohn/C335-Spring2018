/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */
//Code added by Ian Johnson
/* Code: */

#include <f3d_uart.h>
#include <stdio.h>
#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void) {
  f3d_uart_init();

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  printf("Test");

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
  wordcount++; //it's likely that there wasn't a space before the end of the input, so account for a potential off-by-one error

  printf("\n%d %d %d\n", linecount, wordcount, charactercount);

  /*
  while (1) {
    putchar(getchar()); //receive user input and send it back to computer, printing it to the screen
  }
  */
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
