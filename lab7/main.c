/* main.c ---
*
* Filename: main.c
* Description:
* Author:
* Maintainer: Ian Johnson
* Created: Thu Jan 10 11:23:43 2013
/* Code: */

#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_uart.h>
#include <f3d_user_btn.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_delay.h>
#include <stdio.h>
#include <math.h>

#define TIMER 20000
#define MAX 2

void draw_dot(uint8_t, uint8_t);
void erase_dot(uint8_t, uint8_t);

int main(void) {
  // Set up your inits before you go ahead
   f3d_uart_init();
   f3d_delay_init();
   f3d_user_btn_init();
   f3d_lcd_init();
   f3d_i2c1_init();
   delay(10);
   f3d_accel_init();
   delay(10);
   f3d_mag_init();
   delay(10);

   setvbuf(stdin, NULL, _IONBF, 0);
   setvbuf(stdout, NULL, _IONBF, 0);
   setvbuf(stderr, NULL, _IONBF, 0);

   printf("Test\n");

   float accel_data[3], accel_data_old[3], mag_data[3], mag_data_old[3];
   int mode; //0 - tilt angle, 1 - compass
   float pitch, roll, xh, yh, heading;

   f3d_lcd_fillScreen2(RED);

   f3d_accel_read(accel_data);
   f3d_mag_read(mag_data);

   //printf("%f\n%f\n", accel_data[0], accel_data[1], accel_data[2]);
   //printf("%f\n%f\n", mag_data[0], mag_data[1], mag_data[2]);

   while(1) {
     //keep track of current tiltx and tilty, and the old tiltx and tilty
     //each "tick", clear the area drawn to represent the old tilt position, set the old position to the new position, get data for the new position, and draw the area of the new tilt position
     if (f3d_user_btn_read() == 1) {
       mode ^= 1; //toggle mode
       f3d_lcd_fillScreen2(RED); //eventually add graphical setup functions for the two mode, and call the right one
       if (mode == 1) {
	 f3d_lcd_drawChar(ST7735_width / 2, 0, 'N', BLACK, RED);
	 f3d_lcd_drawChar(ST7735_width - 8, ST7735_height / 2, 'E', BLACK, RED);
	 f3d_lcd_drawChar(ST7735_width / 2, ST7735_height - 8, 'S', BLACK, RED);
	 f3d_lcd_drawChar(0, ST7735_height / 2, 'W', BLACK, RED);
       }
       delay(10);
     } else if (mode == 0) {
       //TILT ANGLE
       //clear old spot
       erase_dot((ST7735_width / 2) - (((int)(roll * (float)(ST7735_width / 2))) / MAX) + 1, (ST7735_height / 2) - (((int)(pitch * (float)(ST7735_height / 2))) / MAX) + 1);

       //get new accel data
       f3d_accel_read(accel_data);

       pitch = atan2f(accel_data[0], sqrt(pow(accel_data[1], 2) + pow(accel_data[2], 2)));
       roll = atan2f(accel_data[1], sqrt(pow(accel_data[0], 2) + pow(accel_data[2], 2)));

       //draw new spot
       draw_dot((ST7735_width / 2) - (((int)(roll * (float)(ST7735_width / 2))) / MAX) + 1, (ST7735_height / 2) - (((int)(pitch * (float)(ST7735_height / 2))) / MAX) + 1);
     } else if (mode == 1) {
       //COMPASS HEADING
       //clear old spot
       erase_dot((ST7735_width / 2) + (((int)(cos(heading) * (float)(ST7735_width / 2))) / MAX) + 1, (ST7735_height / 2) + (((int)(sin(heading) * (float)(ST7735_height / 2))) / MAX) + 1);

       //get new accel and mag data
       f3d_accel_read(accel_data);
       f3d_mag_read(mag_data);

       pitch = atan2f(accel_data[0], sqrt(pow(accel_data[1], 2) + pow(accel_data[2], 2)));
       roll = atan2f(accel_data[1], sqrt(pow(accel_data[0], 2) + pow(accel_data[2], 2)));
       xh = (mag_data[0] * cos(pitch)) + (mag_data[2] * sin(pitch));
       yh = (mag_data[0] * sin(roll) * sin(pitch)) + (mag_data[1] * cos(roll)) - (mag_data[2] * sin(roll) * cos(pitch));
       heading = atan2f(yh, xh);

       //draw new spot
       draw_dot((ST7735_width / 2) + (((int)(cos(heading) * (float)(ST7735_width / 2))) / MAX) + 1, (ST7735_height / 2) + (((int)(sin(heading) * (float)(ST7735_height / 2))) / MAX) + 1);
     }
     //printf("X: %f\nY: %f\nZ: %f\n\n", accel_data[0], accel_data[1], accel_data[2]);
     printf("Pitch: %f\nRoll: %f\n\n", pitch, roll);
     //delay(TIMER / 100);
   }
}

void draw_dot(uint8_t x, uint8_t y) {
  //draws 9 white pixels to represent the dot
  uint8_t i, j;
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++)
      f3d_lcd_drawPixel(x + j, y + i, WHITE);
  }
}

void erase_dot(uint8_t x, uint8_t y) {
  //draws 9 red pixels to represent the dot being erased
  uint8_t i, j;
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++)
      f3d_lcd_drawPixel(x + j, y + i, RED);
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

