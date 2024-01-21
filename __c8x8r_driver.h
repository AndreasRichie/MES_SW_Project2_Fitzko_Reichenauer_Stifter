/*
 * __c8x8r_driver.h
 *
 *  Created on: 08.01.2024
 *      Author: dieter wieland
 */

#ifndef C8X8R_DRIVER_H_
#define C8X8R_DRIVER_H_

#include <Port/Io/IfxPort_Io.h>
#include <IfxPort_PinMap.h>
#include <IfxQspi_SpiMaster.h>
#include "stdint.h"

struct display_data{
        uint8_t bpm;
        uint8_t spo2;
};

#define T_C8X8R_P    const uint8_t*

#define SPI_BUFFER_SIZE             3
#define SPI_BUFFER_SIZE_CONSTANTS   30
#define IFX_INTPRIO_QSPI0_TX        5
#define IFX_INTPRIO_QSPI0_RX        6
#define IFX_INTPRIO_QSPI0_ER        8

#define IMAGE_SIZE                   8

extern IfxQspi_SpiMaster spi;
extern IfxQspi_SpiMaster_Channel spiChannel;

#define _C8X8R_SPEED_FAST    3
#define _C8X8R_SPEED_MEDIUM  2
#define _C8X8R_SPEED_SLOW    1

// Register Address Map
#define _C8X8R_DECODE_MODE_REG              0x09
#define  _C8X8R_INTENSITY_REG               0x0A
#define _C8X8R_SCAN_LIMIT_REG               0x0B
#define _C8X8R_SHUTDOWN_REG                 0x0C
#define _C8X8R_DISPLAY_TEST_REG             0x0F

// Decode-Mode Register

#define _C8X8R_NO_DECODE                    0x00
#define _C8X8R_DECODE_DIGIT_0               0x01
#define _C8X8R_DECODE_DIGIT_3_0             0x0F
#define _C8X8R_DECODE_DIGIT_7               0xFF

// Intensity Register Format

#define _C8X8R_INTENSITY_1                  0x00
#define _C8X8R_INTENSITY_3                  0x01
#define _C8X8R_INTENSITY_5                  0x02
#define _C8X8R_INTENSITY_7                  0x03
#define _C8X8R_INTENSITY_9                  0x04
#define _C8X8R_INTENSITY_11                 0x05
#define _C8X8R_INTENSITY_13                 0x06
#define _C8X8R_INTENSITY_15                 0x07
#define _C8X8R_INTENSITY_17                 0x08
#define _C8X8R_INTENSITY_19                 0x09
#define _C8X8R_INTENSITY_21                 0x0A
#define _C8X8R_INTENSITY_23                 0x0B
#define _C8X8R_INTENSITY_25                 0x0C
#define _C8X8R_INTENSITY_27                 0x0D
#define _C8X8R_INTENSITY_29                 0x0E
#define _C8X8R_INTENSITY_31                 0x0F

// Scan-Limit Register Format

#define _C8X8R_DISPLAY_DIGIT_0              0x00
#define _C8X8R_DISPLAY_DIGIT_0_1            0x01
#define _C8X8R_DISPLAY_DIGIT_0_2            0x02
#define _C8X8R_DISPLAY_DIGIT_0_3            0x03
#define _C8X8R_DISPLAY_DIGIT_0_4            0x04
#define _C8X8R_DISPLAY_DIGIT_0_5            0x05
#define _C8X8R_DISPLAY_DIGIT_0_6            0x06
#define _C8X8R_DISPLAY_DIGIT_0_7            0x07


// Shutdown Register Format

#define _C8X8R_SHUTDOWN_MODE                0x00
#define _C8X8R_NORMAL_OPERATION             0x01

//Display-Test Register Format

#define _C8X8R_DISPLAY_NORMAL_OPERATION    0x00
#define _C8X8R_DISPLAY_TEST_MODE           0X01



void c8x8r_writeCmd(uint8_t command, uint8_t _data);

/**
 * @brief Function for refresh display
 *
 * The function switches off all LEDs
 */
void c8x8r_displayRefresh();

/**
 * @brief Function for settings speed scroll display string
 *
 * @param[in] speed        Speed that will be set
 *
 * Options:
      Fast speed (30ms per character),
      Medium speed (100ms per character) - default speed,
      Slow speed ( 200ms per character),
 */
void c8x8r_setSpeedScroll(uint8_t speed);

/**
 * @brief Function for displaying one character
 *
 * @param[in] ch        Character to be displayed
 *
 */
void c8x8r_displayByte(char ch);

/**
 * @brief Scroll string function
 *
 * @param[in] pArray       Pointer to the string to be displayed
 *
 * Function that displays scrolled string with set speed.
   If the speed is not set before calling the function,
   default scroll speed is 100ms per character.
 */
void c8x8r_displayString(char *pArray);

/**
 * @brief Image display function
 *
 * @param[in] pArray       Pointer to the image to be displayed
 *
 * Function for displays the image.
   The image consists of eight elements (eight columns that build the image).
 */
void c8x8r_displayImage(uint8_t *pArray);

/**
 * @brief Init function
 *
 * Function to init the basic functions of the display
 */
void c8x8r_init();

/**
 * @brief Calculates Blinking frequenz
 *
 * Calculates Blinking frequenz according to heart frequency
 */
uint32 c8x8r_getHeartFrequenz(uint8 bpm);

/**
 * @brief get the globals(bpm and spo2) from an external source
 */
void get_globals(struct display_data *data);


/**
 * @brief changes image according to values
 *
 * changes image according to values which are bpm and spo2. For an invalid value a default image will be displayed.
 *
 */
void change_images(struct display_data *data, uint8 *image_big, uint8 *image_small);

#endif /* C8X8R_DRIVER_H_ */
