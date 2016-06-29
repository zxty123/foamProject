################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../proj/drivers/flash.c \
../proj/drivers/i2c.c \
../proj/drivers/pwm.c \
../proj/drivers/spi.c \
../proj/drivers/tl_i2c_master.c 

OBJS += \
./proj/drivers/flash.o \
./proj/drivers/i2c.o \
./proj/drivers/pwm.o \
./proj/drivers/spi.o \
./proj/drivers/tl_i2c_master.o 


# Each subdirectory must supply rules for building sources it contributes
proj/drivers/%.o: ../proj/drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -D__PROJECT_ZX32__=1 -Wall -Os -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


