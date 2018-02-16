/*
FILENAME: main.c
USER: Ian Johnson (ianbjohn)
PARTNER: Kelley Wheeler (kellwhee)
ASSIGNMENT: lab5
DATE CREATED: 2/9/2018
 */
//main.c for lab6
#include <f3d_uart.h>
#include <stdio.h>
#include <f3d_gyro.h>
#include <f3d_led.h>
#include <f3d_user_btn.h>

void measurementAxis(int axis, float* tester);

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void) {
  f3d_uart_init();
  f3d_user_btn_init();
  f3d_led_init();
  
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  f3d_led_all_off();
  while (1) {
    f3d_led_on(0);
    printf("Hello\n");
    printf(",\n");
    printf("world\n");
    printf("!\n");
    f3d_led_off(0);
    printf("Hello\n");
    printf(",\n");
    printf("world\n");
    printf("!\n");
  }
}

void measurementAxis(int axis, float* tester) {
  f3d_gyro_getdata(tester);
	if (axis == 0)
		printf("X: %f\n", tester[0]);
	else if (axis == 1)
		printf("Y: %f\n", tester[1]);
	else
		printf("Z: %f\n", tester[2]);

	f3d_led_all_off();
      if (tester[axis] > 180.0) {
		f3d_led_on(0);
		f3d_led_on(1);
		f3d_led_on(2);
		f3d_led_on(3);
		f3d_led_on(4);
	} else if (tester[axis] > 135.0) {
		f3d_led_on(0);
		f3d_led_on(1);
		f3d_led_on(2);
		f3d_led_on(3);
	} else if (tester[axis] > 90.0) {
		f3d_led_on(0);
		f3d_led_on(1);
		f3d_led_on(2);
	} else if (tester[axis] > 45.0) {
		f3d_led_on(0);
		f3d_led_on(1);
	} else if (tester[axis] > 5.0) {
		f3d_led_on(0);
	} else if (tester[axis] < -180.0) {
		f3d_led_on(0);
		f3d_led_on(7);
		f3d_led_on(6);
		f3d_led_on(5);
		f3d_led_on(4);
	} else if (tester[axis] < -135.0) {
		f3d_led_on(0);
		f3d_led_on(7);
		f3d_led_on(6);
		f3d_led_on(5);
	} else if (tester[axis] < -90.0) {
		f3d_led_on(0);
		f3d_led_on(7);
		f3d_led_on(6);
	} else if (tester[axis] < -45.0) {
		f3d_led_on(0);
		f3d_led_on(7);
	} else if (tester[axis] < -5.0) {
		f3d_led_on(0);
	}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif
