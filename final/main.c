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
#include <f3d_mag.h>
#include <f3d_dac.h>
#include <queue.h>
#include <graphics.h>
#include <ff.h>
#include <diskio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//macros that help with keeping track of what's happening on the board
#define FLAG_SQUARE(X,Y) (board[Y][X] ^= 0b00100000)    //mark the selected square as flagged/unflagged
#define CHECK_FLAGGED(X,Y) (board[Y][X] & 0b00100000)  //check if selected square is flagged
#define CLICK_SQUARE(X,Y) (board[Y][X] |= 0b00010000)  //mark the selected square as clicked
#define CHECK_CLICKED(X,Y) (board[Y][X] & 0b00010000) //check if selected square has already been clicked
#define GET_SQUARE_TYPE(X,Y) (board[Y][X] & 0b00001111) //get what type of square it is
#define SET_SQUARE_TYPE(X,Y,TYPE) (board[Y][X] = TYPE)
#define FLAG_TILE 10
#define MINE_TILE 11
#define CLICKED_MINE_TILE 12
#define FALSE_MINE_TILE 13
#define EMPTY 0 //tile corresponding to the numbers 1-8 have indices 1-8 respectively
#define MINE 9
#define NUM_MINES 12
#define BOARD_WIDTH ST7735_width / 16
#define BOARD_HEIGHT ST7735_height / 16
#define TIMER 20000
#define AUDIOBUFSIZE 128

extern uint8_t Audiobuf[AUDIOBUFSIZE];
extern int audioplayerHalf;
extern int audioplayerWhole;

const char* wavs[5] = {"thermo.wav", "sound1_8.wav", "sound2_8.wav", "sound3_8.wav", "sound4_8.wav"}; //wav file names

FATFS Fatfs;		/* File system object */
FIL fid;		/* File object */
BYTE Buff[512];		/* File read buffer */
int ret;

struct ckhd {
  uint32_t ckID;
  uint32_t cksize;
};

struct fmtck {
  uint16_t wFormatTag;      
  uint16_t nChannels;
  uint32_t nSamplesPerSec;
  uint32_t nAvgBytesPerSec;
  uint16_t nBlockAlign;
  uint16_t wBitsPerSample;
};

unsigned char board[10][8];     //top 4 bytes say what's been done to the square (clicked, flagged, questioned), bottom 4 bytes say what type of square it is (empty, number square, mine)
float accel_data[3];		//store the x/y/z of the accelerometer to help generate random numbers
nunchuk_t nunchuk;              //nunchuk data structure
int num_squares;		//how many squares are still unclicked. Used to help find out when/if the game has been won
int game_state = 0;		//normal playing, game over, game won
int cursor_x = 0, cursor_y = 0;
int num_clicks = 0;             //used to determine the first click, to generate the mines

void draw_tile(int x, int y);  //high-level logic to draw the right tile based on what's going on in the game
void draw_cursor();
void draw_board();
void find_mines(int x, int y);
void reset();
void play();
void wait_for_reset();
uint16_t random();
void play_explosion();

//SD card driver functions
void readckhd(FIL *fid, struct ckhd *hd, uint32_t ckID) {
  f_read(fid, hd, sizeof(struct ckhd), &ret);
  if (ret != sizeof(struct ckhd))
    exit(-1);
  if (ckID && (ckID != hd->ckID))
    exit(-1);
}

void die (FRESULT rc) {
  printf("Failed with rc=%u.\n", rc);
  while (1);
}

int main(void)
{
  //initialize all the drivers
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_uart_init();
  delay(10);
  f3d_timer2_init();
  delay(10);
  f3d_user_btn_init();
  delay(10);
  f3d_i2c1_init();
  delay(10);
  printf("Waiting for Accelerometer\n");
  f3d_accel_init();
  printf("Accelerometer initialized.\n");
  delay(10);
  f3d_dac_init();
  delay(10);
  f3d_delay_init();
  delay(10);
  printf("Ok.\n");
  f3d_rtc_init();
  delay(10);
  f3d_systick_init(20000);
  delay(10);
  f3d_lcd_init();
  delay(10);
  printf("Waiting for nunchuk...\n");
  f3d_nunchuk_init();
  delay(10);

  printf("Everything working.\n");
  f3d_lcd_fillScreen(RED);

  //populate the screen with clickable squares, and initialize board array
  reset();
  draw_cursor();

  //main loop
  while (1) {
    switch(game_state) {
    case 0:
      play();
      break;
    case 1:
    case 2:
      wait_for_reset();
      break;
    }
  }

  return 0;
}

void play() {
  f3d_nunchuk_read(&nunchuk);
  printf("%d %d %d %d %d %d %d\n", nunchuk.jx, nunchuk.jy, nunchuk.ax, nunchuk.ay, nunchuk.az, nunchuk.z, nunchuk.c);

  if (nunchuk.jx >= 0xC0) {
    //right
    draw_tile(cursor_x, cursor_y);  //un-draw old cursor
    if ((++cursor_x) >= BOARD_WIDTH)
      cursor_x = 0;
    draw_cursor();    //draw new cursor
    printf("X: %d, Y: %d\n", cursor_x, cursor_y);
  } else if (nunchuk.jx <= 0x40) {
    //left
    draw_tile(cursor_x, cursor_y);
    if ((--cursor_x) < 0)
      cursor_x = BOARD_WIDTH - 1;
    draw_cursor();
    printf("X: %d, Y: %d\n", cursor_x, cursor_y);
  } else if (nunchuk.jy <= 0x40) {
    //down
    draw_tile(cursor_x, cursor_y);
    if ((++cursor_y) >= BOARD_HEIGHT)
      cursor_y = 0;
    draw_cursor();
    printf("X: %d, Y: %d\n", cursor_x, cursor_y);
  } else if (nunchuk.jy >= 0xC0) {
    //up
    draw_tile(cursor_x, cursor_y);
    if ((--cursor_y) < 0)
      cursor_y = BOARD_HEIGHT - 1;
    draw_cursor();
    printf("X: %d, Y: %d\n", cursor_x, cursor_y);
  } else if (nunchuk.z) {
    if (num_clicks == 0) {
      //set random seed based off first click (The first click can never be a mine
      int mines = 0;
      int x, y;
      while (mines <= NUM_MINES) {
	int x = random() % (BOARD_WIDTH);
	int y = random() % (BOARD_HEIGHT);
	if (x != cursor_x && y != cursor_y && GET_SQUARE_TYPE(x, y) == EMPTY) {
	  SET_SQUARE_TYPE(x, y, MINE);
	  mines++;
	}
	y++;
      }
      find_mines(cursor_x, cursor_y);
      num_clicks++;
      draw_cursor();
    } else {
      if (CHECK_CLICKED(cursor_x, cursor_y) == 0 && CHECK_FLAGGED(cursor_x, cursor_y) == 0) {
	if (GET_SQUARE_TYPE(cursor_x, cursor_y) == MINE) {
	  CLICK_SQUARE(cursor_x, cursor_y);
	  //game over
	  game_state = 1;
	  //re-draw the board drawing mines, with a check over it if it was flagged, and an X over squares that were flagged but which didn't have mines
	  draw_board();
	  f3d_lcd_drawString((ST7735_width / 2) - 20, ST7735_height / 2, "Game Over.", RED, BLACK);
	  play_explosion();
	} else {
	  //recursively check for adjacent mines, and draw the number
	  find_mines(cursor_x, cursor_y);
	  //check if the board has been cleared and all the mines have been found
	  if (num_squares == NUM_MINES) {
	    game_state = 2;
	    f3d_lcd_drawString((ST7735_width / 2) - 20, ST7735_height / 2, "You Win!", BLUE, BLACK);
	  }
	}
	draw_cursor();
      }
      //debug feature
      //f3d_lcd_drawChar(0,0,(char)num_squares + 48, BLUE, BLACK);
    }
  } else if (nunchuk.c) {
    if (CHECK_CLICKED(cursor_x, cursor_y) == 0) {
      FLAG_SQUARE(cursor_x, cursor_y);
      draw_tile(cursor_x, cursor_y);
      draw_cursor();
      delay(200);
    }
  }

  delay(100);
}

void draw_tile(int x, int y) {
  if (CHECK_CLICKED(x, y) == 0) {
    //if it's game over, show the flagged squares as whether or not they had mines
    if (CHECK_FLAGGED(x, y) != 0) {
      if (game_state == 1) {
	//if game over, draw an X over the flag if there was no mine there, or a flag over a mine if there was one there
	if (GET_SQUARE_TYPE(x, y) == MINE)
	  f3d_lcd_drawTile(x * 16, y * 16, tiles, FLAG_TILE);
	else
	  f3d_lcd_drawTile(x * 16, y * 16, tiles, FALSE_MINE_TILE);
      } else //draw normal flag
	f3d_lcd_drawTile(x * 16, y * 16, tiles, FLAG_TILE);
    } else {
      if (game_state == 1 && GET_SQUARE_TYPE(x, y) == MINE)
	f3d_lcd_drawTile(x * 16, y * 16, tiles, MINE_TILE);
      else
	f3d_lcd_drawTile(x * 16, y * 16, tiles, 0);
    }
  } else {
    if (GET_SQUARE_TYPE(x, y) == MINE && x == cursor_x && y == cursor_y) {
      //draw red around the mine indicating that it went off
      f3d_lcd_drawTile(x * 16, y * 16, tiles, CLICKED_MINE_TILE);
    }
    else
      f3d_lcd_drawTile(x * 16, y * 16, tiles, GET_SQUARE_TYPE(x, y) + 1);
  }
}

void wait_for_reset() {
  //wait for player to press Z to play the game again.
  f3d_nunchuk_read(&nunchuk);
  printf("%d %d %d %d %d %d %d\n", nunchuk.jx, nunchuk.jy, nunchuk.ax, nunchuk.ay, nunchuk.az, nunchuk.z, nunchuk.c);
  if (nunchuk.z) {
    reset();
    game_state = 0;
  }
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
    else if (GET_SQUARE_TYPE(x - 1, y - 1) == EMPTY && (CHECK_FLAGGED(x - 1, y - 1) == 0) && count == 0)
      find_mines(x - 1, y - 1);
  }
  //up
  if ((y - 1 >= 0) && (CHECK_CLICKED(x, y - 1) == 0)) {
    if (GET_SQUARE_TYPE(x, y - 1) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x, y - 1) == EMPTY && (CHECK_FLAGGED(x, y - 1) == 0) && count == 0)
      find_mines(x, y - 1);
  }
  //upper-right
  if ((x + 1 < (ST7735_width / 16) && y - 1 >= 0) && (CHECK_CLICKED(x + 1, y - 1) == 0)) {
    if (GET_SQUARE_TYPE(x + 1, y - 1) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x + 1, y - 1) == EMPTY && (CHECK_FLAGGED(x + 1, y - 1) == 0) && count == 0)
      find_mines(x + 1, y - 1);
  }
  //right
  if ((x + 1 < (ST7735_width / 16)) && (CHECK_CLICKED(x + 1, y) == 0)) {
    if (GET_SQUARE_TYPE(x + 1, y) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x + 1, y) == EMPTY && (CHECK_FLAGGED(x + 1, y) == 0) && count == 0)
      find_mines(x + 1, y);
  }
  //bottom-right
  if ((x + 1 < (ST7735_width / 16) && y + 1 < (ST7735_height / 16)) && (CHECK_CLICKED(x + 1, y + 1) == 0)) {
    if (GET_SQUARE_TYPE(x + 1, y + 1) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x + 1, y + 1) == EMPTY && (CHECK_FLAGGED(x + 1, y + 1) == 0) && count == 0)
      find_mines(x + 1, y + 1);
  }
  //bottom
  if ((y + 1 < (ST7735_height / 16)) && (CHECK_CLICKED(x, y + 1) == 0)) {
    if (GET_SQUARE_TYPE(x, y + 1) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x, y + 1) == EMPTY && (CHECK_FLAGGED(x, y + 1) == 0) && count == 0)
      find_mines(x, y + 1);
  }
  //bottom-left
  if ((x >= 0 && y < (ST7735_height / 16)) && (CHECK_CLICKED(x - 1, y + 1) == 0)) {
    if (GET_SQUARE_TYPE(x - 1, y + 1) == MINE)
      count++;
    else if (GET_SQUARE_TYPE(x - 1, y + 1) == EMPTY && (CHECK_FLAGGED(x - 1, y + 1) == 0) && count == 0)
      find_mines(x - 1, y + 1);
  }

  //set square's type to the # of surrounding mines
  SET_SQUARE_TYPE(x, y, count);
  CLICK_SQUARE(x, y);
  draw_tile(x, y);
  printf("Bla\n");
}

void draw_cursor() {
  //a 2x2 pixel yellow dot
  f3d_lcd_drawPixel((cursor_x * 16) + 7, (cursor_y * 16) + 7, YELLOW);
  f3d_lcd_drawPixel((cursor_x * 16) + 8, (cursor_y * 16) + 7, YELLOW);
  f3d_lcd_drawPixel((cursor_x * 16) + 7, (cursor_y * 16) + 8, YELLOW);
  f3d_lcd_drawPixel((cursor_x * 16) + 8, (cursor_y * 16) + 8, YELLOW);
}

void draw_board() {
  int i, j;
  for (i = 0; i < BOARD_HEIGHT; i++) {
    for (j = 0; j < BOARD_WIDTH; j++)
      draw_tile(j, i);
  }
}

void reset() {
  //re-initialize board
  int i, j;
  for (i = 0; i < BOARD_HEIGHT; i++) {
    for (j = 0; j < BOARD_WIDTH; j++)
      board[i][j] = 0;
  }

  cursor_x = 0;
  cursor_y = 0;
  num_clicks = 0;
  num_squares = BOARD_HEIGHT * BOARD_WIDTH;
  draw_board();
}

uint16_t random() {
  //a work in progress
  uint16_t seed = *((uint16_t *)(&accel_data[0])) + *((uint16_t *)(&accel_data[1])) + *((uint16_t *)(&accel_data[2]));
  //printf("%d\n", seed);
  f3d_accel_read(accel_data);
  return seed + *((uint16_t *)(&accel_data[0])) + *((uint16_t *)(&accel_data[1])) + *((uint16_t *)(&accel_data[2]));
}

void play_explosion() {
 FRESULT rc;			/* Result code */
  DIR dir;			/* Directory object */
  FILINFO fno;			/* File information object */
  UINT bw, br;
  unsigned int retval;
  int bytesread;

  f_mount(0, &Fatfs);/* Register volume work area */
  printf("\nOpen %s\n", "xplsn.wav");
  rc = f_open(&fid, "xplsn.wav", FA_READ);
  
  if (rc) die(rc);
  else {
    printf("Okay.\n");

    struct ckhd hd;
    uint32_t  waveid;
    struct fmtck fck;
    
    readckhd(&fid, &hd, 'FFIR');
    
    f_read(&fid, &waveid, sizeof(waveid), &ret);
    if ((ret != sizeof(waveid)) || (waveid != 'EVAW'))
      return;
    
    readckhd(&fid, &hd, ' tmf');
    
    f_read(&fid, &fck, sizeof(fck), &ret);
    
    // skip over extra info
    
    if (hd.cksize != 16) {
      printf("extra header info %d\n", hd.cksize - 16);
      f_lseek(&fid, hd.cksize - 16);
    }
    
    printf("audio format 0x%x\n", fck.wFormatTag);
    printf("channels %d\n", fck.nChannels);
    printf("sample rate %d\n", fck.nSamplesPerSec);
    printf("data rate %d\n", fck.nAvgBytesPerSec);
    printf("block alignment %d\n", fck.nBlockAlign);
    printf("bits per sample %d\n", fck.wBitsPerSample);
    
    // now skip all non-data chunks !
    
    while(1) {
      readckhd(&fid, &hd, 0);
      if (hd.ckID == 'atad')
	break;
      f_lseek(&fid, hd.cksize);
    }
    
    printf("Samples %d\n", hd.cksize);
    
    // Play it !
    // audioplayerInit(fck.nSamplesPerSec);
    f3d_timer2_enable(fck.nSamplesPerSec);
    
    f_read(&fid, Audiobuf, AUDIOBUFSIZE, &ret);
    hd.cksize -= ret;
    audioplayerStart();
    while (hd.cksize) {
      int next = hd.cksize > AUDIOBUFSIZE/2 ? AUDIOBUFSIZE/2 : hd.cksize;
      if (audioplayerHalf) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(Audiobuf, AUDIOBUFSIZE/2);
	f_read(&fid, Audiobuf, next, &ret);
	hd.cksize -= ret;
	audioplayerHalf = 0;
      }
      if (audioplayerWhole) {
	if (next < AUDIOBUFSIZE/2)
	  bzero(&Audiobuf[AUDIOBUFSIZE/2], AUDIOBUFSIZE/2);
	f_read(&fid, &Audiobuf[AUDIOBUFSIZE/2], next, &ret);
	hd.cksize -= ret;
	audioplayerWhole = 0;
      }
    }
    audioplayerStop();
  }
  
  printf("\nClose the file.\n"); 
  rc = f_close(&fid);
  
  //if (rc) die(rc);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  /* Infinite loop */
  /* Use GDB to find out why we're here */
  while (1);
}
#endif
