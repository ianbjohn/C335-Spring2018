//Ian Johnson
//C335
//HW3 - Disassembler
//3/7/18


#include <stdio.h>
#include <stdint.h>


#define Rm(x, y, z) ((x & y) >> z) //used to help get rid of the clutter of all the bitwise operations


const char* opcodes_dp[] = {"ands", "eors", "lsls", "asrs", "adcs", "sbcs", "rors", "tst", "rsbs", "cmp", "cmn", "orrs", "muls", "bics", "mvns"}; //LUT for data-processing opcode strings
const char* opcodes_ascmi[] = {"mov", "cmp", "add", "sub"}; //LUT for add/subtract/compare/move (immediate) opcode strings
const char* opcodes_as[] = {"add", "sub"}; //LUT for add/subtract (same for immediate and register) opcode strings

const char* registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13", "sp", "lr", "pc"};


void interpret(uint16_t word, int mode);

int main()
{
  uint16_t word;

  printf(".text\n.syntax unified\n.thumb\n");
  
  while (scanf("%x", &word) == 1) {
    //interpret the word properly
    	if ((word & 0b1111110000000000) == 0x4000)
      		interpret(word, 0); //data-processing register
    	else if ((word & 0b1110000000000000) == 0x2000)
      		interpret(word, 1); //add/sub/cmp/mov immediate
      	else if ((word & 0b1111110000000000) == 0x1C00)
		interpret(word, 2); //add/subtract immediate
      	else if ((word & 0b1111110000000000) == 0x1800)
		interpret(word, 3); //add/subtract register
	else
		printf("\t.hword 0x%x\n", word); //unknown
	}
}

void interpret(uint16_t word, int mode)
{
  switch (mode) {
  case 0:
    //data processing register
    printf("\t%s %s, %s\n", opcodes_dp[Rm(word, 0b0000001111000000, 6)], registers[Rm(word, 0b0000000000000111, 0)], registers[Rm(word, 0b0000000000111000, 3)]);
    break;
  case 1:
	//add/subtract/compare/move immediate
	printf("\t%s %s, %d\n", opcodes_ascmi[Rm(word, 0b0001100000000000, 11)], registers[Rm(word, 0b0000011100000000, 8)], Rm(word, 0b0000000011111111, 0));
	break;
case 2:
	//add/subtract immediate
	printf("\t%s %s, %s, %d\n", opcodes_as[Rm(word, 0b0000001000000000, 9)], registers[Rm(word, 0b0000000000000111, 0)], registers[Rm(word, 0b0000000000111000, 3)], Rm(word, 0b0000000111000000, 6));
	break;
case 3:
	//add/subtract register
	printf("\t%s %s, %s, %s\n", opcodes_as[Rm(word, 0b0000001000000000, 9)], registers[Rm(word, 0b0000000000000111, 0)], registers[Rm(word, 0b000000000111000, 3)], registers[Rm(word, 0b0000000111000000, 6)]);
	break;
  }
}
