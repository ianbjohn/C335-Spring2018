//Ian Johnson
//C335
//Final Project - Minesweeper
//4/20/2018

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
//#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_uart.h>
#include <f3d_user_btn.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_nunchuk.h>
#include <f3d_delay.h>
#include <f3d_rtc.h>
//#include <f3d_systick.h>
#include <f3d_accel.h>
#include <f3d_dac.h>
#include <queue.h>
#include <graphics.h>
#include <ff.h>
#include <diskio.h>
#include <stdio.h>

//macros that help with keeping track of what's happening on the board
#define FLAG_SQUARE(X,Y) (board[Y][X] ^= 0b00100000)    //mark the selected square as flagged/unflagged
#define QUESTION_SQUARE(X,Y) (board[y][x] ^= 0b01000000) //mark the selected square as questioned / unquestioned
#define CHECK_FLAGGED(X,Y) (board[Y][X] & 0b00100000)  //check if selected square is flagged
#define CHECK_QUESTIONED(X,Y) (board[y][x] & 0b01000000) //check if selected square is questioned
#define CLICK_SQUARE(X,Y) (board[Y][X] |= 0b00010000)  //mark the selected square as clicked
#define CHECK_CLICKED(X,Y) (board[Y][X] & 0b00010000) //check if selected square has already been clicked
#define GET_SQUARE_TYPE(X,Y) (board[Y][X] & 0b00001111) //get what type of square it is
#define SET_SQUARE_TYPE(X,Y,TYPE) (board[Y][X] = TYPE)
#define MINE 9
#define EMPTY 0 //tile corresponding to the numbers 1-8 have indices 1-8 respectively
#define NUM_MINES 10
#define BOARD_WIDTH ST7735_width / 16
#define BOARD_HEIGHT ST7735_height / 16

unsigned char board[10][8] = {0}; //top 4 bytes say what's been done to the square (clicked, flagged, questioned), bottom 4 bytes say what type of square it is (empty, number square, mine)
float accel_data[3];		//store the x/y/z of the accelerometer to help generate random numbers
int num_squares;		//how many squares are still unclicked. Used to help find out when/if the game has been won
int game_state = 0;		//normal playing, game over, game won
int cursor_x = 0, cursor_y = 0;

void draw_tile(int x, int y);
void find_mines(int x, int y);
uint16_t random();

int main(void)
{
  int num_clicks = 0;
  nunchuk_t nunchuk;

  //initialize all the drivers
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_uart_init();
  delay(10);
  printf("Ok.\n");
  //f3d_timer2_init();
  //delay(10);
  f3d_i2c1_init();
  delay(10);
  printf("mhm\n");
  //f3d_accel_init();
  delay(10);
  f3d_user_btn_init();
  delay(10);
  f3d_dac_init();
  delay(10);
  f3d_delay_init();
  delay(10);
  printf("Alright.\n");
  f3d_rtc_init();
  delay(10);
  //f3d_systick_init(20000);
  //delay(10);
  printf("Yeah.\n");
  f3d_lcd_init();
  delay(10);
  printf("Waiting for nunchuk...\n");
  f3d_nunchuk_init();
  delay(10);

  printf("Everything working.\n");
  f3d_lcd_fillScreen(RED);

  //populate the screen with clickable squares, and initialize board array
  int i, j;
  for (i = 0; i < BOARD_HEIGHT; i++) {
    for (j = 0; j < BOARD_WIDTH; j++)
      f3d_lcd_drawTile(j * 16, i * 16, tiles, 0);
  }
  num_squares = BOARD_HEIGHT * BOARD_WIDTH;

  //main loop
  while (1) {
    f3d_nunchuk_read(&nunchuk);

    if (nunchuk.jx == 0xFF) {
	//right
      //"undraw" old cursor
      if ((++cursor_x) >= BOARD_WIDTH)
	cursor_x = 0;
      //draw new cursor
      printf("X: %d, Y: %d\n", cursor_x, cursor_y);
    } else if (nunchuk.jx == 0x00) {
	//left
      //"undraw" old cursor
      if ((--cursor_x) < 0)
	cursor_x = BOARD_WIDTH - 1;
      //draw new cursor
      printf("X: %d, Y: %d\n", cursor_x, cursor_y);
    } else if (nunchuk.jy == 0xFF) {
	//down
      //"undraw" old cursor
      if ((++cursor_y) >= BOARD_HEIGHT)
	cursor_y = 0;
      //draw new cursor
      printf("X: %d, Y: %d\n", cursor_x, cursor_y);
    } else if (nunchuk.jy == 0x00) {
	//up
      //"undraw" old cursor
      if ((--cursor_y) < 0)
	  cursor_y = BOARD_HEIGHT - 1;
      //draw new cursor
      printf("X: %d, Y: %d\n", cursor_x, cursor_y);
    } else if (nunchuk.z) {
      if (num_clicks == 0) {
	//set random seed based off first click (The first click can never be a mine
	int mines = 0;
	int x = 0, y = 0;
	while (mines <= NUM_MINES) {
	  //int x = random() % (BOARD_WIDTH);
	  //int y = random() % (BOARD_HEIGHT);
	  if (x != cursor_x && y != cursor_y && GET_SQUARE_TYPE(x, y) == EMPTY) {
	    SET_SQUARE_TYPE(x, y, MINE);
	    mines++;
	  }
	  y++;
	}
	find_mines(cursor_x, cursor_y);
	num_clicks++;
      } else {
	if (CHECK_CLICKED(cursor_x, cursor_y) == 0 && CHECK_FLAGGED(cursor_x, cursor_y) == 0) {
	  if (GET_SQUARE_TYPE(cursor_x, cursor_y) == MINE) {
	    CLICK_SQUARE(cursor_x, cursor_y);
	    //game over
	    //re-draw the board drawing mines, with a check over it if it was flagged, and an X over squares that were flagged but which didn't have mines
	    printf("Game over.\n");
	    game_state = 1;
	    while(1); //hang the program for now
	  } else {
	    //recursively check for adjacent mines, and draw the number
	    find_mines(cursor_x, cursor_y);
	    //check if the board has been cleared and all the mines have been found
	    if (num_squares == NUM_MINES) {
		printf("You won!\n");
		game_state = 2;
		while(1);  //hang the program for now
	    }
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
  /*
  if (CHECK_CLICKED(x, y) == 0) {
    //if it's game over, show the flagged squares as whether or not they had mines
    if (CHECK_FLAGGED(x, y) == 1) {
      if (game_state == 1) {
	//if game over, draw an X over the flag if there was no mine there, or a check over a mine if there was one there
	if (GET_SQUARE_TYPE(x, y) == MINE);
	else;
      } else; //draw normal flag
    } else
      f3d_lcd_drawTile(x, y, tiles, 0);
  } else {
    if (x == cursor_x && y == cursor_y) {
	//draw red around the mine indicating that it went off
    } else
      f3d_lcd_drawTile(x * 16, y * 16, tiles, GET_SQUARE_TYPE(x, y) + 1);
  }
  */

  if (CHECK_CLICKED(x, y) == 0) {}
  else
    f3d_lcd_drawTile(x * 16, y * 16, tiles, GET_SQUARE_TYPE(x, y) + 1);
}

void find_mines(int x, int y) {
  //recursively searches the board for mines, and marks squares with no mines as clicked
  CLICK_SQUARE(x, y);
  num_squares--;
  
  int count = 0;
  //left
  if ((x - 1 >= 0) && (CHECK_CLICKED(x - 1, y) == 0)) {
    if (GET_SQUARE_TYPE(x - 1, y) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x - 1, y) == EMPTY && (CHECK_FLAGGED(x - 1, y)  == 0))
      find_mines(x - 1, y);
  }
  //upper-left
  if ((x - 1 >= 0 && y - 1 >= 0) && (CHECK_CLICKED(x - 1, y - 1) == 0)) {
    if (GET_SQUARE_TYPE(x - 1, y - 1) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x - 1, y - 1) == EMPTY && (CHECK_FLAGGED(x - 1, y - 1) == 0))
      find_mines(x - 1, y - 1);
  }
  //up
  if ((y - 1 >= 0) && (CHECK_CLICKED(x, y - 1) == 0)) {
    if (GET_SQUARE_TYPE(x, y - 1) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x, y - 1) == EMPTY && (CHECK_FLAGGED(x, y - 1) == 0))
      find_mines(x, y - 1);
  }
  //upper-right
  if ((x + 1 < (ST7735_width / 16) && y - 1 >= 0) && (CHECK_CLICKED(x + 1, y - 1) == 0)) {
    if (GET_SQUARE_TYPE(x + 1, y - 1) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x + 1, y - 1) == EMPTY && (CHECK_FLAGGED(x + 1, y - 1) == 0))
      find_mines(x + 1, y - 1);
  }
  //right
  if ((x + 1 < (ST7735_width / 16)) && (CHECK_CLICKED(x + 1, y) == 0)) {
    if (GET_SQUARE_TYPE(x + 1, y) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x + 1, y) == EMPTY && (CHECK_FLAGGED(x + 1, y) == 0))
      find_mines(x + 1, y);
  }
  //bottom-right
  if ((x + 1 < (ST7735_width / 16) && y + 1 < (ST7735_height / 16)) && (CHECK_CLICKED(x + 1, y + 1) == 0)) {
    if (GET_SQUARE_TYPE(x + 1, y + 1) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x + 1, y + 1) == EMPTY && (CHECK_FLAGGED(x + 1, y + 1) == 0))
      find_mines(x + 1, y + 1);
  }
  //bottom
  if ((y + 1 < (ST7735_height / 16)) && (CHECK_CLICKED(x, y + 1) == 0)) {
    if (GET_SQUARE_TYPE(x, y + 1) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x, y + 1) == EMPTY && (CHECK_FLAGGED(x, y + 1) == 0))
      find_mines(x, y + 1);
  }
  //bottom-left
  if ((x >= 0 && y < (ST7735_height / 16)) && (CHECK_CLICKED(x - 1, y + 1) == 0)) {
    if (GET_SQUARE_TYPE(x - 1, y + 1) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x - 1, y + 1) == EMPTY && (CHECK_FLAGGED(x - 1, y + 1) == 0))
      find_mines(x - 1, y + 1);
  }

  //set square's type to the # of surrounding mines
  SET_SQUARE_TYPE(x, y, count);
  CLICK_SQUARE(x, y);
  draw_tile(x, y);
  printf("Bla\n");
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
