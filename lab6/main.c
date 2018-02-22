/* main.c ---
*
* Filename: main.c
* Description:
* Author: Ian Johnson
* Maintainer:
* Created: Thu Jan 10 11:23:43 2013
* Modified: Fri Feb 16 2018 - 2/22/18
/* Code: */

#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_led.h> // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_delay.h>
#include <stdio.h>

#define TIMER 20000
#define MAX 100    //an (arbitarily) imposed "maximum value" that the gyro data can be

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
  f3d_lcd_fillScreen2(RED); //set background color to red

  while (1) {
    //main loop
    //"clear" the area where the data is going to be drawn by setting the background color there to red (This'll get rid of the white pixels from the previous frame)
    //f3d_lcd_fillArea(8, 0, ST7735_width - 8, 24, RED); //student-defined function
    f3d_gyro_getdata(gyro_data);
    
    //display the gyro data on LCD as a bar graph
    int i;
    for (i = 0; i < 3; i++) {
      //the scary-looking math here is basically converting the gyro values to how much of the screen they should take up
      if (gyro_data[i] < 0) { //negative values go down
	f3d_lcd_fillArea((ST7735_width / 3) * i, 0, ST7735_width / 3, ST7735_height / 2, RED); //clear "positive" area
	f3d_lcd_fillArea((ST7735_width / 3) * i, ST7735_height / 2, ST7735_width / 3, ((int)(-gyro_data[i]) * (ST7735_height / 2)) / MAX, WHITE); //draw bar
	f3d_lcd_fillArea((ST7735_width / 3) * i, (ST7735_height / 2) + (((int)(-gyro_data[i]) * (ST7735_height / 2)) / MAX), ST7735_width / 3, (ST7735_height / 2) - (((int)(-gyro_data[i]) * (ST7735_height / 2)) / MAX), RED); //clear any remaining "negative" area
      }
      else if (gyro_data[i] >= 0) { //positive values go up
	f3d_lcd_fillArea((ST7735_width / 3) * i, 0, ST7735_width / 3, (ST7735_height / 2) - (((int)gyro_data[i] * (ST7735_height / 2)) / MAX), RED); //clear any remaining "positive" area
	f3d_lcd_fillArea((ST7735_width / 3) * i, (ST7735_height / 2) - (((int)(gyro_data[i]) * (ST7735_height / 2)) / MAX), ST7735_width / 3, ((int)(gyro_data[i]) * (ST7735_height / 2)) / MAX, WHITE); //draw bar
	f3d_lcd_fillArea((ST7735_width / 3) * i, ST7735_height / 2, ST7735_width / 3, ST7735_height / 2, RED); //clear "negative" area
      }
    }
    //after drawing the bars, redraw the "axis" on top of it
    f3d_lcd_fillArea(0, ST7735_height / 2, ST7735_width, 1, BLACK);
    f3d_lcd_drawChar(17, ST7735_height - 8, 'X', WHITE, BLACK); //also give the user some HUD
    f3d_lcd_drawChar(62, ST7735_height - 8, 'Y', WHITE, BLACK);
    f3d_lcd_drawChar(104, ST7735_height - 8, 'Z', WHITE, BLACK);
    
    //send the textual data via UART
    printf("X: %f\nY: %f\nZ: %f\n\n", gyro_data[0], gyro_data[1], gyro_data[2]);

    f3d_delay_uS(TIMER);
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
