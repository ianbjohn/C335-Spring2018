/* main.c ---
*
* Filename: main.c
* Description:
* Author: Ian Johnson
* Maintainer:
* Created: Thu Jan 10 11:23:43 2013
* Modified: Fri Feb 16 2018
/* Code: */

#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_led.h> // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_delay.h>
#include <stdio.h>

#define TIMER 20000

int main(void) {
  // If you have your inits set up, this should turn your LCD screen red
  f3d_lcd_init();
  f3d_uart_init();
  f3d_gyro_init();
  f3d_delay_init();

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  float gyro_data[3];
  f3d_lcd_fillScreen2(RED);
  f3d_lcd_drawChar(0, 0, 'X', WHITE, BLACK); //let the user know what's happening on screen
  f3d_lcd_drawChar(0, 8, 'Y', WHITE, BLACK);
  f3d_lcd_drawChar(0, 16, 'Z', WHITE, BLACK);

  while (1) {
    //main loop
    //"clear" the area where the data is going to be drawn by setting the background color there to red (This'll get rid of the white pixels from the previous frame)
    //f3d_lcd_fillArea(8, 0, ST7735_width - 8, 24, RED); //student-defined function
    f3d_gyro_getdata(gyro_data);
    
    //display the gyro data on LCD
    int i;
    for (i = 1; i < 2; i++) {
      if (gyro_data[i] < 0) {
	f3d_lcd_fillArea((ST7735_width / 3) * i, 0, ST7735_width / 3, ST7735_height / 2, RED); //clear "positive" area
	f3d_lcd_fillArea((ST7735_width / 3) * i, ST7735_height / 2, ST7735_width / 3, ((int)(-gyro_data[i]) * (ST7735_height / 2)) / 1000 , WHITE); //draw bar
	f3d_lcd_fillArea((ST7735_width / 3) * i, (ST7735_height / 2) + ((int)(-gyro_data[i]) * (ST7735_height / 2)) / 1000, ST7735_width / 3, (ST7735_height / 2) - (((int)(-gyro_data[i]) * (ST7735_height / 2)) / 1000), BLUE); //clear any remaining "negative" area
      }
      else if (gyro_data[i] >= 0) {
	f3d_lcd_fillArea((ST7735_width / 3) * i, 0, ST7735_width / 3, (ST7735_height / 2) - (((int)gyro_data[i] * (ST7735_height / 2)) / 1000), BLUE); //clear any remaining "positive" area
	f3d_lcd_fillArea((ST7735_width / 3) * i, (ST7735_height / 2) - (((int)(gyro_data[i]) * (ST7735_height / 2)) / 1000), ST7735_width / 3, ((int)(gyro_data[i]) * (ST7735_height / 2)) / 1000, WHITE); //draw bar
	f3d_lcd_fillArea((ST7735_width / 3) * i, ST7735_height / 2, ST7735_width / 3, ST7735_height / 2, RED); //clear "negative" area
      }
    }
    
    //send the textual data via UART
    printf("X: %f\nY: %f\nZ: %f\n\n", gyro_data[0], gyro_data[1], gyro_data[2]);

    //f3d_delay_uS(TIMER);
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
