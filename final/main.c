//Ian Johnson
#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_user_btn.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_nunchuk.h>
#include <f3d_delay.h>
#include <f3d_rtc.h>
#include <f3d_systick.h>
#include <f3d_dac.h>
#include <queue.h>
#include <graphics.h>
#include <ff.h>
#include <diskio.h>
#include <stdio.h>

int main(void)
{
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_uart_init();
  delay(10);
  f3d_timer2_init();
  delay(10);
  f3d_i2c1_init();
  delay(10);
  f3d_user_btn_init();
  delay(10);
  f3d_dac_init();
  delay(10);
  f3d_delay_init();
  delay(10);
  f3d_rtc_init();
  delay(10);
  f3d_dac_init();
  delay(10);
  f3d_systick_init(20000);
  delay(10);
  f3d_lcd_init();
  delay(10);
  f3d_nunchuk_init();
  delay(10);

  printf("Everything working.\n");
  f3d_lcd_fillScreen(RED);
  int i, j;
  for (i = 0; i < ST7735_width / 16; i++) {
    for (j = 0; j < ST7735_height / 16; j++)
      f3d_lcd_drawTile(i * 16, j * 16, tiles, 0);
  }

  while (1);

  return 0;
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif
