################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include vendor/lib/subdir.mk
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
main-build: libzx32lib.a secondary-outputs

# Tool invocations
libzx32lib.a: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: TC32 Archiver'
	tc32-elf-ar -r "libzx32lib.a" $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

: libzx32lib.a
	@echo 'Invoking: TC32 Create Extended Listing'
	libzx32lib.a  >
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: libzx32lib.a
	@echo 'Invoking: Print Size'
	tc32-elf-size -t libzx32lib.a
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(ARCHIVES)$(FLASH_IMAGE)$(LST)$(SIZEDUMMY) libzx32lib.a
	-@echo ' '

pre-build:
	-E:\zxty\project\x5\rx\zxty_sdk/getver.sh
	-@echo ' '

secondary-outputs: $(LST) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY: main-build pre-build

-include ../makefile.targets
