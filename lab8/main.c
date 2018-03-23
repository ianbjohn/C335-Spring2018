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
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_delay.h>
#include <f3d_nunchuk.h>
#include <f3d_gyro.h>
#include <stdio.h>
#include <math.h>

#define TIMER 20000
#define MAX 2
#define GYRO_MAX 100

void draw_dot(uint8_t, uint8_t);
void erase_dot(uint8_t, uint8_t);
void new_app(int mode);

int main(void){ 
  // Set up your inits before you go ahead
   f3d_uart_init();
   delay(100);
   f3d_delay_init();
   delay(100);
   f3d_lcd_init();
   delay(100);
   f3d_gyro_init();
   delay(100);
   f3d_i2c1_init();
   delay(100);
   f3d_accel_init();
   delay(100);
   f3d_mag_init();
   delay(100);
   printf("Here\n");
   f3d_nunchuk_init();

   setvbuf(stdin, NULL, _IONBF, 0);
   setvbuf(stdout, NULL, _IONBF, 0);
   setvbuf(stderr, NULL, _IONBF, 0);

   printf("Test\n");

   float accel_data[3], mag_data[3], gyro_data[3];
   int mode; //0 - tilt angle, 1 - compass, 2 - gyro
   float pitch, roll, xh, yh, heading;
   nunchuk_t nunchuk;

   f3d_lcd_fillScreen2(RED);

   f3d_accel_read(accel_data);
   f3d_mag_read(mag_data);
   f3d_gyro_getdata(gyro_data);

   f3d_nunchuk_read(&nunchuk);
   printf("%x %c %d %d %d %c %c\n", nunchuk.jx, nunchuk.jy, nunchuk.ax, nunchuk.ay, nunchuk.az, nunchuk.c, nunchuk.z);
   delay(TIMER / 100);

   while(1) {
     //check if nunchuck joystick was moved left or right, and go to previous/next mode respectively
     f3d_nunchuk_read(&nunchuk);
     printf("%d %d %d %d %d %d %c\n", nunchuk.jx, nunchuk.jy, nunchuk.ax, nunchuk.ay, nunchuk.az, nunchuk.c, nunchuk.z);
     if (nunchuk.jx == 0x00 || nunchuk.z) {
       mode--;
       if (mode < 0) mode = 2;
       new_app(mode);
     } else if (nunchuk.jx == 0xFF || nunchuk.c) {
       mode++;
       if (mode > 2) mode = 0;
       new_app(mode);
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
     } else if (mode == 2) {
       //"clear" the area where the data is going to be drawn by setting the background color there to red (This'll get rid of the white pixels from the previous frame)
       //f3d_lcd_fillArea(8, 0, ST7735_width - 8, 24, RED); //student-defined function
       f3d_gyro_getdata(gyro_data);
       if (gyro_data[0] > (float)GYRO_MAX) gyro_data[0] = GYRO_MAX;
       else if (gyro_data[0] < -(float)GYRO_MAX) gyro_data[0] = -GYRO_MAX;
       if (gyro_data[1] > (float)GYRO_MAX) gyro_data[1] = GYRO_MAX;
       else if (gyro_data[1] < -(float)GYRO_MAX) gyro_data[1] = -GYRO_MAX;
       if (gyro_data[2] > (float)GYRO_MAX) gyro_data[2] = GYRO_MAX;
       else if (gyro_data[2] < -(float)GYRO_MAX) gyro_data[2] = -GYRO_MAX;
    
       //display the gyro data on LCD as a bar graph
       int i;
       for (i = 0; i < 3; i++) {
	 //the scary-looking math here is basically converting the gyro values to how much of the screen they should take up
	 if (gyro_data[i] < 0) { //negative values go down
	   f3d_lcd_fillArea((ST7735_width / 3) * i, 0, ST7735_width / 3, ST7735_height / 2, RED); //clear "positive" area
	   f3d_lcd_fillArea((ST7735_width / 3) * i, ST7735_height / 2, ST7735_width / 3, ((int)(-gyro_data[i]) * (ST7735_height / 2)) / GYRO_MAX, WHITE); //draw bar
	   f3d_lcd_fillArea((ST7735_width / 3) * i, (ST7735_height / 2) + (((int)(-gyro_data[i]) * (ST7735_height / 2)) / GYRO_MAX), ST7735_width / 3, (ST7735_height / 2) - (((int)(-gyro_data[i]) * (ST7735_height / 2)) / GYRO_MAX), RED); //clear any remaining "negative" area
	 }
	 else if (gyro_data[i] >= 0) { //positive values go up
	   f3d_lcd_fillArea((ST7735_width / 3) * i, 0, ST7735_width / 3, (ST7735_height / 2) - (((int)gyro_data[i] * (ST7735_height / 2)) / GYRO_MAX), RED); //clear any remaining "positive" area
	   f3d_lcd_fillArea((ST7735_width / 3) * i, (ST7735_height / 2) - (((int)(gyro_data[i]) * (ST7735_height / 2)) / GYRO_MAX), ST7735_width / 3, ((int)(gyro_data[i]) * (ST7735_height / 2)) / GYRO_MAX, WHITE); //draw bar
	   f3d_lcd_fillArea((ST7735_width / 3) * i, ST7735_height / 2, ST7735_width / 3, ST7735_height / 2, RED); //clear "negative" area
	 }
       }
       //after drawing the bars, redraw the "axis" on top of it
       f3d_lcd_fillArea(0, ST7735_height / 2, ST7735_width, 1, BLACK);
       char str[25];
       sprintf(str, "X:%.2f Y:%.2f Z:%.2f", gyro_data[0], gyro_data[1], gyro_data[2]); //give user some HUD
       f3d_lcd_drawString(0, ST7735_height - 8, str, WHITE, RED);
     }
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

void new_app(int mode) {
  switch (mode) {
  case 0:
    f3d_lcd_fillScreen2(RED);
    break;
  case 1:
    f3d_lcd_fillScreen2(RED);
    f3d_lcd_drawChar(ST7735_width / 2, 0, 'N', BLACK, RED);
    f3d_lcd_drawChar(ST7735_width - 8, ST7735_height / 2, 'E', BLACK, RED);
    f3d_lcd_drawChar(ST7735_width / 2, ST7735_height - 8, 'S', BLACK, RED);
    f3d_lcd_drawChar(0, ST7735_height / 2, 'W', BLACK, RED);
    break;
  case 2:
    f3d_lcd_fillScreen2(RED);
    break;
  }
  delay(1000);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */

