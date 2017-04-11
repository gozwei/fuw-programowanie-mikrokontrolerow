#ifndef __I2C_H
#define __I2C_H

#include <inttypes.h>

#define ACK 1
#define NACK 0

void i2c_init();
void i2c_start();
void i2c_stop();
void i2c_write( uint8_t data );
uint8_t i2c_read( uint8_t ack );

#endif // __I2C_H
