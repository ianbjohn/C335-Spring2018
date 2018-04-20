//Ian Johnson
//C335
//Final Project - Minesweeper
//4/20/2018

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
#include <f3d_accel.h>
#include <f3d_dac.h>
#include <queue.h>
#include <graphics.h>
#include <ff.h>
#include <diskio.h>
#include <stdio.h>

#define FLAG_SQUARE(X,Y) (board[Y][X] ^= 0b00100000)    //mark the selected square as flagged/unflagged
#define CHECK_FLAGGED(X,Y) (board[Y][X] & 0b00100000)  //check if selected square is flagged
#define CLICK_SQUARE(X,Y) (board[Y][X] |= 0b00010000)  //mark the selected square as clicked
#define CHECK_CLICKED(X,Y) (board[Y][X] & 0b00010000) //check if selected square has already been clicked
#define GET_SQUARE_TYPE(X,Y) (board[Y][X] & 0b00001111) //get what type of square it is
#define SET_SQUARE_TYPE(X,Y,TYPE) (board[Y][X] = TYPE)
#define MINE 9
#define EMPTY 0 //tile corresponding to the numbers 1-8 have indices 1-9 respectively

unsigned char board[10][8] = {0}; //top 4 bytes say what's been done to the square (clicked, flagged), bottom 4 bytes say what type of square it is (empty, number square, mine)
float accel_data[3];

void draw_tile(int x, int y);
void find_mines(int x, int y);
uint16_t random();
void draw_board();

int main(void)
{
  int cursor_x = 0, cursor_y = 0, num_clicks = 0;
  nunchuk_t nunchuk;

  //initialize all the drivers
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
  f3d_accel_init();
  f3d_lcd_init();
  delay(10);
  f3d_nunchuk_init();
  delay(10);

  printf("Everything working.\n");
  f3d_lcd_fillScreen(RED);

  //populate the screen with clickable squares, and initialize board array
  int i, j;
  for (i = 0; i < ST7735_height / 16; i++) {
    for (j = 0; j < ST7735_width / 16; j++)
      f3d_lcd_drawTile(j * 16, i * 16, tiles, 0);
  }

  //board[0][0] = 1;

  while (1) {
    f3d_nunchuk_read(&nunchuk);

    if (nunchuk.jx == 0xFF) {
      //"undraw" old cursor
      if ((++cursor_x) >= (ST7735_width / 16))
	cursor_x = 0;
      //draw new cursor
      printf("X: %d, Y: %d\n", cursor_x, cursor_y);
    } else if (nunchuk.jx == 0x00) {
      //"undraw" old cursor
      if ((--cursor_x) < 0)
	cursor_x = (ST7735_width / 16) - 1;
      //draw new cursor
      printf("X: %d, Y: %d\n", cursor_x, cursor_y);
    } else if (nunchuk.jy == 0xFF) {
      //"undraw" old cursor
      if ((++cursor_y) >= (ST7735_height / 16))
	cursor_y = 0;
      //draw new cursor
      printf("X: %d, Y: %d\n", cursor_x, cursor_y);
    } else if (nunchuk.jy == 0x00) {
      //"undraw" old cursor
      if ((--cursor_y) < 0)
	  cursor_y = (ST7735_height / 16) - 1;
      //draw new cursor
      printf("X: %d, Y: %d\n", cursor_x, cursor_y);
    } else if (nunchuk.z) {
      if (num_clicks == 0) {
	//set random seed based off first click (The first click can never be a mine
	int mines = 0;
	while (mines <= 20) {
	  int x = random() % (ST7735_width / 16);
	  int y = random() % (ST7735_height / 16);
	  if (x != cursor_x && y != cursor_y && GET_SQUARE_TYPE(x, y) == EMPTY) {
	    SET_SQUARE_TYPE(x, y, MINE);
	    mines++;
	  }
	}
	find_mines(cursor_x, cursor_y);
      } else {
	if (CHECK_CLICKED(cursor_x, cursor_y) == 0 && CHECK_FLAGGED(cursor_x, cursor_y) == 0) {
	  num_clicks++;
	  if (GET_SQUARE_TYPE(cursor_x, cursor_y) == MINE) {
	    //game over
	    //re-draw the board drawing mines, with a check over it if it was flagged, and an X over squares that were flagged but which didn't have mines
	    printf("Game over.\n");
	    while(1); //hang the program for now
	  } else {
	    //recursively check for adjacent mines, and draw the number
	    find_mines(cursor_x, cursor_y);
	  }
	}
      }
    } else if (nunchuk.c) {
      if (CHECK_CLICKED(cursor_x, cursor_y) == 0)
	FLAG_SQUARE(cursor_x, cursor_y);
      //draw the new tile
    }
  }

  return 0;
}

void draw_tile(int x, int y) {
  if (CHECK_CLICKED(x, y) == 0) {
    //if it's game over, show the flagged squares as whether or not they had mines
    if (CHECK_FLAGGED(x, y) == 1); //draw flag tile
    else
      f3d_lcd_drawTile(x, y, tiles, 0);
  } else {
    f3d_lcd_drawTile(x * 16, y * 16, tiles, GET_SQUARE_TYPE(x, y) + 1);
  }
}

void find_mines(int x, int y) {
  //recursively searches the board for mines, and marks squares with no mines as clicked
  CLICK_SQUARE(x, y);
  
  int count = 0;
  //left
  if ((x - 1 >= 0) && (CHECK_CLICKED(x - 1, y) == 0)) {
    if (GET_SQUARE_TYPE(x - 1, y) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x - 1, y) == EMPTY)
      find_mines(x - 1, y);
  }
  //upper-left
  if ((x - 1 >= 0 && y - 1 >= 0) && (CHECK_CLICKED(x - 1, y - 1) == 0)) {
    if (GET_SQUARE_TYPE(x - 1, y - 1) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x - 1, y - 1) == EMPTY)
      find_mines(x - 1, y - 1);
  }
  //up
  if ((y - 1 >= 0) && (CHECK_CLICKED(x, y - 1) == 0)) {
    if (GET_SQUARE_TYPE(x, y - 1) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x, y - 1) == EMPTY)
      find_mines(x, y - 1);
  }
  //upper-right
  if ((x + 1 < (ST7735_width / 16) && y - 1 >= 0) && (CHECK_CLICKED(x + 1, y - 1) == 0)) {
    if (GET_SQUARE_TYPE(x + 1, y - 1) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x + 1, y - 1) == EMPTY)
      find_mines(x + 1, y - 1);
  }
  //right
  if ((x + 1 < (ST7735_width / 16)) && (CHECK_CLICKED(x + 1, y) == 0)) {
    if (GET_SQUARE_TYPE(x + 1, y) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x + 1, y) == EMPTY)
      find_mines(x + 1, y);
  }
  //bottom-right
  if ((x + 1 < (ST7735_width / 16) && y + 1 < (ST7735_height / 16)) && (CHECK_CLICKED(x + 1, y + 1) == 0)) {
    if (GET_SQUARE_TYPE(x + 1, y + 1) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x + 1, y + 1) == EMPTY)
      find_mines(x + 1, y + 1);
  }
  //bottom
  if ((y + 1 < (ST7735_height / 16)) && (CHECK_CLICKED(x, y + 1) == 0)) {
    if (GET_SQUARE_TYPE(x, y + 1) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x, y + 1) == EMPTY)
      find_mines(x, y + 1);
  }
  //bottom-left
  if ((x >= 0 && y < (ST7735_height / 16)) && (CHECK_CLICKED(x - 1, y + 1) == 0)) {
    if (GET_SQUARE_TYPE(x - 1, y + 1) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x - 1, y + 1) == EMPTY)
      find_mines(x - 1, y + 1);
  }

  //set square's type to the # of surrounding mines
  SET_SQUARE_TYPE(x, y,  count);
  CLICK_SQUARE(x, y);
  draw_tile(x, y);
}

uint16_t random() {
  //a work in progress
  uint16_t seed = *((uint16_t *)(&accel_data[0])) + *((uint16_t *)(&accel_data[1])) + *((uint16_t *)(&accel_data[2]));
  //printf("%d\n", seed);
  f3d_accel_read(accel_data);
  return seed + *((uint16_t *)(&accel_data[0])) + *((uint16_t *)(&accel_data[1])) + *((uint16_t *)(&accel_data[2]));
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif
