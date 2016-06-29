################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/lib/comparator.c \
../vendor/lib/mathft.c \
../vendor/lib/motor.c \
../vendor/lib/ota_sample.c \
../vendor/lib/uart.c 

OBJS += \
./vendor/lib/comparator.o \
./vendor/lib/mathft.o \
./vendor/lib/motor.o \
./vendor/lib/ota_sample.o \
./vendor/lib/uart.o 


# Each subdirectory must supply rules for building sources it contributes
vendor/lib/%.o: ../vendor/lib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -D__PROJECT_ZX32__=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


