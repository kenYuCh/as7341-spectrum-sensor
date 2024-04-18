#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#include "i2c_master.h"

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

void i2c_master_init();
void i2c_master_deinit();

void I2C_Start();
void I2C_RepeatedStart();
void I2C_Stop();

uint8_t I2C_Write(char data);
char I2C_Read(bool a);

void i2c_master_write(uint8_t slaveAddress, uint8_t *data, uint8_t length);

void i2c_master_interrupt();

#endif