# My heart beats for multicore

## Overview

This project uses the Oximeter 5 Click [https://www.mikroe.com/oximeter-5-click] and the 8x8 R-Click [https://www.mikroe.com/8x8-r-click] to messure, calculate and display the heartrate and SpO2 values of the user.
Running the project on an Aurix Tricore Microprocessor utilizes all 3 cores of the system. 
Additionally, you can view the values in more detail via UART.<br>
<img src="https://github.com/AndreasRichie/MES_SW_Project2_Fitzko_Reichenauer_Stifter/assets/90688800/c7414ffd-eb68-48a9-a421-90e697d788b6" width="300">
<br>
Overview of the setup. The X indicates, that no finger is on the sensor.

<img src="https://github.com/AndreasRichie/MES_SW_Project2_Fitzko_Reichenauer_Stifter/assets/90688800/01c5d756-a73a-4ca3-aa14-d299d088ce9d" width="300"><br>
After a finger is placed on the sensor, it starts messuring the heartrate and SpO2. 

<img src="https://github.com/AndreasRichie/MES_SW_Project2_Fitzko_Reichenauer_Stifter/assets/90688800/c872af60-918d-40cf-a90a-f887ead0ba17" width="300"><br>
If the MC is connected to a PC with hterm, the values can be viewed in more detail.

## Core Setup
<img src="https://github.com/AndreasRichie/MES_SW_Project2_Fitzko_Reichenauer_Stifter/assets/90688800/65164936-17fd-4c8e-a28d-55fe8c933e48" align="center">

### CPU0
This core handles the communication with the LED-Matrix display via SPI.

### CPU1
This core handles the sensor communication via I²C.

### CPU2
This core handles the UART communication.

## What happens at runtime

After all the cores are initialized, CPU1 gathers data from the sensor and saves the calculated SpO2 and pulse values in global variables every 100ms. 
When CPU0 and CPU2 are ready they make a check if they can "grab" the sensor data. To prevent the simultanious access of multiple cores on one variable, the global variables are protected by a CPU Mutex.
If the value retrieving was successful, CPU0 uses the data to vizualise it on the 8x8 LED Matrix. 
The higher the pulse, the faster the heart blinks ("beats") on the 8x8 matrix. In the middle of the heart is space to visualize the SpO2 value. A completly filled heart means SpO2 above 98%. More info about the different filled states under "Display Values".

CPU2 has a timer interrup every second. In this ISR it checks the global sensor variables as well and if the values are returned successfully it generates a timestamp of the passed time and then sends the time stamp and the data via UART to the user. 
This happens periodically.

## All functions

### The Sensor

<img src="https://github.com/AndreasRichie/MES_SW_Project2_Fitzko_Reichenauer_Stifter/assets/90688800/aab4536a-0ca0-4613-aac4-9ed3ea96cd45" align="center">

The sensor works by messuring the brightnesses of the reflected infrared and red light, which are emitted by respective LEDs. After init it reads the current light levels and saves them to a buffer. After a few of these readings it starts calculating the BPM and SpO2 with bio magic. The calculated values are then saved into global variables so that the other cores can read them too. Meanwhile, it continues reading lightlevels and overwrites old values. 
This means the sensor is continuously reading and calculating. 
In case of a calculation or saving error, it just retries until it works again. If there is a error happening in the sensor communication, the CPU0 waits for 5 seconds and then tries another measurement.

### Display values
<img src="https://github.com/AndreasRichie/MES_SW_Project2_Fitzko_Reichenauer_Stifter/assets/90688800/8f1ff79a-3c46-4f2a-ad1b-2f4766c8f4dd" align="center">

How much or how little the heart is filled informs the user about the approximate SpO2 value.
A not filled heart corresponds to a value below 94% SpO2 and looks like this: <img src="https://github.com/AndreasRichie/MES_SW_Project2_Fitzko_Reichenauer_Stifter/assets/90688800/879bb90f-7c17-49a4-8eb6-ed8257706553" width="96" align="center">

A value between 94% and 96% SpO2 looks like this: <img src="https://github.com/AndreasRichie/MES_SW_Project2_Fitzko_Reichenauer_Stifter/assets/90688800/49ed1313-f028-4cf9-83a8-824a5104a355" width="96" align="center">

A value between 96% and 98% SpO2 looks like this: <img src="https://github.com/AndreasRichie/MES_SW_Project2_Fitzko_Reichenauer_Stifter/assets/90688800/26c26fc7-f36d-419a-91b3-079ff65a2d64" width="96" align="center">

Finally, above 98% SpO2 looks like this <img src="https://github.com/AndreasRichie/MES_SW_Project2_Fitzko_Reichenauer_Stifter/assets/90688800/b0999573-0757-4726-b2e0-ac9409e5784f" width="96" align="center">

The speed at which the heart beats corresponds directly to the calculated pulse. Via a medical function the heartbeat is split into a diastolic and systolic part respectively which is visualized by the duration of the "smaller" heart and the "bigger" heart.
For the actual values please use the UART communication. 

### UART 

<img src="https://github.com/AndreasRichie/MES_SW_Project2_Fitzko_Reichenauer_Stifter/assets/90688800/66ae1569-75d9-4d3b-b191-162c15c71b14" align="center">

When connected to hterm the sensor values get posted every second via UART in this format: 
[xxh:xxm:xxs] xxBPM, xx%SpO2,

