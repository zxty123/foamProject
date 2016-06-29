################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include vendor/common/subdir.mk
-include vendor/8368_tx/subdir.mk
-include proj/mcu/subdir.mk
-include proj/drivers/subdir.mk
-include proj/common/subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LST += \
8368_emi.lst \

FLASH_IMAGE += \
example_tx8368.bin \

SIZEDUMMY += \
sizedummy \


# All Target
all: pre-build main-build

# Main-build Target
main-build: Quadcopter.elf secondary-outputs

# Tool invocations
Quadcopter.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: TC32 C Linker'
	tc32-elf-ld --gc-sections -L"E:\foamProject\Source\foamTx\proj_lib" -L"E:\foamProject\Source\foamTx\vendor\txlib" -T ../boot.link -o"Quadcopter.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

8368_emi.lst: Quadcopter.elf
	@echo 'Invoking: TC32 Create Extended Listing'
	tc32-elf-objdump -x -D -l -S Quadcopter.elf  >"8368_emi.lst"
	@echo 'Finished building: $@'
	@echo ' '

example_tx8368.bin: Quadcopter.elf
	@echo 'Create Flash image (binary format)'
	tc32-elf-objcopy -O binary Quadcopter.elf  "example_tx8368.bin"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Quadcopter.elf
	@echo 'Invoking: Print Size'
	tc32-elf-size -t Quadcopter.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(FLASH_IMAGE)$(ELFS)$(LST)$(SIZEDUMMY) Quadcopter.elf
	-@echo ' '

pre-build:
	-E:\foamProject\Source\foamTx/getver.sh
	-@echo ' '

secondary-outputs: $(LST) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY: main-build pre-build

-include ../makefile.targets
