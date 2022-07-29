################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../algorithm/aes_ccm/aes_ccm.c 

OBJS += \
./algorithm/aes_ccm/aes_ccm.o 

C_DEPS += \
./algorithm/aes_ccm/aes_ccm.d 


# Each subdirectory must supply rules for building sources it contributes
algorithm/aes_ccm/%.o: ../algorithm/aes_ccm/%.c algorithm/aes_ccm/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto  -g3 -DPROJ_BTBLE_CONTROLLER=1 -I"$(PWD)/../common" -I"$(PWD)/../drivers/B91" -I"$(PWD)/../incs" -I"$(PWD)/../stack" -I"$(PWD)/.." -c -fmessage-length=0 -fno-builtin -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -mext-dsp  -fpack-struct  -fno-delete-null-pointer-checks -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


