################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/common/app_hw_timer.c \
../vendor/common/app_irq_common.c \
../vendor/common/app_sw_timer.c \
../vendor/common/app_timer.c \
../vendor/common/ble_custom_pair.c \
../vendor/common/ble_device_manage.c \
../vendor/common/ble_simple_sdp.c \
../vendor/common/blt_common.c \
../vendor/common/bt_dbg.c 

OBJS += \
./vendor/common/app_hw_timer.o \
./vendor/common/app_irq_common.o \
./vendor/common/app_sw_timer.o \
./vendor/common/app_timer.o \
./vendor/common/ble_custom_pair.o \
./vendor/common/ble_device_manage.o \
./vendor/common/ble_simple_sdp.o \
./vendor/common/blt_common.o \
./vendor/common/bt_dbg.o 

C_DEPS += \
./vendor/common/app_hw_timer.d \
./vendor/common/app_irq_common.d \
./vendor/common/app_sw_timer.d \
./vendor/common/app_timer.d \
./vendor/common/ble_custom_pair.d \
./vendor/common/ble_device_manage.d \
./vendor/common/ble_simple_sdp.d \
./vendor/common/blt_common.d \
./vendor/common/bt_dbg.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/common/%.o: ../vendor/common/%.c vendor/common/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto  -g3 -DPROJ_BTBLE_CONTROLLER=1 -I"$(PWD)/../common" -I"$(PWD)/../drivers/B91" -I"$(PWD)/../incs" -I"$(PWD)/../stack" -I"$(PWD)/.." -c -fmessage-length=0 -fno-builtin -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -mext-dsp  -fpack-struct  -fno-delete-null-pointer-checks -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


