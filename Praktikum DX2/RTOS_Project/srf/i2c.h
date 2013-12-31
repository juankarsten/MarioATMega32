#ifndef __i2c_h__
#define __i2c_h__

void i2c_transmit(char address, char reg, char data);

unsigned char i2cRead(char address, char reg);

#endif
