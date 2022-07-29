################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/_proj_btble_controller_/app.c \
../vendor/_proj_btble_controller_/app_ble.c \
../vendor/_proj_btble_controller_/app_bt.c \
../vendor/_proj_btble_controller_/app_irq.c \
../vendor/_proj_btble_controller_/main.c 

OBJS += \
./vendor/_proj_btble_controller_/app.o \
./vendor/_proj_btble_controller_/app_ble.o \
./vendor/_proj_btble_controller_/app_bt.o \
./vendor/_proj_btble_controller_/app_irq.o \
./vendor/_proj_btble_controller_/main.o 

C_DEPS += \
./vendor/_proj_btble_controller_/app.d \
./vendor/_proj_btble_controller_/app_ble.d \
./vendor/_proj_btble_controller_/app_bt.d \
./vendor/_proj_btble_controller_/app_irq.d \
./vendor/_proj_btble_controller_/main.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/_proj_btble_controller_/%.o: ../vendor/_proj_btble_controller_/%.c vendor/_proj_btble_controller_/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto  -g3 -DPROJ_BTBLE_CONTROLLER=1 -I"$(PWD)/../common" -I"$(PWD)/../drivers/B91" -I"$(PWD)/../incs" -I"$(PWD)/../stack" -I"$(PWD)/.." -c -fmessage-length=0 -fno-builtin -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -mext-dsp  -fpack-struct  -fno-delete-null-pointer-checks -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


