################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../algorithm/ecc/ecc_ll.c \
../algorithm/ecc/hw_ecc.c \
../algorithm/ecc/sw_ecc.c 

OBJS += \
./algorithm/ecc/ecc_ll.o \
./algorithm/ecc/hw_ecc.o \
./algorithm/ecc/sw_ecc.o 

C_DEPS += \
./algorithm/ecc/ecc_ll.d \
./algorithm/ecc/hw_ecc.d \
./algorithm/ecc/sw_ecc.d 


# Each subdirectory must supply rules for building sources it contributes
algorithm/ecc/%.o: ../algorithm/ecc/%.c algorithm/ecc/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto  -g3 -DPROJ_BTBLE_CONTROLLER=1 -I"$(PWD)/../common" -I"$(PWD)/../drivers/B91" -I"$(PWD)/../incs" -I"$(PWD)/../stack" -I"$(PWD)/.." -c -fmessage-length=0 -fno-builtin -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -mext-dsp  -fpack-struct  -fno-delete-null-pointer-checks -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


