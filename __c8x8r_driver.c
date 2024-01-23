/*
 * __c8x8r_driver.c
 *
 *  Created on: 08.01.2024
 *      Author: dieter stifter
 */
#include <Port/Io/IfxPo2rt_Io.h>
#include <IfxPort_PinMap.h>
#include "IfxQspi_SpiMaster.h"
#include <Bsp.h>
#include <stdio.h>
#include "__c8x8r_driver.h"
#include <stdio.h>
#include <string.h>
#include "hr_and_spo2_handler.h"
/*************************************************************************************************************/
/*-------------------------------------------------Global variables------------------------------------------*/
/*************************************************************************************************************/

IfxQspi_SpiMaster spi;
IfxQspi_SpiMaster_Channel spiChannel;



static uint8 big_heart_98_100[8] = {0x78,0xFC,0xFE,0x7F,0x7F,0xFE,0xFC,0x78};
static uint8 big_heart_96_98[8] = {0x78,0xFC,0xDE,0x6F,0x6F,0xDE,0xFC,0x78};
static uint8 big_heart_94_96[8] = {0x78,0xFC,0xCE,0x67,0x67,0xCE,0xFC,0x78};
static uint8 big_heart_94[8] = {0x78,0xFC,0xC6,0x63,0x63,0xC6,0xFC,0x78};

static uint8 small_heart_98_100[8] = {0x00,0x38,0x7C,0x3E,0x3E,0x7C,0x38,0x00};
static uint8 small_heart_96_98[8] = {0x00,0x38,0x5C,0x2E,0x2E,0x5C,0x38,0x00};
static uint8 small_heart_94_96[8] = {0x00,0x38,0x4C,0x26,0x26,0x4C,0x38,0x00};
static uint8 small_heart_94[8] = {0x00,0x38,0x44,0x22,0x22,0x44,0x38,0x00};

static uint8 def_pic[8] = {0x81,0x42,0x24, 0x18, 0x18, 0x24, 0x42,0x81};


static uint8 ascii_matrix[][10] = {

          { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0 },   // space -- 32
          { 0x00, 0x00, 0x0,  0x0,  0x60, 0xfa, 0xfa, 0x60, 0x0,  0x0 },   // ! -- 33
          { 0x00, 0x00, 0x0,  0xc0, 0xe0, 0x0,  0x0,  0xe0, 0xc0, 0x0 },   // " -- 34
          { 0x00, 0x00, 0x28, 0xfe, 0xfe, 0x28, 0xfe, 0xfe, 0x28, 0x0 },   // # -- 35
          { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0 },   // $ -- 36
          { 0x00, 0x00, 0x62, 0x66, 0xc,  0x18, 0x30, 0x66, 0x46, 0x0 },   // % -- 37
          { 0x00, 0x00, 0xc,  0x5e, 0xf2, 0xba, 0xec, 0x5e, 0x12, 0x0 },   // & -- 38
          { 0x00, 0x00, 0x0,  0x0,  0x20, 0xe0, 0xc0, 0x0,  0x0,  0x0 },   // ' -- 39
          { 0x00, 0x00, 0x0,  0x0,  0x38, 0x7c, 0xc6, 0x82, 0x0,  0x0 },   // ( -- 40
          { 0x00, 0x00, 0x0,  0x0,  0x82, 0xc6, 0x7c, 0x38, 0x0,  0x0 },   // ) -- 41
          { 0x00, 0x00, 0x10, 0x54, 0x7c, 0x38, 0x38, 0x7c, 0x54, 0x10},   // * -- 42
          { 0x00, 0x00, 0x0,  0x10, 0x10, 0x7c, 0x7c, 0x10, 0x10, 0x0 },   // + -- 43
          { 0x00, 0x00, 0x0,  0x0,  0x0,  0x18, 0x1c, 0x0,  0x0,  0x0 },   // , -- 44
          { 0x00, 0x00, 0x0,  0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x0 },   // - -- 45
          { 0x00, 0x00, 0x0,  0x0,  0x0,  0x6,  0x6,  0x0,  0x0,  0x0 },   // . -- 46
          { 0x00, 0x00, 0x6,  0xc,  0x18, 0x30, 0x60, 0xc0, 0x80, 0x0 },   // / -- 47

          { 0x00, 0x00, 0x7c, 0xfe, 0x8a, 0x92, 0xa2, 0xfe, 0x7c, 0x00},   // 0 -- 48
          { 0x00, 0x00, 0x0,  0x2,  0x42, 0xfe, 0xfe, 0x2,  0x2,  0x00},   // 1 -- 49
          { 0x00, 0x00, 0x42, 0xc6, 0x8e, 0x9a, 0x92, 0xf6, 0x66, 0x00},   // 2 -- 50
          { 0x00, 0x00, 0x22, 0x63, 0x49, 0x49, 0x49, 0x7F, 0x36, 0x00},   // 3 -- 51
          { 0x00, 0x00, 0x18, 0x38, 0x68, 0xca, 0xfe, 0xfe, 0xa,  0x0 },   // 4 -- 52
          { 0x00, 0x00, 0x0 , 0xf4, 0xf6, 0x92, 0x92, 0x92, 0x9e, 0x8c},   // 5 -- 53
          { 0x00, 0x00, 0x3c, 0x7e, 0xd2, 0x92, 0x92, 0x1e, 0xc,  0x00},   // 6 -- 54
          { 0x00, 0x00, 0x0,  0x0,  0x80, 0x9e, 0xb0, 0xe0, 0xc0, 0x0 },   // 7 -- 55
          { 0x00, 0x00, 0x6c, 0xfe, 0x92, 0x92, 0x92, 0xfe, 0x6c, 0x0 },   // 8 -- 56
          { 0x00, 0x00, 0x60, 0xf2, 0x92, 0x92, 0x96, 0xfc, 0x78, 0x0 },   // 9 -- 57//

          { 0x00, 0x00, 0x0,  0x0,  0x0,  0x66, 0x66, 0x0,  0x0,  0x0 },   // : -- 58
          { 0x00, 0x00, 0x0,  0x0,  0x0,  0x6c, 0x6e, 0x0,  0x0,  0x0 },   // ; -- 59
          { 0x00, 0x00, 0x0,  0x0,  0x10, 0x38, 0x6c, 0xc6, 0x82, 0x0 },   // < -- 60
          { 0x00, 0x00, 0x0,  0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x0 },   // = -- 61
          { 0x00, 0x00, 0x0,  0x82, 0xc6, 0x6c, 0x38, 0x10, 0x0,  0x0 },   // > -- 62
          { 0x00, 0x00, 0x40, 0xc0, 0x80, 0x9a, 0xba, 0xe0, 0x40, 0x0 },   // ? -- 63
          { 0x00, 0x00, 0x7c, 0xfe, 0x82, 0xba, 0xaa, 0xf8, 0x78, 0x0 },   // @ -- 64

          { 0x00, 0x00, 0x3e, 0x7e, 0xd0, 0x90, 0xd0, 0x7e, 0x3e, 0x0 },   // A -- 65
          { 0x00, 0x00, 0x82, 0xfe, 0xfe, 0x92, 0x92, 0xfe, 0x6c, 0x0 },   // B -- 66
          { 0x00, 0x00, 0x38, 0x7c, 0xc6, 0x82, 0x82, 0xc6, 0x44, 0x0 },   // C -- 67
          { 0x00, 0x00, 0x82, 0xfe, 0xfe, 0x82, 0xc6, 0x7c, 0x38, 0x0 },   // D -- 68
          { 0x00, 0x00, 0x82, 0xfe, 0xfe, 0x92, 0xba, 0x82, 0xc6, 0x0 },   // E -- 69
          { 0x00, 0x00, 0x82, 0xfe, 0xfe, 0x92, 0xb8, 0x80, 0xc0, 0x0 },   // F -- 70
          { 0x00, 0x00, 0x38, 0x7c, 0xc6, 0x82, 0x8a, 0xce, 0x4e, 0x0 },   // G -- 71
          { 0x00, 0x00, 0xfe, 0xfe, 0x10, 0x10, 0x10, 0xfe, 0xfe, 0x0 },   // H -- 72
          { 0x00, 0x00, 0x0,  0x0,  0x82, 0xfe, 0xfe, 0x82, 0x0,  0x0 },   // I -- 73
          { 0x00, 0x00, 0x0,  0xe,  0x2,  0x82, 0xfe, 0xfc, 0x80, 0x0 },   // J -- 74
          { 0x00, 0x00, 0x82, 0xfe, 0xfe, 0x10, 0x38, 0xee, 0xc6, 0x0 },   // K -- 75
          { 0x00, 0x00, 0x82, 0xfe, 0xfe, 0x82, 0x2,  0x6,  0xe,  0x0 },   // L -- 76
          { 0x00, 0x00, 0xfe, 0xfe, 0x70, 0x38, 0x70, 0xfe, 0xfe, 0x0 },   // M -- 77
          { 0x00, 0x00, 0xfe, 0xfe, 0x60, 0x30, 0x18, 0xfe, 0xfe, 0x0 },   // N -- 78
          { 0x00, 0x00, 0x7c, 0xfe, 0x82, 0x82, 0x82, 0xfe, 0x7c, 0x0 },   // O -- 79
          { 0x00, 0x00, 0x82, 0xfe, 0xfe, 0x92, 0x90, 0xf0, 0x60, 0x0 },   // P -- 80
          { 0x00, 0x00, 0x7c, 0xfe, 0x82, 0x82, 0x87, 0xff, 0x7d, 0x0 },   // Q -- 81
          { 0x00, 0x00, 0x82, 0xfe, 0xfe, 0x90, 0x98, 0xfe, 0x66, 0x0 },   // R -- 82
          { 0x00, 0x00, 0x44, 0xe6, 0xb2, 0x92, 0x9a, 0xce, 0x44, 0x0 },   // S -- 83
          { 0x00, 0x00, 0x0,  0xe0, 0xc2, 0xfe, 0xfe, 0xc2, 0xe0, 0x0 },   // T -- 84
          { 0x00, 0x00, 0xfc, 0xfe, 0x2,  0x2,  0x2,  0xfe, 0xfc, 0x0 },   // U -- 85
          { 0x00, 0x00, 0xf8, 0xfc, 0x6,  0x2,  0x6,  0xfc, 0xf8, 0x0 },   // V -- 86
          { 0x00, 0x00, 0xfc, 0xfe, 0x6,  0x1c, 0x6,  0xfe, 0xfc, 0x0 },   // W -- 87
          { 0x00, 0x00, 0xc6, 0xee, 0x38, 0x10, 0x38, 0xee, 0xc6, 0x0 },   // X -- 88
          { 0x00, 0x00, 0x0,  0xe0, 0xf2, 0x1e, 0x1e, 0xf2, 0xe0, 0x0 },   // Y -- 89
          { 0x00, 0x00, 0xe2, 0xc6, 0x8e, 0x92, 0xb2, 0xe6, 0xce, 0x0 },   // Z -- 90

          { 0x00, 0x00, 0x0,  0x0,  0xfe, 0xfe, 0x82, 0x82, 0x0,  0x0 },   // [ -- 91
          { 0x00, 0x00, 0x80, 0xc0, 0x60, 0x30, 0x18, 0xc,  0x6,  0x0 },   // \ -- 92
          { 0x00, 0x00, 0x0,  0x0,  0x82, 0x82, 0xfe, 0xfe, 0x0,  0x0 },   // ] -- 93
          { 0x00, 0x00, 0x10, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x10, 0x0 },   // ^ -- 94
          { 0x00, 0x00, 0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1,  0x1 },   // _ -- 95
          { 0x00, 0x00, 0x0,  0x0,  0x80, 0xc0, 0x60, 0x20, 0x0,  0x0 },   // ` -- 96{}

          { 0x00, 0x00, 0x4,  0x2e, 0x2a, 0x2a, 0x3c, 0x1e, 0x2,  0x0 },   // a -- 97
          { 0x00, 0x00, 0x82, 0xfe, 0xfc, 0x22, 0x22, 0x3e, 0x1c, 0x0 },   // b -- 98
          { 0x00, 0x00, 0x1c, 0x3e, 0x22, 0x22, 0x22, 0x36, 0x14, 0x0 },   // c -- 99
          { 0x00, 0x00, 0x1c, 0x3e, 0x22, 0xa2, 0xfc, 0xfe, 0x2,  0x0 },   // d -- 100
          { 0x00, 0x00, 0x1c, 0x3e, 0x2a, 0x2a, 0x2a, 0x3a, 0x18, 0x0 },   // e -- 101
          { 0x00, 0x00, 0x12, 0x7e, 0xfe, 0x92, 0x90, 0xc0, 0x40, 0x0 },   // f -- 102
          { 0x00, 0x00, 0x19, 0x3d, 0x25, 0x25, 0x3f, 0x3e, 0x20, 0x0 },   // g -- 103
          { 0x00, 0x00, 0x82, 0xfe, 0xfe, 0x30, 0x20, 0x3e, 0x1e, 0x0 },   // h -- 104
          { 0x00, 0x00, 0x0,  0x0,  0x22, 0xbe, 0xbe, 0x2,  0x0,  0x0 },   // i -- 105
          { 0x00, 0x00, 0x0,  0x6,  0x7,  0x1,  0x1,  0xbf, 0xbe, 0x0 },   // j -- 106
          { 0x00, 0x00, 0x82, 0xfe, 0xfe, 0x8,  0x1c, 0x36, 0x22, 0x0 },   // k -- 107
          { 0x00, 0x00, 0x0,  0x0,  0x82, 0xfe, 0xfe, 0x2,  0x0,  0x0 },   // l -- 108
          { 0x00, 0x00, 0x3e, 0x3e, 0x30, 0x1e, 0x30, 0x3e, 0x1e, 0x0 },   // m -- 109
          { 0x00, 0x00, 0x20, 0x3e, 0x1e, 0x20, 0x20, 0x3e, 0x1e, 0x0 },   // n -- 110
          { 0x00, 0x00, 0x1c, 0x3e, 0x22, 0x22, 0x22, 0x3e, 0x1c, 0x0 },   // o -- 111
          { 0x00, 0x00, 0x21, 0x3f, 0x1f, 0x25, 0x24, 0x3c, 0x18, 0x0 },   // p -- 112
          { 0x00, 0x00, 0x18, 0x3c, 0x24, 0x25, 0x1f, 0x3f, 0x21, 0x0 },   // q -- 113
          { 0x00, 0x00, 0x22, 0x3e, 0x1e, 0x32, 0x20, 0x30, 0x10, 0x0 },   // r -- 114
          { 0x00, 0x00, 0x12, 0x3a, 0x2a, 0x2a, 0x2a, 0x2e, 0x24, 0x0 },   // s -- 115
          { 0x00, 0x00, 0x20, 0x20, 0xfc, 0xfe, 0x22, 0x26, 0x4,  0x0 },   // t -- 116
          { 0x00, 0x00, 0x3c, 0x3e, 0x2,  0x2,  0x3c, 0x3e, 0x2,  0x0 },   // u -- 117
          { 0x00, 0x00, 0x38, 0x3c, 0x6,  0x3,  0x6,  0x3c, 0x38, 0x0 },   // v -- 118
          { 0x00, 0x00, 0x3c, 0x3e, 0x6,  0x1c, 0x6,  0x3e, 0x3c, 0x0 },   // w -- 119
          { 0x00, 0x00, 0x22, 0x36, 0x1c, 0x8,  0x1c, 0x36, 0x22, 0x0 },   // x -- 120
          { 0x00, 0x00, 0x39, 0x3e, 0x5,  0x5,  0x5,  0x3f, 0x3e, 0x0 },   // y -- 121
          { 0x00, 0x00, 0x0,  0x32, 0x26, 0x2e, 0x3a, 0x32, 0x26, 0x0 },
};



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

static uint8 _speedScroll = 3;


void get_globals(struct display_data *data){


    sint32 bpm = 0;
    uint8 spo2 = 90;

    interface_return_value_t oximeter_error = get_values(&spo2, &bpm);


    // Check for input parameters
    if(bpm < 35|| bpm > 150){
        data->bpm = 0;
    }else{
        data->bpm = (uint8)bpm;
    }
    if(spo2 < 50|| spo2 > 100){
        data->spo2 = 0;
    }else {
        data->spo2 = spo2;
    }
}


void change_images(struct display_data *data,uint8 *image_big, uint8 *image_small){

    if(data->bpm < 30)
    {
        memcpy(image_small, &def_pic, IMAGE_SIZE);
        memcpy(image_big, &def_pic, IMAGE_SIZE);
        return;
    }
    if(data->spo2 < 94){
        memcpy(image_small, &small_heart_94, IMAGE_SIZE);
        memcpy(image_big, &big_heart_94, IMAGE_SIZE);
    }else if(data->spo2 < 96){
        memcpy(image_small, &small_heart_94_96, IMAGE_SIZE);
        memcpy(image_big, &big_heart_94_96, IMAGE_SIZE);
    }else if(data->spo2 < 98){
        memcpy(image_small, &small_heart_96_98, IMAGE_SIZE);
        memcpy(image_big, &big_heart_96_98, IMAGE_SIZE);
    }else{
        memcpy(image_small, &small_heart_98_100, IMAGE_SIZE);
        memcpy(image_big, &big_heart_98_100, IMAGE_SIZE);
    }

    return;
}

uint32 c8x8r_getHeartFrequenz(uint8 bpm){
    // read the pulse (in beats per minute) sent over serial
    float pulse_BPM = bpm;

    // convert pulse to beats per second
    float pulse_BPS = pulse_BPM/60;

    // calculate the period of one beat
    float period = 1/pulse_BPS;

    // calculate floating point value of the time where the light should be on
    // assuming the systole lasts 3/8 of a pulse period
    float time_on_f = 0.375 * period;

    // convert that time on to an int in milliseconds
    int time_on = time_on_f * 1000;

    // calculate the remaining floating point time in the period - "the diastole"
    float time_off_f = period - time_on_f;

    // convert the time off to an int in milliseconds
    int time_off = time_off_f * 1000;

    uint32 timings = (uint16)time_on << 16 | (uint16)time_off;


    if(timings == 0)
    {
        timings = 500;
    }

    return timings;
}

void c8x8r_writeCmd(uint8 command, uint8 data)
{
    uint8 spiTxBuffer[2] = {0};
    spiTxBuffer[0] = command;
    spiTxBuffer[1] = data;
    while(IfxQspi_SpiMaster_getStatus(&spiChannel) == SpiIf_Status_busy );
    IfxQspi_SpiMaster_exchange(&spiChannel, &spiTxBuffer, NULL_PTR, 2);
}

static void _speed()
{
    switch(_speedScroll)
    {
        case 1:
           waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 200));
           break;
        case 2:
            waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 100));
           break;
        case 3:
            waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 30));
            break;
        default:
            waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 100));
            break;
    }
}



void c8x8r_displayRefresh()
{
   uint8 cnt;

   for( cnt = 1; cnt < 9; cnt++)
   {
        c8x8r_writeCmd(cnt,0x00);
   }
}

void c8x8r_setSpeedScroll(uint8 speed)
{
    _speedScroll = speed;
}

void c8x8r_displayString(char *pArray)
{
    uint8 str_word[512];
    char wrChr;
    uint8 cnt, i;
    uint8 global_cnt = 0;
    uint8 charAscii;
    uint8 position = 0;
    uint8 col = 8;

    for (i = 0; i < sizeof(pArray); i++)
    {
        charAscii = pArray[i] - 32;
        for( cnt = 0; cnt < 10; cnt++)
        {
            str_word[global_cnt++] = ascii_matrix[charAscii][cnt];
        }
    }

    while ( position < (global_cnt - 8) )
    {
         for( cnt = position; cnt < position + 8; cnt++)
         {
             wrChr = str_word[cnt];
             c8x8r_writeCmd(col, wrChr);
             col--;
         }
         col = 8;
         position++;
         _speed();
     }

}

void c8x8r_displayByte(char ch)
{
    uint8 cnt;
    uint8 charAscii;
    uint8 position = 8;
    char wrChr;

    charAscii = ch - 32;
    for( cnt = 2; cnt < 10; cnt++)
    {
        wrChr = ascii_matrix[charAscii][cnt];
        c8x8r_writeCmd(position,wrChr);
        position--;
    }
}

void c8x8r_displayImage(uint8 *pImage)
{
    uint8 cnt;
    uint8 line;
    uint8 position = 8;

    for(cnt = 0; cnt < 8; cnt++)
    {
        line = pImage[cnt];
        c8x8r_writeCmd(position,line);
        position--;
    }
}

void initSPI(){
    IfxQspi_SpiMaster_Config spiMasterConfig;
    IfxQspi_SpiMaster_initModuleConfig(&spiMasterConfig, &MODULE_QSPI1);

    // set the desired mode and maximum baudrate
    spiMasterConfig.base.mode             = SpiIf_Mode_master;
    spiMasterConfig.base.maximumBaudrate  = 100000;
    // ISR priorities and interrupt target
    spiMasterConfig.base.txPriority       = IFX_INTPRIO_QSPI0_TX;
    spiMasterConfig.base.rxPriority       = IFX_INTPRIO_QSPI0_RX;
    spiMasterConfig.base.erPriority       = IFX_INTPRIO_QSPI0_ER;
    spiMasterConfig.base.isrProvider      = IfxCpu_Irq_getTos(IfxCpu_getCoreIndex());

          // pin configuration
    const IfxQspi_SpiMaster_Pins pins = {
       &IfxQspi1_SCLK_P10_2_OUT, IfxPort_OutputMode_pushPull, // SCLK
       &IfxQspi1_MTSR_P10_3_OUT, IfxPort_OutputMode_pushPull, // MTSR
       &IfxQspi1_MRSTA_P10_1_IN, IfxPort_InputMode_pullDown,  // MRST
       IfxPort_PadDriver_cmosAutomotiveSpeed3 // pad driver mode
    };

    spiMasterConfig.pins = &pins;

    // initialize module
    //IfxQspi_SpiMaster spi; // defined globally
    IfxQspi_SpiMaster_initModule(&spi, &spiMasterConfig);

    // create channel config
    IfxQspi_SpiMaster_ChannelConfig spiMasterChannelConfig;
    IfxQspi_SpiMaster_initChannelConfig(&spiMasterChannelConfig, &spi);

    spiMasterChannelConfig.base.mode.clockPolarity = 1;
    spiMasterChannelConfig.base.mode.shiftClock=0;

    // set the baudrate for this channel
    spiMasterChannelConfig.base.baudrate = 50000;

    // select pin configuration
    const IfxQspi_SpiMaster_Output slsOutput = {
               &IfxQspi1_SLSO9_P10_5_OUT,
               IfxPort_OutputMode_pushPull,
               IfxPort_PadDriver_cmosAutomotiveSpeed1
           };
    spiMasterChannelConfig.sls.output = slsOutput;
    IfxQspi_SpiMaster_initChannel(&spiChannel, &spiMasterChannelConfig);

}

void c8x8r_default_cfg () {
    c8x8r_writeCmd(_C8X8R_DECODE_MODE_REG, _C8X8R_NO_DECODE );
    c8x8r_writeCmd(_C8X8R_INTENSITY_REG,   _C8X8R_INTENSITY_15 );
    c8x8r_writeCmd(_C8X8R_SCAN_LIMIT_REG,  _C8X8R_DISPLAY_DIGIT_0_7 );
    c8x8r_writeCmd(_C8X8R_SHUTDOWN_REG,    _C8X8R_NORMAL_OPERATION );

    c8x8r_setSpeedScroll( _C8X8R_SPEED_MEDIUM );
    c8x8r_displayRefresh();
}


void c8x8r_init(){
    initSPI();
    c8x8r_default_cfg();
}


