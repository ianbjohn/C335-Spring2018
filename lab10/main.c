//C335
//Lab 10
//Ian Johnson
//3/30/18
//Re-write of lab 3 to use interrupts to control the LEDs instead of delays

#include <stm32f30x.h>
#include <f3d_uart.h>
#include <stdio.h>
#include <f3d_led.h>
#include <f3d_user_btn.h>
#include <f3d_systick.h>


int main(void) {
  f3d_uart_init();
  f3d_led_init();
  f3d_user_btn_init();
  f3d_systick_init(100);

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  printf("Test\n");

  f3d_led_all_off();

  while(1) {
    putchar(getchar());
  }
}


#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif
