# My heart beats for multicore

## Overview

This project uses the Oximeter 5 Click [https://www.mikroe.com/oximeter-5-click] and the 8x8 R-Click [https://www.mikroe.com/8x8-r-click] to messure, calculate and display the heartrate and SpO2 values of the user.
Running the project on an Aurix Tricore Microprocessor utilizes all 3 cores of the system. 
Additionally, you can view the values in more detail via UART.

## Core Setup

### CPU0
This core handles the communication with LED-Matrix display via SPI

### CPU1
This core handles the sensor communication via I²C

### CPU2
This core handles UART communication

## What happens at runtime

## All functions

### Display values

How much or how little the heart is filled informes the user about the approximate SpO2 value.
A not filled heart means a value below XXXXXX % and looks like this:

<img src="https://github.com/AndreasRichie/MES_SW_Project2_Fitzko_Reichenauer_Stifter/assets/90688800/879bb90f-7c17-49a4-8eb6-ed8257706553" width="48">

A heart which is filled a little corresponds to a value between XXXX % and XXXXX %
![image](https://github.com/AndreasRichie/MES_SW_Project2_Fitzko_Reichenauer_Stifter/assets/90688800/49ed1313-f028-4cf9-83a8-824a5104a355)

![image](https://github.com/AndreasRichie/MES_SW_Project2_Fitzko_Reichenauer_Stifter/assets/90688800/b0999573-0757-4726-b2e0-ac9409e5784f)



## 
