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
a14-dht11-gpio-counter.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
a14-dht11-gpio-counter.list \

OBJCOPY_BIN += \
a14-dht11-gpio-counter.bin \


# All Target
all: a14-dht11-gpio-counter.elf secondary-outputs

# Tool invocations
a14-dht11-gpio-counter.elf: $(OBJS) $(USER_OBJS) C:\Users\user\Downloads\Visual-C\a14-16\a14-dht11-gpio-counter\STM32F103RBTX_FLASH.ld
	arm-none-eabi-gcc -o "a14-dht11-gpio-counter.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\user\Downloads\Visual-C\a14-16\a14-dht11-gpio-counter\STM32F103RBTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="a14-dht11-gpio-counter.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

a14-dht11-gpio-counter.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "a14-dht11-gpio-counter.list"
	@echo 'Finished building: $@'
	@echo ' '

a14-dht11-gpio-counter.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "a14-dht11-gpio-counter.bin"
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
