#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "mcc_generated_files/mcc.h"
#include "configuration_bits.h"
#include "configuration.h"
#include "tmr0.h"
#include "my_helpers.h"
#include "i2c_master.h"
#include "i2c_slave.h"
#include "eusart1.h"
#include "i2c_master.h"
#include "as7341_model.h"


volatile uint32_t ticks = 0;

uint32_t get_ticks_with_lock(){
    GIE = 0;
    uint32_t result = ticks;
    GIE = 1;
    return result;
}


uint32_t calculate_diff(uint32_t now, uint32_t prev_ticks) {
    uint32_t diff;
    if (now >= prev_ticks) {
        diff = now - prev_ticks;
    } else {
        // Handling overflow situation
        diff = (UINT32_MAX - prev_ticks) + now + 1;
    }
    return diff;
}

#define INTERRUPT_GlobalInterruptEnable() (INTCONbits.GIE = 1)
#define INTERRUPT_GlobalInterruptDisable() (INTCONbits.GIE = 0)
#define INTERRUPT_PeripheralInterruptEnable() (INTCONbits.PEIE = 1)
#define INTERRUPT_PeripheralInterruptDisable() (INTCONbits.PEIE = 0)

void __interrupt() INTERRUPT_InterruptManager() {
    // timer0 interrupt
    if (PIR0bits.TMR0IF) {
        PIR0bits.TMR0IF = 0;
        ticks++;
    }
    
    // I2C slave interrupt
    if (PIR3bits.SSP1IF == 1) {
        PIR3bits.SSP1IF = 0;
        i2c_slave_interrupt();
    }
}

#define set_RA5_as_UART_TX()              (RA5PPS = 0x0F)
#define reset_RA5_peripheral_pin_select() (RA5PPS = 0x00)

void main() {


    SYSTEM_Initialize();
    tmr0_init();
    EUSART1_Initialize();
    __delay_ms(500);

    reset_RA5_peripheral_pin_select();
    set_RA5_as_UART_TX();

    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    
    i2c_master_init();
    i2c_slave_init();
    
    printf("Hello Light Sensor\r\n");
    printf("sn: %s\r\n", DEVICE_SERIAL);
    
    
    as7341_begin();
    F1F4_CLEAR_NIR_DATA_t data1;
    F5F8_CLEAR_NIR_DATA_t data2;
    as7341_enableLED(true);
    control_LED(true, 19); //Turn on or off the LED and set the brightness of the LED
    while (1){
        uint32_t now = get_ticks_with_lock();
        static uint32_t prev_ticks1 = 0;
        static uint32_t prev_ticks2 = 0;
        
//        if(calculate_diff(now, prev_ticks2) >= 3000){
//            prev_ticks2 = now;
//            as7341_start_measurement(F1F4ClearNIR);
//            __delay_ms(100);
//            as7341_start_measurement(F5F8ClearNIR);
//            __delay_ms(100);
//        };
        
        if(calculate_diff(now, prev_ticks1) >= 5000){
            prev_ticks1 = now;
                as7341_start_measurement(F1F4ClearNIR);
                data1 = readSpectralDataOne();
                printf("Channel-F1-405-425nm: %u, "
                        "Channel-F2-435-455nm: %u, "
                        "Channel-F3-470-490nm: %u, "
                        "Channel-F4-505-525nm: %u, "
                        "Channel-F5-CLEAR: %u, "
                        "Channel-F5-NIR: %u \r\n", 
                        data1.ADC_CHANNEL_F1, 
                        data1.ADC_CHANNEL_F2, 
                        data1.ADC_CHANNEL_F3, 
                        data1.ADC_CHANNEL_F4, 
                        data1.ADC_CHANNEL_CLEAR, 
                        data1.ADC_CHANNEL_NIR);
                __delay_ms(50);
                as7341_start_measurement(F5F8ClearNIR);
                data2 = readSpectralDataTwo();
                printf("Channel-F5-405-425nm: %u, "
                        "Channel-F6-435-455nm: %u, "
                        "Channel-F7-470-490nm: %u, "
                        "Channel-F8-505-525nm: %u, "
                        "Channel-CLEAR-CLEAR: %u, "
                        "Channel-NIR-NIR: %u \r\n",
                        data2.ADC_CHANNEL_F5, 
                        data2.ADC_CHANNEL_F6, 
                        data2.ADC_CHANNEL_F7,
                        data2.ADC_CHANNEL_F8, 
                        data2.ADC_CHANNEL_CLEAR, 
                        data2.ADC_CHANNEL_NIR);
//                is_as7341_interrupt();
                INTERRUPT_GlobalInterruptDisable();
                i2c_slave_set_register(0, data1.ADC_CHANNEL_F1 >> 8);
                i2c_slave_set_register(1, data1.ADC_CHANNEL_F1 & 0xFF);
                i2c_slave_set_register(2, data1.ADC_CHANNEL_F2 >> 8);
                i2c_slave_set_register(3, data1.ADC_CHANNEL_F2 & 0xFF);
                i2c_slave_set_register(4, data1.ADC_CHANNEL_F3 >> 8);
                i2c_slave_set_register(5, data1.ADC_CHANNEL_F3 & 0xFF);
                i2c_slave_set_register(6, data1.ADC_CHANNEL_F4 >> 8);
                i2c_slave_set_register(7, data1.ADC_CHANNEL_F4 & 0xFF);
                i2c_slave_set_register(8, data1.ADC_CHANNEL_CLEAR >> 8);
                i2c_slave_set_register(9, data1.ADC_CHANNEL_CLEAR & 0xFF);
                i2c_slave_set_register(10, data1.ADC_CHANNEL_NIR >> 8);
                i2c_slave_set_register(11, data1.ADC_CHANNEL_NIR & 0xFF);
                i2c_slave_set_register(12, 10); // LED_state
                //
                for (uint8_t i = 0; i < 5; i++) {
                    char buffer[3];
                    buffer[0] = DEVICE_SERIAL[i * 2];
                    buffer[1] = DEVICE_SERIAL[i * 2 + 1];
                    buffer[2] = '\0';
                    uint8_t value = strtol(buffer, NULL, 16);
                    i2c_slave_set_register(57u + i, value);
                }
                INTERRUPT_GlobalInterruptEnable();
                dump_memory_map();            
        }
        

    }
    
 
}


/*
 * Handy Note
 * 2023-09-08
 *      modify from simuAIR
 *      printf output DONE.
 * 2023-09-14
 *      RC2 toggle output DONE
 *      1ms timebase toggle DONE
 * 2023-09-15
 *      get Temperature response.
 * 2023-09-20
 *      add rh measurement function.
 *      add soft_reset - sensor reset.
 *      add user_register
 * 2023-10-03
 *      merge with 4-20 current sensor, to workaround simuAIR i2c slave interface problem. 
 *      merge with soil v1, uart output from RA5
 *      change compiler version from 1.45 to 2.40 !!!
 * 2023-10-04
 *      v1 + xc8 v1.45 -> master OK, slave NG
 *      v2 + xc8 v1.40 -> master NG, slave OK
 *      merge v1 and v2 to v3
 *      v3 + xc8 v1.45 -> master OK, slave NG
 *      v3 + xc8 v1.40 -> master NG, slave OK
 *      add mcc generated code i2c2_master.c/.h and i2c2_master_example.c/.h
 * 2023-10-05
 *      RC4 -> SCL2
 *      RC5 -> SDA2
 *      xc8 v2.40
 * 
 */

// 
