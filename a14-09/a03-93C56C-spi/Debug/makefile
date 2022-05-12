################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
a03-93C56C-spi.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
a03-93C56C-spi.list \

OBJCOPY_BIN += \
a03-93C56C-spi.bin \


# All Target
all: a03-93C56C-spi.elf secondary-outputs

# Tool invocations
a03-93C56C-spi.elf: $(OBJS) $(USER_OBJS) C:\Users\user\Downloads\Visual-C\a14-09\a03-93C56C-spi\STM32F103RBTX_FLASH.ld
	arm-none-eabi-gcc -o "a03-93C56C-spi.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\user\Downloads\Visual-C\a14-09\a03-93C56C-spi\STM32F103RBTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="a03-93C56C-spi.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

a03-93C56C-spi.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "a03-93C56C-spi.list"
	@echo 'Finished building: $@'
	@echo ' '

a03-93C56C-spi.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "a03-93C56C-spi.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
