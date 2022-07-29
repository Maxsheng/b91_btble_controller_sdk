################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../stack/hci/bt_hci.c \
../stack/hci/hci.c \
../stack/hci/hci_cmd.c \
../stack/hci/hci_event.c \
../stack/hci/hci_transport.c 

OBJS += \
./stack/hci/bt_hci.o \
./stack/hci/hci.o \
./stack/hci/hci_cmd.o \
./stack/hci/hci_event.o \
./stack/hci/hci_transport.o 

C_DEPS += \
./stack/hci/bt_hci.d \
./stack/hci/hci.d \
./stack/hci/hci_cmd.d \
./stack/hci/hci_event.d \
./stack/hci/hci_transport.d 


# Each subdirectory must supply rules for building sources it contributes
stack/hci/%.o: ../stack/hci/%.c stack/hci/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto  -g3 -DPROJ_BTBLE_CONTROLLER=1 -I"$(PWD)/../common" -I"$(PWD)/../drivers/B91" -I"$(PWD)/../incs" -I"$(PWD)/../stack" -I"$(PWD)/.." -c -fmessage-length=0 -fno-builtin -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -mext-dsp  -fpack-struct  -fno-delete-null-pointer-checks -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


