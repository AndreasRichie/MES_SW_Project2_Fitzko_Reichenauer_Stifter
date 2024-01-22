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

## 
