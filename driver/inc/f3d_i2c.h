#include <stm32f30x.h>

void f3d_i2c1_init();
void f3d_i2c1_read(uint8_t device, uint8_t reg, uint8_t* buffer, uint16_t numbytes);
void f3d_i2c1_write(uint8_t device, uint8_t reg, uint8_t* value);
void f3d_i2c1_read_nunchuk (uint8_t device, uint8_t* buffer, uint16_t numbytes);
