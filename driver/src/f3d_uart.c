/*
FILENAME: f3d_uart.c
USER: Ian Johnson (ianbjohn)
PARTNER: Kelly Wheeler (kellwhee)
ASSIGNMENT: lab5
DATE CREATED: 2/9/2018
 */
//the uart.c file
#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>
#include <f3d_led.h>


//intializes the port and pins for the leds on the board
void f3d_uart_init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOC, 4, GPIO_AF_7);
  GPIO_PinAFConfig(GPIOC, 5, GPIO_AF_7);

  USART_InitTypeDef USART_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
  USART_Cmd(USART1, ENABLE);
}
//prints the char given in the serialT command
int putchar(int c){
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == (uint16_t)RESET);
  USART_SendData(USART1, c);
  return 0;
}

//reads the char provided in serialT command
int getchar(void){
  int c = 0;
  if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != (uint16_t)RESET){
  c = USART_ReceiveData(USART1);
  }
  return c;
}

//prints the string given in the serialT command
void putstring(char * ptr){
  int i = 0;
  while(ptr[i] != '\0'){
    putchar(ptr[i]);
    i++;
  }
}