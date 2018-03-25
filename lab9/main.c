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
#include <f3d_led.h>
#include <f3d_uart.h>
#include <f3d_gyro.h>
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

void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}

FATFS Fatfs;		/* File system object */
FIL Fil;		/* File object */
uint16_t Buff[160][128]; /* File read buffer */

int main(void) { 
  char footer[20];
  int count=0;
  int i, j;

  struct bmpfile_magic magic;
  struct bmpfile_header header;
  struct bmppixel pixel;
  BITMAPINFOHEADER info;

  FRESULT rc;			/* Result code */
  DIR dir;			/* Directory object */
  FILINFO fno;			/* File information object */
  UINT bw, br;
  unsigned int retval;

  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_uart_init();
  f3d_lcd_init();
  f3d_delay_init();
  f3d_rtc_init();

  f_mount(0, &Fatfs);		/* Register volume work area (never fails) */

  f3d_lcd_fillScreen2(RED);

  printf("\nOpen an existing file (img1.bmp).\n");
    rc = f_open(&Fil, "IMG1.BMP", FA_READ);
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
 
  /*
  printf("\nType the file content.\n");
  for (;;) {
    rc = f_read(&Fil, Buff, sizeof Buff, &br);	//Read a chunk of file 
    if (rc || !br) break;			// Error or end of file
    for (i = 0; i < br; i++)		        // Type the data
      putchar(Buff[i]);
    putchar('\n');
  }
  if (rc) die(rc);
  */

  for (i = 0; i < info.height; i++) {
    for (j = 0; j < info.width; j++) {
      //read three bytes at a time and put them into the pixel structure
      f_read(&Fil, (void *) &pixel, sizeof(pixel), &br);
      //printf("pixel %d,%d : %d,%d,%d\n", i, j, pixel.r, pixel.g, pixel.b);
      Buff[i][j] = (uint16_t)(pixel.r >> 3) | (uint16_t)((pixel.g >> 2) << 5) | (uint16_t)((pixel.b >> 3) << 11);
    }
  }
  f3d_lcd_fillScreen3(Buff);
  
  printf("\nClose the file.\n");
  rc = f_close(&Fil);
  if (rc) die(rc);
  
  /*
  printf("\nCreate a new file (hello.txt).\n");
  rc = f_open(&Fil, "HELLO.TXT", FA_WRITE | FA_CREATE_ALWAYS);
  if (rc) die(rc);
  
  printf("\nWrite a text data. (Hello world!)\n");
  rc = f_write(&Fil, "Hello world!\r\n", 14, &bw);
  if (rc) die(rc);
  printf("%u bytes written.\n", bw);
  
  printf("\nClose the file.\n");
  rc = f_close(&Fil);
  if (rc) die(rc);
  
  printf("\nOpen root directory.\n");
  rc = f_opendir(&dir, "");
  if (rc) die(rc);
  
  printf("\nDirectory listing...\n");
  for (;;) {
    rc = f_readdir(&dir, &fno);		// Read a directory item
    if (rc || !fno.fname[0]) break;	// Error or end of dir
    if (fno.fattrib & AM_DIR)
      printf("   <dir>  %s\n", fno.fname);
    else
      printf("%8lu  %s\n", fno.fsize, fno.fname);
  }
  if (rc) die(rc);
  
  printf("\nTest completed.\n");

  rc = disk_ioctl(0,GET_SECTOR_COUNT,&retval);
  printf("%d %d\n",rc,retval);
  */

  while (1);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
