#include <stdint.h>
#include <graphics.h>

//unclicked square, empty square, numbers 1-9, mine square, flagged square, false flag square
uint16_t tiles[10][16][16] = {{
 {0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB}, //blank tile
 {0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xB5B6},
 {0xDEDB,0xDEDB,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6,0xB5B6},
 {0xDEDB,0xDEDB,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6,0xB5B6},
 {0xDEDB,0xDEDB,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6,0xB5B6},
 {0xDEDB,0xDEDB,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6,0xB5B6},
 {0xDEDB,0xDEDB,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6,0xB5B6},
 {0xDEDB,0xDEDB,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6,0xB5B6},
 {0xDEDB,0xDEDB,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6,0xB5B6},
 {0xDEDB,0xDEDB,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6,0xB5B6},
 {0xDEDB,0xDEDB,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6,0xB5B6},
 {0xDEDB,0xDEDB,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6,0xB5B6},
 {0xDEDB,0xDEDB,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6,0xB5B6},
 {0xDEDB,0xDEDB,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6,0xB5B6},
 {0xDEDB,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6},
 {0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6}},
 {{0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6}, //clicked tile
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6}},
 {{0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6}, //clicked tile 1
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x007F,0x007F,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x007F,0x007F,0x007F,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x007F,0x007F,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x007F,0x007F,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x007F,0x007F,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x007F,0x007F,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0x007F,0x007F,0x007F,0x007F,0x007F,0x007F,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0x007F,0x007F,0x007F,0x007F,0x007F,0x007F,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6}},
 {{0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6}, //clicked tile 2
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x0308,0x0308,0x0308,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x0308,0xCE79,0xCE79,0x0308,0x0308,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x0308,0x0308,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x0308,0x0308,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x0308,0x0308,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x0308,0x0308,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x0308,0x0308,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x0308,0x0308,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0x0308,0x0308,0x0308,0x0308,0x0308,0x0308,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0x0308,0x0308,0x0308,0x0308,0x0308,0x0308,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6}},
 {{0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6},  //clicked tile 3
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xF800,0xF800,0xF800,0xF800,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xF800,0xF800,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xF800,0xF800,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xF800,0xF800,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xF800,0xD800,0xF800,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xF800,0xF800,0xF800,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xF800,0xF800,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xF800,0xF800,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xF800,0xF800,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xF800,0xF800,0xF800,0xF800,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6}},
 {{0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6}, //clicked tile 4
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x002E,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x002E,0x002E,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x002E,0x002E,0x002E,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x002E,0x002E,0xCE79,0x002E,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0x002E,0x002E,0xCE79,0xCE79,0x002E,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0x002E,0x002E,0xCE79,0xCE79,0xCE79,0x002E,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0x002E,0x002E,0x002E,0x002E,0x002E,0x002E,0x002E,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x002E,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x002E,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x002E,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6}},
 {{0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6}, //clicked tile 5
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0x7800,0x7800,0x7800,0x7800,0x7800,0x7800,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0x7800,0x7800,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0x7800,0x7800,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0x7800,0x7800,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0x7800,0x7800,0x7800,0x7800,0x7800,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x7800,0x7800,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x7800,0x7800,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x7800,0x7800,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0x7800,0x7800,0xCE79,0xCE79,0x7800,0x7800,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x7800,0x7800,0x7800,0x7800,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6}},
 {{0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6}, //clicked tile 6
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x03CF,0x03CF,0x03CF,0x03CF,0x03CF,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0x03CF,0x03CF,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0x03CF,0x03CF,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0x03CF,0x03CF,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0x03CF,0x0C3F,0x03CF,0x03CF,0x03CF,0x03CF,0x03CF,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0x03CF,0x03CF,0xCE79,0xCE79,0xCE79,0xCE79,0x03CF,0x03CF,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0x03CF,0x03CF,0xCE79,0xCE79,0xCE79,0xCE79,0x03CF,0x03CF,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0x03CF,0x03CF,0xCE79,0xCE79,0xCE79,0xCE79,0x03CF,0x03CF,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0x03CF,0x03CF,0xCE79,0xCE79,0xCE79,0xCE79,0x03CF,0x03CF,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0x0C3F,0x03CF,0x03CF,0x03CF,0x03CF,0x03CF,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6}},
 {{0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6}, //clicked tile 7
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0x31A6,0x31A6,0x31A6,0x31A6,0x31A6,0x31A6,0x31A6,0x31A6,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0x31A6,0x31A6,0x31A6,0x31A6,0x31A6,0x31A6,0x31A6,0x31A6,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x31A6,0x31A6,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x31A6,0x31A6,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x31A6,0x31A6,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x31A6,0x31A6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x31A6,0x31A6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x31A6,0x31A6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x31A6,0x31A6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0x31A6,0x31A6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6}},
 {{0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6}, //clicked tile 8
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0x94B2,0x94B2,0x94B2,0x94B2,0x94B2,0x94B2,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0x94B2,0x94B2,0xCE79,0xCE79,0xCE79,0xCE79,0x94B2,0x94B2,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0x94B2,0x94B2,0xCE79,0xCE79,0xCE79,0xCE79,0x94B2,0x94B2,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0x94B2,0x94B2,0xCE79,0xCE79,0xCE79,0xCE79,0x94B2,0x94B2,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0x94B2,0x94B2,0x94B2,0x94B2,0x94B2,0x94B2,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0x94B2,0x94B2,0x94B2,0x94B2,0x94B2,0x94B2,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0x94B2,0x94B2,0xCE79,0xCE79,0xCE79,0xCE79,0x94B2,0x94B2,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0x94B2,0x94B2,0xCE79,0xCE79,0xCE79,0xCE79,0x94B2,0x94B2,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0x94B2,0x94B2,0xCE79,0xCE79,0xCE79,0xCE79,0x94B2,0x94B2,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0x94B2,0x94B2,0x94B2,0x94B2,0x94B2,0x94B2,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xCE79,0xB5B6},
 {0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xB5B6}}
};
