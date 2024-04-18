
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "configuration_bits.h"
#include "as7341_model.h"
#include "i2c_master.h"

eMode_t measurement_mode;
as7341_gpio_mode_t gpio_mode;

void as7341_begin() {
    as7341_init(SPM);
//    enable_spectral_interrupt(true);
    set_ATIME(100); // 0x81 // 100
    set_ASTEP(999); // 0xCA, 0xCB 999 = 0x3E7
    set_AGAIN(9); // 64x
//    set_interrupt_threshold(300, 10000);
//    set_interrupt_persistence(0); // cycle_count
//    set_spectral_threshold_channel(0);
//    set_spectral_threshold_channel(1);
//    set_spectral_threshold_channel(2);
//    set_spectral_threshold_channel(3);
//    set_spectral_threshold_channel(4);
    as7341_enableLED(true);
    printf("as7341_init...\r\n");
//    set_WTIME(0x0A); // 10
//    enable_WITME(true);
//    as7341_power_enable(true);
};

void as7341_init(eMode_t mode) {
    uint8_t as7341_id = as7341_i2c_read(AS7341_ID);
    if(as7341_id != 0) {
        printf("Could not find AS7341, Try to find again\r\n");
    } else {
        as7341_power_enable(true);
        measurement_mode = mode;
        printf("AS7341-GPIO: %u, CHIP-ID: %2X \r\n", mode, as7341_id);
    };
};

/*
function? enable PON - PON_bit set to 0 (0xFE = 11111110)
info?	power on - as7341 power enable PON bit[0] true 1 / false 0
*/
void as7341_power_enable(bool state) {
    uint8_t regVal = as7341_i2c_read(AS7341_ENABLE);
    regVal = (state == true) ?  regVal | (1<<0) : regVal & (~1);
    uint8_t flag = as7341_i2c_write(AS7341_ENABLE, regVal);
    printf("Initialization is %s?\r\n", (flag == 1) ? "complete" : "wrong");
    as7341_i2c_write(0x00, 0x30);
};

// spectral measurement enable.  Disable SP_EN bit.
// ok 00000010 = 0x02, bit[1]  0: Spectral Measurement(spm) Disabled 1: Spectral Measurement Enabled
void enable_sp_en(bool state) {
    uint8_t regVal = as7341_i2c_read(AS7341_ENABLE);
    as7341_i2c_write(AS7341_ENABLE, (state ? (regVal | (1<<1)) : regVal & (~(1<<1))));
};

/* ok , Start SMUX command while keeping power on (SMUXEN = ?1? and PON = ?1?) - bit[4]
 * enable SMUX (0x10 = 0001 0000)
 * disable SMUX(0xEF = 1110 1111)
 */
void enable_smux(bool state) {
    uint8_t regVal = as7341_i2c_read(AS7341_ENABLE);
    if(state == true) {
        regVal = regVal | (1<<4);
    } else {
        regVal = regVal & (~(1<<4));
    };
    as7341_i2c_write(AS7341_ENABLE, regVal);
//    as7341_i2c_write(AS7341_ENABLE, state ? (regVal | (1<<4)) : (regVal & (~(1<<4))));
};

/*
enable flicker detection
*/
void enable_flicker_detection(bool state) {
    uint8_t regVal = as7341_i2c_read(AS7341_ENABLE);
    regVal = state ? (regVal | (1 << 6)) : (regVal & ~(1 << 6));
    as7341_i2c_write(AS7341_ENABLE, regVal);
};


void as7341_config(eMode_t mode) {
    uint8_t data = 0;
    set_bank(true);
    data = as7341_i2c_read(AS7341_CONFIG);
    switch(mode){
    case SPM : 
        data = (data & (~3)) | SPM;
        break;
    case SYNS :
        data = (data & (~3)) | SYNS;
        break;
    case SYND :
        data = (data & (~3)) | SYND;
        break;
    default : 
        break;
    as7341_i2c_write(AS7341_CONFIG, data);
    set_bank(false);
  }
};



void set_F1F4ClearNIR() {
    I2C_Start();
    I2C_Write(AS7341_SLAVE_ADDRESS << 1);
    as7341_write_byte(0x00, 0x30); // F3 left set to ADC2
    as7341_write_byte(0x01, 0x01); // F1 left set to ADC0
    as7341_write_byte(0x02, 0x00); // Reserved or disabled
    as7341_write_byte(0x03, 0x00); // F8 left disabled
    as7341_write_byte(0x04, 0x00); // F6 left disabled
    
    as7341_write_byte(0x05, 0x42); // F4 left connected to ADC3/f2 left connected to ADC1
    as7341_write_byte(0x06, 0x00); // F5 left disbled
    as7341_write_byte(0x07, 0x00); // F7 left disbled
    as7341_write_byte(0x08, 0x50); // CLEAR connected to ADC4
    as7341_write_byte(0x09, 0x00); // F5 right disabled
    
    as7341_write_byte(0x0A, 0x00); // F7 right disabled
    as7341_write_byte(0x0B, 0x00); // Reserved or disabled
    as7341_write_byte(0x0C, 0x20); // F2 right connected to ADC1
    as7341_write_byte(0x0D, 0x04); // F4 right connected to ADC3
    as7341_write_byte(0x0E, 0x00); // F6/F8 right disabled
    
    as7341_write_byte(0x0F, 0x30); // F3 right connected to AD2
    as7341_write_byte(0x10, 0x01); // F1 right connected to AD0
    as7341_write_byte(0x11, 0x50); // CLEAR right connected to AD4
    as7341_write_byte(0x12, 0x00); // Reserved or disabled
    as7341_write_byte(0x13, 0x06); // NIR connected to ADC5
    
    I2C_Stop();
};

void set_F5F8ClearNIR() {
    I2C_Start();
    I2C_Write(AS7341_SLAVE_ADDRESS << 1);
    as7341_write_byte(0x00, 0x00); // F3 left disable
    as7341_write_byte(0x01, 0x00); // F1 left disable
    as7341_write_byte(0x02, 0x00); // reserved/disable
    as7341_write_byte(0x03, 0x40); // F8 left connected to ADC3
    as7341_write_byte(0x04, 0x02); // F6 left connected to ADC1
    
    as7341_write_byte(0x05, 0x00); // F4/ F2 disabled
    as7341_write_byte(0x06, 0x10); // F5 left connected to ADC0
    as7341_write_byte(0x07, 0x03); // F7 left connected to ADC2
    as7341_write_byte(0x08, 0x50); // CLEAR Connected to ADC4
    as7341_write_byte(0x09, 0x10); // F5 right connected to ADC0
    
    as7341_write_byte(0x0A, 0x03); // F7 right connected to ADC2
    as7341_write_byte(0x0B, 0x00); // Reserved or disabled
    as7341_write_byte(0x0C, 0x00); // F2 right disabled
    as7341_write_byte(0x0D, 0x00); // F4 right disabled
    as7341_write_byte(0x0E, 0x24); // F8 right connected to ADC2/ F6 right connected to ADC1
    
    as7341_write_byte(0x0F, 0x00); // F3 right disabled
    as7341_write_byte(0x10, 0x00); // F1 right disabled
    as7341_write_byte(0x11, 0x50); // CLEAR right connected to AD4
    as7341_write_byte(0x12, 0x00); // Reserved or disabled
    as7341_write_byte(0x13, 0x06); // NIR connected to ADC5
    I2C_Stop();
};
void as7341_write_byte(uint8_t reg, uint8_t value) {
    I2C_Write(reg); 
    I2C_Write(value); 
};


void flicker_detection_config() {
    as7341_i2c_write(0x00, 0x00); // F3 left disable
    as7341_i2c_write(0x01, 0x00); // F1 left disable
    as7341_i2c_write(0x02, 0x00); // reserved/disable
    as7341_i2c_write(0x03, 0x00); // F8 left connected to ADC3
    as7341_i2c_write(0x04, 0x00); // F6 left connected to ADC1
    as7341_i2c_write(0x05, 0x00); // F4/ F2 disabled
    as7341_i2c_write(0x06, 0x00); // F5 left connected to ADC0
    as7341_i2c_write(0x07, 0x00); // F7 left connected to ADC2
    as7341_i2c_write(0x08, 0x00); // CLEAR Connected to ADC4
    as7341_i2c_write(0x09, 0x00); // F5 right connected to ADC0
    as7341_i2c_write(0x0A, 0x00); // F7 right connected to ADC2
    as7341_i2c_write(0x0B, 0x00); // Reserved or disabled
    as7341_i2c_write(0x0C, 0x00); // F2 right disabled
    as7341_i2c_write(0x0D, 0x00); // F4 right disabled
    as7341_i2c_write(0x0E, 0x00); // F8 right connected to ADC2/ F6 right connected to ADC1
    as7341_i2c_write(0x0F, 0x00); // F3 right disabled
    as7341_i2c_write(0x10, 0x00); // F1 right disabled
    as7341_i2c_write(0x11, 0x00); // CLEAR right connected to AD4
    as7341_i2c_write(0x12, 0x00); // Reserved or disabled
    as7341_i2c_write(0x13, 0x60); // NIR connected to ADC5
};

void as7341_start_measurement(channel_t channel) {
    uint8_t data = 0;
    data = as7341_i2c_read(AS7341_CFG0);
    data = data & (~(1<<4));
    as7341_i2c_write(AS7341_CFG0, data);
    
    enable_sp_en(false); // spectral_enable
    as7341_smux_cmd_config(); // smux_cmd - write
   
    if(channel  == F1F4ClearNIR) {
        printf("start_measurement_channel: F1F4ClearNIR \r\n");
        set_F1F4ClearNIR();
    } else if(channel == F5F8ClearNIR){
        printf("start_measurement_channel: F5F8ClearNIR \r\n");
        set_F5F8ClearNIR();
    };
    enable_smux(true);
    if(measurement_mode == SYNS) {
        printf("as7341_start_measurement: SYNS \r\n");
        set_GPIO_mode(AS7341_GPIO_INPUT);
        as7341_config(SYNS);
    } else if(measurement_mode == SPM) {
        printf("as7341_start_measurement: SPM \r\n");
        as7341_config(SPM);
    };
    enable_sp_en(true);
    __delay_ms(100);
    if(measurement_mode == SPM){
//       while(!is_measurement_complete());
    };
};

uint8_t read_flicker_data(){
    return 0;
};



bool is_measurement_complete() {
    uint8_t status;
	status = as7341_i2c_read(AS7341_STATUS2); 
	if((status & (1<<6))){
		return true;
	} else {
		return false;
	};
};

uint16_t get_channel_data(uint8_t channel){
  uint16_t data[2];
  uint16_t channelData = 0x0000;
  data[0] = as7341_i2c_read(AS7341_CH0_DATA_L + channel*2); 
  data[1] = as7341_i2c_read(AS7341_CH0_DATA_H + channel*2); 
  channelData = data[1];
  channelData = (channelData<<8) | data[0];
  __delay_ms(50);
  return channelData;
};
/*
 Use SMUX to read data from the low channel.
*/
F1F4_CLEAR_NIR_DATA_t readSpectralDataOne() {
    F1F4_CLEAR_NIR_DATA_t data;
    data.ADC_CHANNEL_F1 = get_channel_data(0);
    data.ADC_CHANNEL_F2 = get_channel_data(1);
    data.ADC_CHANNEL_F3 = get_channel_data(2);
    data.ADC_CHANNEL_F4 = get_channel_data(3);
    data.ADC_CHANNEL_CLEAR = get_channel_data(4);
    data.ADC_CHANNEL_NIR = get_channel_data(5);
    return data;
};
/*
Use SMUX to read data from the hight channel 
*/
F5F8_CLEAR_NIR_DATA_t readSpectralDataTwo() {
    F5F8_CLEAR_NIR_DATA_t data;
    data.ADC_CHANNEL_F5 = get_channel_data(0);
    data.ADC_CHANNEL_F6 = get_channel_data(1);
    data.ADC_CHANNEL_F7 = get_channel_data(2);
    data.ADC_CHANNEL_F8 = get_channel_data(3);
    data.ADC_CHANNEL_CLEAR = get_channel_data(4);
    data.ADC_CHANNEL_NIR = get_channel_data(5);
    return data;
};

/* Write SMUX configuration from RAM to set SMUX chain Register: CFG6 / 0xAF
 * 0 - ROM code initialization of SMUX                  0000 0000 = 0x00
 * 1 - Read SMUX configuration to RAM from SMUX chain , 0000 1000 = 0x08
 * 2 - Write SMUX configuration from RAM to SMUX chain, 0001 0000 = 0x10 (default)
 * 3 - Reserved, do not use,                            0001 1000 = 0x18
 */
void as7341_smux_cmd_config(){
    as7341_i2c_write(AS7341_CFG6, 0x10); // 0001 0000 bit[4:3], 
};


/*
 * Set GPIO to input or output mode.
*/
void set_GPIO_mode(uint8_t mode) {
  uint8_t data;
  data = as7341_i2c_read(AS7341_GPIO2);  
  if(mode == AS7341_GPIO_INPUT){
     data = data | (1<<2);
  }
  if(mode == AS7341_GPIO_OUTPUT){
     data = data & (~(1<<2));
  }
  as7341_i2c_write(AS7341_GPIO2,data);
  
}


// ok , if set_bank is true,can modify to 0x60~0x74 registers, else is 0x80 and above.
bool set_bank(bool state) {
    uint8_t regVal = as7341_i2c_read(AS7341_CFG0);
    if(state == true) {
        regVal = regVal | (1<<4); // REG_BANK_bit set to 1
    } else {
        regVal = regVal & (~(1<<4)); // REG_BANK_bit set to 0
    };
    as7341_i2c_write(AS7341_CFG0, regVal);
};

bool set_ATIME(uint8_t value) {
    as7341_i2c_write(AS7341_ATIME, value);
};
/*
Sets the ASTEP for integration time from 0 to 65535 in register (0xCA[7:0]) and (0xCB[15:8]), 
integration time = (ATIME + 1) * (ASTEP + 1) * 2.78µS , default = 999 (0x03E7)
 * CA : E7, CB: 03
*/

bool set_ASTEP(uint16_t value) {
    uint8_t high_value, low_value;
    low_value = value & 0x00FF;
    high_value = value >> 8 ;
    as7341_i2c_write(AS7341_ASTEP_L, low_value);
    as7341_i2c_write(AS7341_ASTEP_H, high_value);
    __delay_ms(500);
//    as7341_i2c_write(AS7341_ASTEP_L, value1); // 0xCA ASTEP[7:0] 0xE7
//    as7341_i2c_write(AS7341_ASTEP_H, value2); // 0xCB ASTEP[15:8] 0x03
}; 
//
bool set_AGAIN(as7341_gain_t value) {
    if(value > 10) value = 10;
    as7341_i2c_write(AS7341_CFG1, value);
};



bool set_LED_current(uint16_t led_current_ma) {
//    if (led_current_ma > 258) {
//        return false;
//    }
//    if (led_current_ma < 4) {
//      led_current_ma = 4;
//    }
    set_bank(true); // Access 0x60 0x74
    bool result = as7341_i2c_write(AS7341_LED, 0x98);
    set_bank(false); // Access registers 0x80 and above (default)
    return result;
};
/*
function:	enable led
info? true		: Enable
      false		: Anergy 
*/
bool as7341_enableLED(bool state) {
    uint8_t data = 0;
    uint8_t data1 = 0;
    set_bank(true); 
    data = as7341_i2c_read(AS7341_CONFIG);
    data1 = as7341_i2c_read(AS7341_LED);
    if(state == true) {
        data = data | 0x08;
    } else {
        data = data & 0xF7;     // 1111 0111
        data1 = data1 & 0x7F;   // 0111 1111
        as7341_i2c_write(AS7341_LED, data1);
    }
    as7341_i2c_write(AS7341_CONFIG, data);
    set_bank(false); 
};

/*
    Control the brightness of the LED.
*/
void control_LED(uint8_t LED, uint8_t current) {
    uint8_t data = 0;
    if(current < 1) current = 1;
        current--;
    if(current > 19) { 
        current = 19;
    };
    set_bank(true); 
    if(LED == true) {
      data = 0x80 | current;
    } else {
      data = current;
    }
    as7341_i2c_write(AS7341_LED, data);
    __delay_ms(100);
    set_bank(false);
};

/******************************************************************************
function:	Determine whether the threshold setting is exceeded
info?Spectral interruptions occur when the set threshold is exceeded
******************************************************************************/
void is_as7341_interrupt() {
    uint8_t data = as7341_i2c_read(AS7341_STATUS);
    if(data & AS7341_ENABLE){
      printf("Spectral interrupt generation!\r\n");
    } else {
      return;
    };
};

/******************************************************************************
function:	clear interrupt
info?		This register is self-clearing, meaning that writing a "1" to any bit in the
	register clears that status bit. 
******************************************************************************/
void clear_interrupt(){
    as7341_i2c_write(AS7341_STATUS, 0xFF);
};

/******************************************************************************
function:	enable spectral interrupt
info?
******************************************************************************/
void enable_spectral_interrupt(bool state) {
    uint8_t data;
    data = as7341_i2c_read(AS7341_INTENAB);
    if(state == true) {
      data = data | (1<<3);
      as7341_i2c_write(AS7341_INTENAB,data);
    } else {
      data = data & (~(1<<3));
      as7341_i2c_write(AS7341_INTENAB,data);
    };
};
/******************************************************************************
function:Spectral Interrupt Persistence
	value:      CHANNEL:
	0			Every spectral cycle generates aninterrupt
	1			1
	2			2
	3			3
	4			5
	5			10
	...			5*(value-3)
	14			55
	15			60
******************************************************************************/
void set_interrupt_persistence(uint8_t value) {
	as7341_i2c_write(AS7341_PERS, value);
};


/******************************************************************************
function:	Set the interrupt threshold up and hight
info?
******************************************************************************/
void set_interrupt_threshold(uint8_t low_threshold, uint16_t hight_threshold) {
    if(low_threshold >= hight_threshold){
        return;
    } else {
        as7341_i2c_write(AS7341_SP_LOW_TH_L,low_threshold);
        as7341_i2c_write(AS7341_SP_LOW_TH_H,low_threshold>>8);
        as7341_i2c_write(AS7341_SP_HIGH_TH_L,hight_threshold);
        as7341_i2c_write(AS7341_SP_HIGH_TH_H,hight_threshold>>8); 
        __delay_ms(20);
    };
};

/******************************************************************************
function:	Set the Spectral Threshold Channel
		VALUE 			CHANNEL
		0 				CH0
		1 				CH1
		2 				CH2
		3 				CH3
		4 				CH4
******************************************************************************/
void set_spectral_threshold_channel(uint8_t value) {
	as7341_i2c_write(AS7341_CFG12, value);
};

/******************************************************************************
function:	get low threshold
info?
******************************************************************************/


uint16_t get_low_threshold() {
    uint16_t data; 
    data = as7341_i2c_read(AS7341_SP_LOW_TH_L);  
    data = (as7341_i2c_read(AS7341_SP_LOW_TH_H)<<8) | data; 
    return data;
};

/******************************************************************************
function:	get high threshold
info?
******************************************************************************/
uint16_t get_high_threshold() {
  uint16_t data;
  data = as7341_i2c_read(AS7341_SP_HIGH_TH_L);
  data = (as7341_i2c_read(AS7341_SP_HIGH_TH_H) <<8) | data;
  return data;
};

/*
function:	syns interrupt set. 
*/
void syns_interrupt_sel() {
    as7341_i2c_write(AS7341_CONFIG, 0x05);
};

/*
function:	disable power,spectral reading, flicker detection   
*/
void disabledAll() {
    as7341_i2c_write(AS7341_ENABLE, 0x02);
};
//



// The SMUXEN bit gets cleared automatically as soon as SMUX operation is finished.
bool get_smux_enabled() {
  bool isEnabled = false;
  uint8_t regVal = as7341_i2c_read(AS7341_ENABLE);
  if ( (regVal & 0x10) == 0x10) {
    return isEnabled = true;
  } else {
    return isEnabled = false;
  };
};


/* Enable special interrupt (SINT_SMUX). As soon as SMUX command has finished interrupt is activated.
 * 
 
 */
bool enableSMUX_Interrupt(){
    as7341_i2c_write(AS7341_CFG9, 0x10);
};

// Enable special interrupt SIEN (Register: INTENAB / 0xF9)
bool enable_Special_Interrupt(){
    as7341_i2c_write(AS7341_INTENAB, 0x01);
};

bool set_integration_mode(eMode_t *integration_mode) {
    as7341_i2c_write(AS7341_CONFIG, integration_mode);
};



bool enable_WITME(bool state) {
    uint8_t regVal = as7341_i2c_read(AS7341_ENABLE);
    uint8_t temp = regVal;
    if(state){
        regVal = regVal | 0x08; // bit[4] = enable WITEM_EN (0x10 = 0001 0000)
    } else {
        regVal = temp & 0xF7; // bit[4] = disable WITEM_EN(0xEF = 1110 1111)
    };
    as7341_i2c_write(AS7341_ENABLE, regVal);
};

bool set_WTIME(uint8_t value) {
    as7341_i2c_write(AS7341_WTIME, value);
};

void INT_MODE(uint8_t mode) {
  uint8_t regVal = as7341_i2c_read(AS7341_CONFIG);
  regVal = regVal & 0xFC;
  regVal = regVal | mode;
  as7341_i2c_write(AS7341_CONFIG, regVal);
}
void set_APERS(as7341_int_cycle_count_t count) {
     as7341_i2c_write(AS7341_PERS, count);
};

void flicker_detect_config() {
    // SMUX Config for Flicker- register (0x13)left set to ADC6 for flicker
    as7341_i2c_write(0x00, 0x00); // F3 left disable
    as7341_i2c_write(0x01, 0x00); // F1 left disable
    as7341_i2c_write(0x02, 0x00); // reserved/disable
    as7341_i2c_write(0x03, 0x00); // F8 left connected to ADC3
    as7341_i2c_write(0x04, 0x00); // F6 left connected to ADC1
    as7341_i2c_write(0x05, 0x00); // F4/ F2 disabled
    as7341_i2c_write(0x06, 0x00); // F5 left connected to ADC0
    as7341_i2c_write(0x07, 0x00); // F7 left connected to ADC2
    as7341_i2c_write(0x08, 0x00); // CLEAR Connected to ADC4
    as7341_i2c_write(0x09, 0x00); // F5 right connected to ADC0
    as7341_i2c_write(0x0A, 0x00); // F7 right connected to ADC2
    as7341_i2c_write(0x0B, 0x00); // Reserved or disabled
    as7341_i2c_write(0x0C, 0x00); // F2 right disabled
    as7341_i2c_write(0x0D, 0x00); // F4 right disabled
    as7341_i2c_write(0x0E, 0x00); // F8 right connected to ADC2/ F6 right connected to ADC1
    as7341_i2c_write(0x0F, 0x00); // F3 right disabled
    as7341_i2c_write(0x10, 0x00); // F1 right disabled
    as7341_i2c_write(0x11, 0x00); // CLEAR right connected to AD4
    as7341_i2c_write(0x12, 0x00); // Reserved or disabled
    as7341_i2c_write(0x13, 0x60); // NIR connected to ADC5
};
uint8_t as7341_i2c_write(uint8_t reg, uint8_t value) {
    uint8_t data;
    I2C_Start();
    I2C_Write(AS7341_SLAVE_ADDRESS << 1); // slave address + write,
    I2C_Write(reg); 
    data = I2C_Write(value);
    I2C_Stop();
    if(data == true) {
        return 1;
    } else {
        return 0;
    };
    
};
uint8_t as7341_i2c_read(uint8_t reg) {
    uint8_t buf;
    I2C_Start();
    I2C_Write(AS7341_SLAVE_ADDRESS << 1);
    I2C_Write(reg);
    I2C_Start();
    I2C_Write((AS7341_SLAVE_ADDRESS << 1) | 0x01);
    buf = I2C_Read(nack);
    I2C_Stop();
    return buf;
};


uint16_t readTwoReg(uint8_t reg){
    uint8_t readingL; 
    uint16_t readingH; 
    uint16_t reading = 0;
    I2C_Start();
    I2C_Write(AS7341_SLAVE_ADDRESS << 1);
    I2C_Write(reg);
    I2C_Start();
    I2C_Write((AS7341_SLAVE_ADDRESS << 1) | 0x01);
    readingL = I2C_Read(ack); // 0xEE
    readingH = I2C_Read(nack); // 0x03
    readingH = readingH << 8;
    reading = (readingH | readingL);
    I2C_Stop();
    return reading;
};
