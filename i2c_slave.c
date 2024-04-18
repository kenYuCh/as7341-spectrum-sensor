
#include <stdint.h>
#include <xc.h>
#include "i2c_slave.h"
#include "my_helpers.h"



#define SLAVE_ADDRESS 0x68
#define MAX_REGISTERS 64u

static volatile uint8_t slaveData[MAX_REGISTERS] = {0};

void i2c_slave_init() {
    SSP1STAT = 0x00;
    SSP1CON1 = 0x36;
    SSP1CON2 = 0x01;

    // Set slave address
    SSP1ADD = SLAVE_ADDRESS << 1;
    SSP1MSK = 0xFF;

	PIR3bits.SSP1IF = 0;
    PIE3bits.SSP1IE = 1;

    // Set all data to zero
    for (uint8_t i = 0; i < MAX_REGISTERS; i++) {
        slaveData[i] = 0;
    }
}

void i2c_slave_set_register(uint8_t registerAddress, uint8_t value) {
    slaveData[registerAddress] = value;
}

uint8_t i2c_slave_get_register(uint8_t registerAddress) {
    return slaveData[registerAddress];
}


void i2c_slave_set_register_with_lock(uint8_t registerAddress, uint8_t value){
    GIE = 0;
    i2c_slave_set_register(registerAddress, value);
    GIE = 1;
}

uint8_t i2c_slave_get_register_with_lock(uint8_t registerAddress){
    GIE = 0;
    uint8_t result = i2c_slave_get_register(registerAddress);
    GIE = 1;
    return result;
}



static uint8_t currentRegister = 0;

#define STATE_ADDRESS 0
#define STATE_REGISTER 1
#define STATE_DATA 2

static uint8_t currentState = 0;


void i2c_slave_interrupt() {

	// Get read bit
	uint8_t isRead = SSP1STATbits.R_nW;
	int8_t isNack = SSP1CON2bits.ACKSTAT;

    // If nack -> done
	if (isNack) {
        currentState = STATE_ADDRESS;
		return;
	}

	// Check if read, otherwise write
	if (isRead) {

        SSP1BUF = slaveData[currentRegister];

		currentRegister++;

		if (currentRegister >= MAX_REGISTERS) {
            currentRegister = 0;
        }

	} else {

        uint8_t value = SSP1BUF;

        switch (currentState) {

            case STATE_ADDRESS:
                currentState = STATE_REGISTER;
                break;

            case STATE_REGISTER:
                currentRegister = value % MAX_REGISTERS;
                currentState = STATE_DATA;
                break;

            case STATE_DATA:
                // Write only
                if (currentRegister < 57) {
                    slaveData[currentRegister] = value;
                }
                currentState = STATE_ADDRESS;
                break;

            default:
                break;
        }
	}

    // Release clock
    SSP1CON1bits.CKP = 1;
}

// handy debug
void dump_memory_map(){
    dump_buffer(slaveData, sizeof(slaveData));
}