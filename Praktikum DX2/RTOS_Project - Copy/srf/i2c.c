#include <util/twi.h>

void i2c_transmit(char address, char reg, char data)
{
	TWCR = 0xA4;                                                  // send a start bit on i2c bus
	while(!(TWCR & 0x80));                                        // wait for confirmation of transmit 
	TWDR = address;                                               // load address of i2c device
	TWCR = 0x84;                                                  // transmit
	while(!(TWCR & 0x80));                                        // wait for confirmation of transmit
	TWDR = reg;
	TWCR = 0x84;                                                  // transmit
	while(!(TWCR & 0x80));                                        // wait for confirmation of transmit
	TWDR = data;
	TWCR = 0x84;                                                  // transmit
	while(!(TWCR & 0x80));                                        // wait for confirmation of transmit
	TWCR = 0x94;                                                  // stop bit
}

unsigned char i2cRead(char address, char reg)
{
   char read_data = 0;

   TWCR = 0xA4;                                                  // send a start bit on i2c bus
   while(!(TWCR & 0x80));                                        // wait for confirmation of transmit  
   TWDR = address;                                               // load address of i2c device
   TWCR = 0x84;                                                  // transmit 
   while(!(TWCR & 0x80));                                        // wait for confirmation of transmit
   TWDR = reg;                                                   // send register number to read from
   TWCR = 0x84;                                                  // transmit
   while(!(TWCR & 0x80));                                        // wait for confirmation of transmit

   TWCR = 0xA4;                                                  // send repeated start bit
   while(!(TWCR & 0x80));                                        // wait for confirmation of transmit 
   TWDR = address+1;                                             // transmit address of i2c device with readbit set
   TWCR = 0xC4;                                                  // clear transmit interupt flag
   while(!(TWCR & 0x80));                                        // wait for confirmation of transmit
   TWCR = 0x84;                                                  // transmit, nack (last byte request)
   while(!(TWCR & 0x80));                                        // wait for confirmation of transmit 
   read_data = TWDR;                                             // and grab the target data
   TWCR = 0x94;                                                  // send a stop bit on i2c bus
   return read_data;
}
