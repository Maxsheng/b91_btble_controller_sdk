################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../common/co_list.c \
../common/co_printf.c \
../common/co_utils.c \
../common/fifo.c \
../common/my_debug.c \
../common/printf.c \
../common/string.c \
../common/sys_timer.c \
../common/timer_event.c \
../common/utility.c 

OBJS += \
./common/co_list.o \
./common/co_printf.o \
./common/co_utils.o \
./common/fifo.o \
./common/my_debug.o \
./common/printf.o \
./common/string.o \
./common/sys_timer.o \
./common/timer_event.o \
./common/utility.o 

C_DEPS += \
./common/co_list.d \
./common/co_printf.d \
./common/co_utils.d \
./common/fifo.d \
./common/my_debug.d \
./common/printf.d \
./common/string.d \
./common/sys_timer.d \
./common/timer_event.d \
./common/utility.d 


# Each subdirectory must supply rules for building sources it contributes
common/%.o: ../common/%.c common/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU RISC-V Cross C Compiler'
	riscv32-elf-gcc -mcmodel=medium -mcmodel=medium -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -flto  -g3 -DPROJ_BTBLE_CONTROLLER=1 -I"$(PWD)/../common" -I"$(PWD)/../drivers/B91" -I"$(PWD)/../incs" -I"$(PWD)/../stack" -I"$(PWD)/.." -c -fmessage-length=0 -fno-builtin -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -mext-dsp  -fpack-struct  -fno-delete-null-pointer-checks -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


