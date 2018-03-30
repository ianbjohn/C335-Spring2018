/* f3d_systick.c --- 
 * 
 * Filename: f3d_systick.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Nov 14 07:57:37 2013
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

/* Copyright (c) 2004-2007 The Trustees of Indiana University and 
 * Indiana University Research and Technology Corporation.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */

#include <f3d_systick.h>
#include <f3d_led.h> 
#include <f3d_user_btn.h>
#include <f3d_uart.h>

volatile int systick_flag = 0;
int counter = 0;

void f3d_systick_init(int time) {
  SysTick_Config(SystemCoreClock / time);
}

void SysTick_Handler(void) {
  if (f3d_user_btn_read())
    SysTick_Config(SystemCoreClock / 10); //slow down when button is pressed
  else
    SysTick_Config(SystemCoreClock / 100);

  if (++counter > 7) 
    counter = 0;
  f3d_led_all_off();
  f3d_led_on(counter);
}

/* f3d_systick.c ends here */
