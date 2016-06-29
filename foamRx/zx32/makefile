################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include vendor/example/subdir.mk
-include vendor/common/subdir.mk
-include proj/mcu/subdir.mk
-include proj/drivers/subdir.mk
-include proj/common/subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LST += \
boot-.lst \

FLASH_IMAGE += \

SIZEDUMMY += \
sizedummy \


# All Target
all: pre-build main-build

# Main-build Target
main-build: zxty_SDK.elf secondary-outputs

# Tool invocations
zxty_SDK.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: TC32 C Linker'
	tc32-elf-ld --gc-sections -L"E:\foamProject\foamProject.git\trunk\foamRx\proj_lib" -L"E:\foamProject\foamProject.git\trunk\foamRx\vendor\lib" -T ../boot.link -o"zxty_SDK.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

boot-.lst: zxty_SDK.elf
	@echo 'Invoking: TC32 Create Extended Listing'
	tc32-elf-objdump -x -D -l -S zxty_SDK.elf  >"boot-.lst"
	@echo 'Finished building: $@'
	@echo ' '

: zxty_SDK.elf
	@echo 'Create Flash image (binary format)'
	zxty_SDK.elf
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: zxty_SDK.elf
	@echo 'Invoking: Print Size'
	tc32-elf-size -t zxty_SDK.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(FLASH_IMAGE)$(ELFS)$(LST)$(SIZEDUMMY) zxty_SDK.elf
	-@echo ' '

pre-build:
	-E:\foamProject\foamProject.git\trunk\foamRx/getver.sh
	-@echo ' '

post-build:
	-E:\foamProject\foamProject.git\trunk\foamRx/proc.bat
	-@echo ' '

secondary-outputs: $(LST) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY: main-build pre-build post-build

-include ../makefile.targets
