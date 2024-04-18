#include "i2c_master.h"

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void i2c_master_init() {
    ANSELCbits.ANSC4 = 0;
    ANSELCbits.ANSC5 = 0;
    TRISCbits.TRISC4 = 1;
    TRISCbits.TRISC5 = 1;

    SSP2CON1 = 0x28;
    SSP2CON2 = 0x00;
    SSP2STAT = 0x00;
    SSP2ADD = 0x09;
    
}

void i2c_master_deinit() {
    SSP2CON1 = 0x00;
    SSP2CON2 = 0x00;
    SSP2STAT = 0x00;
    SSP2ADD = 0x00;
}

void I2C_Wait() {
    while ((SSP2STAT & 0x04) || (SSP2CON2 & 0x1F));
}

void I2C_Start() {

    SSP2CON2bits.SEN = 1;
    while (SSP2CON2bits.SEN);

    PIR3bits.SSP2IF = 0;
}

void I2C_RepeatedStart() {

    I2C_Wait();

    SSP2CON2bits.RSEN = 1;
    while (SSP2CON2bits.RSEN);  //Initiate repeated start condition

    PIR3bits.SSP2IF = 0;
}

void I2C_Stop() {

    I2C_Wait();

    SSP2CON2bits.PEN = 1;
    while (SSP2CON2bits.PEN);

    PIR3bits.SSP2IF = 0;
}

uint8_t I2C_Write(char data) {

    // Load data into SSPxBUFF
    SSP2BUF = data;

    while (!(PIR3bits.SSP2IF));
    PIR3bits.SSP2IF = 0;

    if (SSP2CON2bits.ACKSTAT) {
        SSP2CON2bits.PEN = 1;
        return false; // transmission error 
    }
    return true; // transmission ok; 
}

char I2C_Read(bool a) {

    SSP2CON2bits.RCEN = 1;

    while (!(PIR3bits.SSP2IF));
    PIR3bits.SSP2IF = 0;

    uint8_t temp = SSP2BUF;  // Read data from SSPBUF

    ACKDT2 = a;  //Acknowledge bit
    ACKEN2 = 1;  // Acknowledge sequence
    while (!(PIR3bits.SSP2IF));
    PIR3bits.SSP2IF = 0;

    return temp;
}