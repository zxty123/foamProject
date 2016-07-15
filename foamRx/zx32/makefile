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
main-build: foamRx1.elf secondary-outputs

# Tool invocations
foamRx1.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: TC32 C Linker'
	tc32-elf-ld --gc-sections -L"C:\Users\PengYF\Desktop\Ò£¿Ø·ÉÐÐÆ÷\yfpeng\20160619Plane\Rx\20160715\foamRx\proj_lib" -L"C:\Users\PengYF\Desktop\Ò£¿Ø·ÉÐÐÆ÷\yfpeng\20160619Plane\Rx\20160715\foamRx\vendor\lib" -T ../boot.link -o"foamRx1.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

boot-.lst: foamRx1.elf
	@echo 'Invoking: TC32 Create Extended Listing'
	tc32-elf-objdump -x -D -l -S foamRx1.elf  >"boot-.lst"
	@echo 'Finished building: $@'
	@echo ' '

: foamRx1.elf
	@echo 'Create Flash image (binary format)'
	foamRx1.elf
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: foamRx1.elf
	@echo 'Invoking: Print Size'
	tc32-elf-size -t foamRx1.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(FLASH_IMAGE)$(ELFS)$(LST)$(SIZEDUMMY) foamRx1.elf
	-@echo ' '

pre-build:
	-C:\Users\PengYF\Desktop\Ò£¿Ø·ÉÐÐÆ÷\yfpeng\20160619Plane\Rx\20160715\foamRx/getver.sh
	-@echo ' '

post-build:
	-C:\Users\PengYF\Desktop\Ò£¿Ø·ÉÐÐÆ÷\yfpeng\20160619Plane\Rx\20160715\foamRx/proc.bat
	-@echo ' '

secondary-outputs: $(LST) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY: main-build pre-build post-build

-include ../makefile.targets
