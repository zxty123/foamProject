################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include vendor/txlib/subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LST += \

FLASH_IMAGE += \

SIZEDUMMY += \
sizedummy \


# All Target
all: pre-build main-build

# Main-build Target
main-build: libtxlib.a secondary-outputs

# Tool invocations
libtxlib.a: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: TC32 Archiver'
	tc32-elf-ar -r "libtxlib.a" $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

: libtxlib.a
	@echo 'Invoking: TC32 Create Extended Listing'
	tc32-elf-objdump libtxlib.a  >
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: libtxlib.a
	@echo 'Invoking: Print Size'
	tc32-elf-size -t libtxlib.a
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(ARCHIVES)$(FLASH_IMAGE)$(LST)$(SIZEDUMMY) libtxlib.a
	-@echo ' '

pre-build:
	-E:\zxty\project\x5\Quadcopter\Quadcopter/getver.sh
	-@echo ' '

secondary-outputs: $(LST) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY: main-build pre-build

-include ../makefile.targets
