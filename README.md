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
A not filled heart corresponds to a value below 94% SpO2 and looks like this: <img src="https://github.com/AndreasRichie/MES_SW_Project2_Fitzko_Reichenauer_Stifter/assets/90688800/879bb90f-7c17-49a4-8eb6-ed8257706553" width="96" align="center">

A value between 94% and 96% SpO2 looks like this: <img src="https://github.com/AndreasRichie/MES_SW_Project2_Fitzko_Reichenauer_Stifter/assets/90688800/49ed1313-f028-4cf9-83a8-824a5104a355" width="96" align="center">

A value between 96% and 98% SpO2 looks like this: <img src="https://github.com/AndreasRichie/MES_SW_Project2_Fitzko_Reichenauer_Stifter/assets/90688800/26c26fc7-f36d-419a-91b3-079ff65a2d64" width="96" align="center">

Finally, above 98% SpO2 looks like this <img src="https://github.com/AndreasRichie/MES_SW_Project2_Fitzko_Reichenauer_Stifter/assets/90688800/b0999573-0757-4726-b2e0-ac9409e5784f" width="96" align="center">

The speed at which the heart beats corresponds directly to the hearts BPM. Via a medical function the diastolic and systolic values get calculated and via the duration of the "smaller" heart and the "bigger" heart visuallized.
For the actual values please use the UART communication. 


## 
