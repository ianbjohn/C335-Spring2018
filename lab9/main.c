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
/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_uart.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <f3d_rtc.h>
#include <ff.h>
#include <diskio.h>
#include <stdio.h>
#include "bmp.h"
#include <string.h>
#include <math.h>

void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}

void loadImage(int c);

FATFS Fatfs;		/* File system object */
FIL Fil;		/* File object */
uint16_t Buff[128]; /* File read buffer */
FRESULT rc;			/* Result code */
DIR dir;			/* Directory object */
FILINFO fno;			/* File information object */
UINT bw, br;
unsigned int retval;
struct bmpfile_magic magic;
struct bmpfile_header header;
struct bmppixel pixel;
BITMAPINFOHEADER info;

float accel_data[3], mag_data[3];
float pitch, roll, heading,  xh, yh;
int orientation; //which way to rotate the image

//file names strings
const char* fileNames[3] = {"IMG1.BMP", "IMG2.BMP", "IMG3.BMP"};

int main(void) { 
  int count=0;
  nunchuk_t nunchuk;

  f3d_uart_init();
  delay(100);
  f3d_delay_init();
  delay(100);
  f3d_lcd_init();
  printf("LCD good\n");
  delay(100);
  f3d_i2c1_init();
  delay(100);
  f3d_accel_init();
  printf("Accel good\n");
  delay(100);
  f3d_mag_init();
  printf("Mag good\n");
  delay(100);
  f3d_rtc_init();
  delay(100);
  printf("good\n");
  f3d_nunchuk_init();
  printf("Also good\n");

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  loadImage(0);

  //flip through images with the nunchuk
  while (1) {
    /*
      if Z button pressed
        move to next image
      else if C button pressed
        move to previous image
    */
    
    memset(&nunchuk, 0, sizeof(nunchuk_t));
    f3d_nunchuk_read(&nunchuk);
    
     //get new accel and mag data
     f3d_accel_read(accel_data);
     f3d_mag_read(mag_data);

     pitch = atan2f(accel_data[0], sqrt(pow(accel_data[1], 2) + pow(accel_data[2], 2)));
     roll = atan2f(accel_data[1], sqrt(pow(accel_data[0], 2) + pow(accel_data[2], 2)));
     xh = (mag_data[0] * cos(pitch)) + (mag_data[2] * sin(pitch));
     yh = (mag_data[0] * sin(roll) * sin(pitch)) + (mag_data[1] * cos(roll)) - (mag_data[2] * sin(roll) * cos(pitch));
     heading = atan2f(yh, xh);
     printf("%f\n", heading);

      //change orientation if need be
     if (orientation == 0) { //up
       if (heading > 1.5f) {
	 orientation = 1;
	 loadImage(count);
       } else if (heading < -1.4f) {
	 orientation = 3;
	 loadImage(count);
       }
     } else if (orientation == 1) { //left
       if (heading > 2.8f) {
	 orientation = 2;
	 loadImage(count);
       } else if (heading <= 1.5f) {
	 orientation = 0;
	 loadImage(count);
       }
     } else if (orientation == 2) { //down
       if (heading > 0 && heading <= 2.8f) {
	 orientation = 1;
	 loadImage(count);
       } else if (heading < 0 && heading > -2.0f) {
	 orientation = 3;
	 loadImage(count);
       }
     } else { //right
       if (heading <= -2.0f) {
	 orientation = 2;
	 loadImage(count);
       } else if (heading > -0.5f) {
	 orientation = 0;
	 loadImage(count);
       }
     }

    if (nunchuk.z) {
      //printf("%d %d %d %d %d %d %d\n", nunchuk.jx, nunchuk.jy, nunchuk.ax, nunchuk.ay, nunchuk.az, nunchuk.c, nunchuk.z);
      if ((++count) > 2) {
	count = 0;
      }
      loadImage(count);
      f3d_nunchuk_read(&nunchuk);
    } else if (nunchuk.c) {
      if ((--count) < 0) {
	count = 2;
      }
      loadImage(count);
      f3d_nunchuk_read(&nunchuk);
    }
  }
}

void loadImage(int c) {
  int i, j;
   
  f_mount(0, &Fatfs);		/* Register volume work area (never fails) */

  printf("\nOpen an existing file (%s).\n", fileNames[c]);
  rc = f_open(&Fil, fileNames[c], FA_READ);
  if (rc) die(rc);

  //parse image data
  f_read(&Fil, (void *) &magic, 2, &br); //get magic numbers
  if (rc) die(rc);
  printf("Magic %c%c\n", magic.magic[0], magic.magic[1]);
  f_read(&Fil, (void *) &header, sizeof(header), &br); //get header
  if (rc) die(rc);
  printf("file size: %d offset %d\n", header.filesz, header.bmp_offset);
  f_read(&Fil, (void *) &info, sizeof(info), &br); //get info
  if (rc) die(rc);
  printf("Width: %d\nHeight:%d\n\n", info.width, info.height);

  for (i = 0; i < info.height; i++) {
    for (j = 0; j < info.width; j++) {
      //read three bytes at a time and put them into the pixel structure
      f_read(&Fil, (void *) &pixel, sizeof(pixel), &br);
      //printf("pixel %d,%d : %d,%d,%d\n", i, j, pixel.r, pixel.g, pixel.b);
      Buff[j] = (uint16_t)(pixel.r >> 3) | (uint16_t)((pixel.g >> 2) << 5) | (uint16_t)((pixel.b >> 3) << 11); //convert 24bpp -> 16bpp
    }
    //draw the image correctly based on the angle of the board
    if (orientation == 0) //up
      f3d_lcd_fillRow(info.height - 1 - i, Buff); //too much RAM required to buffer the whole image, so we buffer just the rows instead
    else if (orientation == 1) //left
      f3d_lcd_fillColumn(info.height - 1 - i, Buff);
    else if (orientation == 2) //down
      f3d_lcd_fillRow(i, Buff);
    else  //right
      f3d_lcd_fillColumn(i, Buff);
  }
  
  printf("\nClose the file.\n");
  rc = f_close(&Fil);
  if (rc) die(rc);
  
  printf("\nTest completed.\n");

  rc = disk_ioctl(0,GET_SECTOR_COUNT,&retval);
  printf("%d %d\n",rc,retval);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
