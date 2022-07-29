################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../common/usb_dbg/myudb_usb.c \
../common/usb_dbg/myudb_usbdesc.c 

OBJS += \
./common/usb_dbg/myudb_usb.o \
./common/usb_dbg/myudb_usbdesc.o 

C_DEPS += \
./common/usb_dbg/myudb_usb.d \
./common/usb_dbg/myudb_usbdesc.d 


# Each subdirectory must supply rules for building sources it contributes
common/usb_dbg/%.o: ../common/usb_dbg/%.c common/usb_dbg/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto  -g3 -DPROJ_BTBLE_CONTROLLER=1 -I"$(PWD)/../common" -I"$(PWD)/../drivers/B91" -I"$(PWD)/../incs" -I"$(PWD)/../stack" -I"$(PWD)/.." -c -fmessage-length=0 -fno-builtin -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -mext-dsp  -fpack-struct  -fno-delete-null-pointer-checks -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


