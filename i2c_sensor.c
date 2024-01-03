/*
 * i2c_sensor.c
 *
 *  Created on: 10.12.2023
 *      Author: Andreas Reichenauer
 */

#include "i2c_sensor.h"
#include <I2c/I2c/IfxI2c_I2c.h>
#include <Bsp.h>                      //Board support functions (for the waitTime function)

/*************************************************************************************************************/
/*------------------------------------------------------Macros-----------------------------------------------*/
/*************************************************************************************************************/
#define I2C_FREQ                400000      // Clock frequency of I2C in Hz
#define SENSOR_ADDRESS_7BIT     0x44        // I2C address of the click sensor in 7 bit format
#define SOFT_RESET_COMMAND      0x94        // Sensor command for a soft reset
#define READ_SERIAL_COMMAND     0x89        // Sensor command to read serial number
#define READ_VALUES_COMMAND     0xFD        // Sensor command to read the values with high precision
#define SIZE_SEND               1           // Number of bytes of command to send
#define SIZE_READ               6           // Number of bytes of read data
#define TWO_POW_16              65536       // Value of 2^16 for sensor value conversion
#define DATA_BUFFER_SIZE        10          // Number of bytes reserved for I2C data
#define RESET_WAIT_TIME         100         // ms to wait after soft reset
#define COMMAND_WAIT_TIME       10          // ms to wait after a command was sent for read
#define TEMPERATURE_MIN         -40         // Minimum value for temperature
#define TEMPERATURE_MAX         125         // Maximum value for temperature
#define HUMIDITY_MIN            0           // Minimum value for humidity
#define HUMIDITY_MAX            100         // Maximum value for humidity
#define CONVERSION_OFFSET_TEMP  -45.f       // Offset value for temperature conversion formula from datasheet
#define CONVERSION_FACTOR_TEMP  175.f       // Factor for temperature conversion formula from datasheet
#define CONVERSION_OFFSET_HUM   -6.f        // Offset value for temperature conversion formula from datasheet
#define CONVERSION_FACTOR_HUM   125.f       // Factor for temperature conversion formula from datasheet

/*************************************************************************************************************/
/*-------------------------------------------------Global variables------------------------------------------*/
/*************************************************************************************************************/
static IfxI2c_I2c i2c;                      // I2C handle
static IfxI2c_I2c_Device i2cDev;            // Slave device handle
static uint8 data[DATA_BUFFER_SIZE];        // Data buffer

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
void init_i2c(void){
    IfxI2c_I2c_Config config;                               // Create config structure
    IfxI2c_I2c_initConfig(&config, &MODULE_I2C0);           // Fill structure with default values and Module address

    // Configure pins
    const IfxI2c_Pins pins = {
        &IfxI2c0_SCL_P02_5_INOUT,
        &IfxI2c0_SDA_P02_4_INOUT,
        IfxPort_PadDriver_cmosAutomotiveSpeed1
    };

    config.pins = &pins;                                    // Set configured Pins to config
    config.baudrate = I2C_FREQ;                             // Set I2C clock frequency to config

    IfxI2c_I2c_initModule(&i2c, &config);                   // Initialize module

    IfxI2c_I2c_deviceConfig i2cDeviceConfig;                // Create device config
    IfxI2c_I2c_initDeviceConfig(&i2cDeviceConfig, &i2c);    // Fill structure with default values and i2c Handler

    // Convert 7 bit device address into 8 bit value
    uint8 address = SENSOR_ADDRESS_7BIT;
    i2cDeviceConfig.deviceAddress = address << 1;

    IfxI2c_I2c_initDevice(&i2cDev, &i2cDeviceConfig);       // Initialize the i2c device handle
}


void do_soft_reset(void){
    data[0] = SOFT_RESET_COMMAND;   // Set command to write to data
    // Write command to device as soon as it is ready
    while(IfxI2c_I2c_write(&i2cDev, data, SIZE_SEND) == IfxI2c_I2c_Status_nak);
    // Wait after soft reset
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, RESET_WAIT_TIME));
}


uint32 read_serial_id(void){
    data[0] = READ_SERIAL_COMMAND;  // Set command to write to data
    // Write command to device as soon as it is ready
    while(IfxI2c_I2c_write(&i2cDev, data, SIZE_SEND) == IfxI2c_I2c_Status_nak);

    // Wait after command was sent before read is started
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, COMMAND_WAIT_TIME));

    // Read device data to data array
    while(IfxI2c_I2c_read(&i2cDev, data, SIZE_READ) == IfxI2c_I2c_Status_nak);

    // Extract serial id bytes from data, with byte 2 and 5 being for CRC and convert them to a 32 bit value
    uint32 serial_id = ((uint32)data[0]<<24) | ((uint32)data[1]<<16) | ((uint32)data[3]<<8) | (uint32)data[4];
    return serial_id;
}


void read_and_calculate_values(float* temperature, float* humidity){
    data[0] = READ_VALUES_COMMAND;  // set command to write to data
    // Write command to device as soon as it is ready
    while(IfxI2c_I2c_write(&i2cDev, data, SIZE_SEND) == IfxI2c_I2c_Status_nak);

    // Wait after command was sent before read is started
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, COMMAND_WAIT_TIME));

    // Read device data to data array
    while(IfxI2c_I2c_read(&i2cDev, data, SIZE_READ) == IfxI2c_I2c_Status_nak);

    // Transform 2 data bytes to 16 bit values for temperature and humidity, ignoring CRC
    uint16 temp_read = ((uint16)data[0]<<8) | (uint16)data[1];
    uint16 hum_read = ((uint16)data[3]<<8) | (uint16)data[4];

    // Convert 16 bit value to float with formulas from datasheet
    *temperature = CONVERSION_OFFSET_TEMP + CONVERSION_FACTOR_TEMP*((float)temp_read/(float)(TWO_POW_16-1));
    *humidity = CONVERSION_OFFSET_HUM + CONVERSION_FACTOR_HUM*((float)hum_read/(float)(TWO_POW_16-1));

    // Boundary check temperature
    if(*temperature < TEMPERATURE_MIN)
        *temperature = TEMPERATURE_MIN;
    else if(*temperature > TEMPERATURE_MAX)
        *temperature = TEMPERATURE_MAX;
    // Boundary check humidity
    if(*humidity < HUMIDITY_MIN)
        *humidity = TEMPERATURE_MIN;
    else if(*humidity > HUMIDITY_MAX)
        *humidity = HUMIDITY_MAX;
}
