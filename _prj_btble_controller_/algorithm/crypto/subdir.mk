################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../algorithm/crypto/crypto_alg.c 

OBJS += \
./algorithm/crypto/crypto_alg.o 

C_DEPS += \
./algorithm/crypto/crypto_alg.d 


# Each subdirectory must supply rules for building sources it contributes
algorithm/crypto/%.o: ../algorithm/crypto/%.c algorithm/crypto/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto  -g3 -DPROJ_BTBLE_CONTROLLER=1 -I"$(PWD)/../common" -I"$(PWD)/../drivers/B91" -I"$(PWD)/../incs" -I"$(PWD)/../stack" -I"$(PWD)/.." -c -fmessage-length=0 -fno-builtin -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -mext-dsp  -fpack-struct  -fno-delete-null-pointer-checks -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


