################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include vendor/common/subdir.mk
-include vendor/_proj_btble_controller_/subdir.mk
-include stack/hci/dfu/subdir.mk
-include stack/hci/subdir.mk
-include drivers/B91/subdir.mk
-include common/usb_dbg/subdir.mk
-include common/subdir.mk
-include boot/9518/subdir.mk
-include algorithm/ecc/subdir.mk
-include algorithm/crypto/subdir.mk
-include algorithm/aes_ccm/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := _prj_btble_controller_
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
_prj_btble_controller_.bin \

SECONDARY_LIST += \
_prj_btble_controller_.lst \

SECONDARY_SIZE += \
_prj_btble_controller_.siz \


# All Target
all: main-build

# Main-build Target
main-build: _prj_btble_controller_.elf secondary-outputs

# Tool invocations
_prj_btble_controller_.elf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU RISC-V Cross C Linker'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto  -g3 -T ../boot.link -nostartfiles -Xlinker --gc-sections -L"$(PWD)/../proj_lib" -Wl,-Map,"_prj_btble_controller_.map" -mcmodel=medium -g3 -mcpu=d25f -ffunction-sections -fdata-sections -fmessage-length=0 -fno-builtin -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -mext-dsp -fpack-struct -fno-delete-null-pointer-checks  -O2 -o "_prj_btble_controller_.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

_prj_btble_controller_.bin: _prj_btble_controller_.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GNU RISC-V Cross Create Flash Image'
	riscv32-elf-objcopy -O binary "_prj_btble_controller_.elf"  "_prj_btble_controller_.bin"
	@echo 'Finished building: $@'
	@echo ' '

_prj_btble_controller_.lst: _prj_btble_controller_.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GNU RISC-V Cross Create Listing'
	riscv32-elf-objdump --source --all-headers --demangle --line-numbers --wide "_prj_btble_controller_.elf" > "_prj_btble_controller_.lst"
	@echo 'Finished building: $@'
	@echo ' '

_prj_btble_controller_.siz: _prj_btble_controller_.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GNU RISC-V Cross Print Size'
	riscv32-elf-size "_prj_btble_controller_.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) _prj_btble_controller_.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents main-build

-include ../makefile.targets
