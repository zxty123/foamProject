################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/example/example.c \
../vendor/example/flashCtrl.c \
../vendor/example/imu.c \
../vendor/example/pid.c \
../vendor/example/sensor.c 

OBJS += \
./vendor/example/example.o \
./vendor/example/flashCtrl.o \
./vendor/example/imu.o \
./vendor/example/pid.o \
./vendor/example/sensor.o 


# Each subdirectory must supply rules for building sources it contributes
vendor/example/%.o: ../vendor/example/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -D__PROJECT_ZX32__=1 -Wall -Os -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


