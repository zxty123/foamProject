################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/8368_tx/key_scan.c \
../vendor/8368_tx/remote_tx.c 

OBJS += \
./vendor/8368_tx/key_scan.o \
./vendor/8368_tx/remote_tx.o 


# Each subdirectory must supply rules for building sources it contributes
vendor/8368_tx/%.o: ../vendor/8368_tx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -D__PROJECT_TX8368__=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


