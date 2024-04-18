#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

#include <stdint.h>

void i2c_slave_init();

uint8_t i2c_slave_get_register(uint8_t registerAddress);
void i2c_slave_set_register(uint8_t registerAddress, uint8_t value);

void i2c_slave_interrupt();


void i2c_slave_set_register_with_lock(uint8_t registerAddress, uint8_t value);
uint8_t i2c_slave_get_register_with_lock(uint8_t registerAddress);


void dump_memory_map();

#endif