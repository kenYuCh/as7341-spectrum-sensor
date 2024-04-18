
#ifndef AS7341_MODEL_H
#define	AS7341_MODEL_H

#define AS7341_SLAVE_ADDRESS 0x39 ///< AS7341 default i2c address
    
#define AS7341_ASTATUS 0x60     ///< AS7341_ASTATUS (unused)
#define AS7341_CH0_DATA_L_ 0x61 ///< AS7341_CH0_DATA_L (unused)
#define AS7341_CH0_DATA_H_ 0x62 ///< AS7341_CH0_DATA_H (unused)
#define AS7341_ITIME_L 0x63     ///< AS7341_ITIME_L (unused) 99
#define AS7341_ITIME_M 0x64     ///< AS7341_ITIME_M (unused) 100
#define AS7341_ITIME_H 0x65     ///< AS7341_ITIME_H (unused) 101
#define AS7341_CONFIG 0x70 ///< Enables LED control and sets light sensing mode
#define AS7341_STAT 0x71   ///< AS7341_STAT (unused)
#define AS7341_EDGE 0x72   ///< AS7341_EDGE (unused)
#define AS7341_GPIO 0x73   ///< Connects photo diode to GPIO or INT pins
#define AS7341_LED 0x74    ///< LED Register; Enables and sets current limit
#define AS7341_ENABLE 0x80 
///< Main enable register. Controls SMUX, Flicker Detection, Spectral
       ///< Measurements and Power
#define AS7341_ATIME 0x81       ///< Sets ADC integration step count
#define AS7341_WTIME 0x83       ///< AS7341_WTIME (unused)
#define AS7341_SP_LOW_TH_L 0x84 ///< Spectral measurement Low Threshold low byte
#define AS7341_SP_LOW_TH_H                                                     \
  0x85 ///< Spectral measurement Low Threshold high byte
#define AS7341_SP_HIGH_TH_L                                                    \
  0x86 ///< Spectral measurement High Threshold low byte
#define AS7341_SP_HIGH_TH_H                                                    \
  0x87                    ///< Spectral measurement High Threshold low byte
#define AS7341_AUXID 0x90 ///< AS7341_AUXID (unused)
#define AS7341_REVID 0x91 ///< AS7341_REVID (unused)
#define AS7341_ID 0x92    ///< AS7341_ID (unused)
#define AS7341_STATUS                                                          \
  0x93 ///< Interrupt status registers. Indicates the occourance of an interrupt
#define AS7341_ASTATUS_ 0x94   ///< AS7341_ASTATUS, same as 0x60 (unused)
#define AS7341_CH0_DATA_L 0x95 ///< ADC Channel Data
#define AS7341_CH0_DATA_H 0x96 ///< ADC Channel Data
#define AS7341_CH1_DATA_L 0x97 ///< ADC Channel Data
#define AS7341_CH1_DATA_H 0x98 ///< ADC Channel Data
#define AS7341_CH2_DATA_L 0x99 ///< ADC Channel Data
#define AS7341_CH2_DATA_H 0x9A ///< ADC Channel Data
#define AS7341_CH3_DATA_L 0x9B ///< ADC Channel Data
#define AS7341_CH3_DATA_H 0x9C ///< ADC Channel Data
#define AS7341_CH4_DATA_L 0x9D ///< ADC Channel Data
#define AS7341_CH4_DATA_H 0x9E ///< ADC Channel Data
#define AS7341_CH5_DATA_L 0x9F ///< ADC Channel Data
#define AS7341_CH5_DATA_H 0xA0 ///< ADC Channel Data
#define AS7341_STATUS2 0xA3 ///< Measurement status flags; saturation, validity
#define AS7341_STATUS3                                                         \
  0xA4 ///< Spectral interrupt source, high or low threshold
#define AS7341_STATUS5 0xA6 ///< AS7341_STATUS5 (unused)
#define AS7341_STATUS6 0xA7 ///< AS7341_STATUS6 (unused)
#define AS7341_CFG0                                                            \
  0xA9 ///< Sets Low power mode, Register bank, and Trigger lengthening
#define AS7341_CFG1 0xAA ///< Controls ADC Gain
#define AS7341_CFG3 0xAC ///< AS7341_CFG3 (unused)
#define AS7341_CFG6 0xAF ///< Used to configure Smux
#define AS7341_CFG8 0xB1 ///< AS7341_CFG8 (unused)
#define AS7341_CFG9                                                            \
  0xB2 ///< Enables flicker detection and smux command completion system
       ///< interrupts
#define AS7341_CFG10 0xB3 ///< AS7341_CFG10 (unused)
#define AS7341_CFG12                                                           \
  0xB5 ///< Spectral threshold channel for interrupts, persistence and auto-gain
#define AS7341_PERS                                                            \
  0xBD ///< Number of measurement cycles outside thresholds to trigger an
       ///< interupt
#define AS7341_GPIO2                                                           \
  0xBE ///< GPIO Settings and status: polarity, direction, sets output, reads
       ///< input
#define AS7341_ASTEP_L 0xCA      ///< Integration step size ow byte
#define AS7341_ASTEP_H 0xCB      ///< Integration step size high byte
#define AS7341_AGC_GAIN_MAX 0xCF ///< AS7341_AGC_GAIN_MAX (unused)
#define AS7341_AZ_CONFIG 0xD6    ///< AS7341_AZ_CONFIG (unused)
#define AS7341_FD_TIME1 0xD8 ///< Flicker detection integration time low byte
#define AS7341_FD_TIME2 0xDA ///< Flicker detection gain and high nibble
#define AS7341_FD_CFG0 0xD7  ///< AS7341_FD_CFG0 (unused)
#define AS7341_FD_STATUS                                                       \
  0xDB ///< Flicker detection status; measurement valid, saturation, flicker
       ///< type
#define AS7341_INTENAB 0xF9  ///< Enables individual interrupt types
#define AS7341_CONTROL 0xFA  ///< Auto-zero, fifo clear, clear SAI active
#define AS7341_FIFO_MAP 0xFC ///< AS7341_FIFO_MAP (unused)
#define AS7341_FIFO_LVL 0xFD ///< AS7341_FIFO_LVL (unused)
#define AS7341_FDATA_L 0xFE  ///< AS7341_FDATA_L (unused)
#define AS7341_FDATA_H 0xFF  ///< AS7341_FDATA_H (unused)

#define AS7341_SPECTRAL_INT_HIGH_MSK                                           \
  0b00100000 ///< bitmask to check for a high threshold interrupt
#define AS7341_SPECTRAL_INT_LOW_MSK                                            \
  0b00010000 ///< bitmask to check for a low threshold interrupt

#define ERR_OK             0      // OK
#define ERR_DATA_BUS      -1      // Data Bus error
#define ERR_IC_VERSION    -2      // Chip version mismatch 


// Variables
bool ack = 0;
bool nack = 1;

typedef enum {
  SPM = 0x00,
  SYNS = 0x01,
  SYND = 0x03,
} eMode_t;

typedef enum  {
  AS7341_SMUX_CMD_ROM_RESET,  ///< ROM code initialization of SMUX
  AS7341_SMUX_CMD_READ,       ///< Read SMUX configuration to RAM from SMUX chain
  AS7341_SMUX_CMD_WRITE,      ///< Write SMUX configuration from RAM to SMUX chain
} AS7341SmuxCommand;

typedef enum{
  F1F4ClearNIR,/**<Map the values of the registers of 6 channels to F1,F2,F3,F4,clear,NIR>*/
  F5F8ClearNIR,/**<Map the values of the registers of 6 channels to F5,F6,F7,F8,clear,NIR>*/
} channel_t;

typedef struct{
    uint16_t ADC_CHANNEL_F1;/**<F1 diode data>*/
    uint16_t ADC_CHANNEL_F2;/**<F2 diode data>*/
    uint16_t ADC_CHANNEL_F3;/**<F3 diode data>*/
    uint16_t ADC_CHANNEL_F4;/**<F4 diode data>*/
    uint16_t ADC_CHANNEL_CLEAR;/**<clear diode data>*/
    uint16_t ADC_CHANNEL_NIR;/**<NIR diode data>*/
} F1F4_CLEAR_NIR_DATA_t;

typedef struct{
    uint16_t ADC_CHANNEL_F5;/**<F5 diode data>*/
    uint16_t ADC_CHANNEL_F6;/**<F6 diode data>*/
    uint16_t ADC_CHANNEL_F7;/**<F7 diode data>*/
    uint16_t ADC_CHANNEL_F8;/**<F8 diode data>*/
    uint16_t ADC_CHANNEL_CLEAR;/**<clear diode data>*/
    uint16_t ADC_CHANNEL_NIR;/**<NIR diode data>*/
} F5F8_CLEAR_NIR_DATA_t;


typedef enum {
  AS7341_GAIN_0_5X, // 0
  AS7341_GAIN_1X,   // 1
  AS7341_GAIN_2X,   // 2
  AS7341_GAIN_4X,   // 3
  AS7341_GAIN_8X,   // 4
  AS7341_GAIN_16X,  // 5
  AS7341_GAIN_32X,  // 6
  AS7341_GAIN_64X,  // 7
  AS7341_GAIN_128X, // 8
  AS7341_GAIN_256X, // 9
  AS7341_GAIN_512X, // 10
} as7341_gain_t;

as7341_gain_t GAIN = AS7341_GAIN_64X;

typedef enum {
  AS7341_INT_COUNT_ALL, ///< 0
  AS7341_INT_COUNT_1,   ///< 1
  AS7341_INT_COUNT_2,   ///< 2
  AS7341_INT_COUNT_3,   ///< 3
  AS7341_INT_COUNT_5,   ///< 4
  AS7341_INT_COUNT_10,  ///< 5
  AS7341_INT_COUNT_15,  ///< 6
  AS7341_INT_COUNT_20,  ///< 7
  AS7341_INT_COUNT_25,  ///< 8
  AS7341_INT_COUNT_30,  ///< 9
  AS7341_INT_COUNT_35,  ///< 10
  AS7341_INT_COUNT_40,  ///< 11
  AS7341_INT_COUNT_45,  ///< 12
  AS7341_INT_COUNT_50,  ///< 13
  AS7341_INT_COUNT_55,  ///< 14
  AS7341_INT_COUNT_60,  ///< 15
} as7341_int_cycle_count_t;

typedef enum {
  AS7341_GPIO_OUTPUT, ///< THhe GPIO pin is configured as an open drain output
  AS7341_GPIO_INPUT,  ///< The GPIO Pin is set as a high-impedence input
} as7341_gpio_mode_t;


//
void as7341_begin();
void as7341_init(eMode_t mode);
void as7341_power_enable(bool state);
void enable_sp_en(bool state); // enable spectral measurement. Disable SP_EN bit
void enable_smux(bool state);
void enable_flicker_detection(bool state);
void set_F1F4ClearNIR(void);
void set_F5F8ClearNIR(void);
void as7341_config(eMode_t mode);
void flicker_detection_config(void);
void as7341_start_measurement(channel_t channel);
bool is_measurement_complete(void);
uint8_t read_flicker_data();
uint16_t get_channel_data(uint8_t channel);
F1F4_CLEAR_NIR_DATA_t readSpectralDataOne();
F5F8_CLEAR_NIR_DATA_t readSpectralDataTwo();
void as7341_smux_cmd_config(void);
bool enableSMUX_Interrupt(void);
void set_GPIO_mode(uint8_t mode);

bool set_LED_current(uint16_t led_current_ma);
bool as7341_enableLED(bool state);
void control_LED(uint8_t LED, uint8_t current);
bool set_bank(bool state);

// Sets the spectral sensitivity. 0xAA
bool set_AGAIN(as7341_gain_t value);
// Integration time. Sets the number of integration steps from 1 to 256.
bool set_ATIME(uint8_t value);
// Integration time step size. default is 999(2.78ms). 0xCA[7:0] / 0xCB[15:8]
bool set_ASTEP(uint16_t value);
bool set_WTIME(uint8_t value);


void is_as7341_interrupt();
void clear_interrupt();
void enable_spectral_interrupt(bool state);
void set_interrupt_persistence(uint8_t value);

void set_interrupt_threshold(uint8_t low_threshold, uint16_t hight_threshold); // Set the interrupt threshold up and down.
void set_spectral_threshold_channel(uint8_t value);
uint16_t get_low_threshold();
uint16_t get_high_threshold();

void syns_interrupt_sel();

void disabledAll();

// --------------------------------------------------------------------------------


bool enable_WITME(bool state);


void set_APERS(as7341_int_cycle_count_t count);

uint8_t as7341_i2c_write(uint8_t reg, uint8_t value);
uint8_t as7341_i2c_read(uint8_t reg);
void as7341_write_byte(uint8_t reg, uint8_t value);
uint16_t readTwoReg(uint8_t reg);

#endif	/* AS7341_MODEL_H */



