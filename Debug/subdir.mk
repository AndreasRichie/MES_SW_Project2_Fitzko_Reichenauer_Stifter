################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Cpu0_Main.c \
../Cpu1_Main.c \
../Cpu2_Main.c \
../STM_Interrupt.c \
../UART.c \
../__c8x8r_driver.c \
../hr_and_spo2_handler.c \
../oximeter5_click.c \
../timer.c 

OBJS += \
./Cpu0_Main.o \
./Cpu1_Main.o \
./Cpu2_Main.o \
./STM_Interrupt.o \
./UART.o \
./__c8x8r_driver.o \
./hr_and_spo2_handler.o \
./oximeter5_click.o \
./timer.o 

COMPILED_SRCS += \
./Cpu0_Main.src \
./Cpu1_Main.src \
./Cpu2_Main.src \
./STM_Interrupt.src \
./UART.src \
./__c8x8r_driver.src \
./hr_and_spo2_handler.src \
./oximeter5_click.src \
./timer.src 

C_DEPS += \
./Cpu0_Main.d \
./Cpu1_Main.d \
./Cpu2_Main.d \
./STM_Interrupt.d \
./UART.d \
./__c8x8r_driver.d \
./hr_and_spo2_handler.d \
./oximeter5_click.d \
./timer.d 


# Each subdirectory must supply rules for building sources it contributes
%.src: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc27xd "-fC:/Users/dieter wieland/Documents/GitHub/MES_SW_Project2_Fitzko_Reichenauer_Stifter/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc27xd -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

%.o: ./%.src subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


