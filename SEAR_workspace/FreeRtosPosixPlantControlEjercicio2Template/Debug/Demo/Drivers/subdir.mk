################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Demo/Drivers/gpio.c \
../Demo/Drivers/interrupts.c 

OBJS += \
./Demo/Drivers/gpio.o \
./Demo/Drivers/interrupts.o 

C_DEPS += \
./Demo/Drivers/gpio.d \
./Demo/Drivers/interrupts.d 


# Each subdirectory must supply rules for building sources it contributes
Demo/Drivers/%.o: ../Demo/Drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/user/workspace/FreeRtosDemoPosix/Demo" -I"/home/user/workspace/FreeRtosDemoPosix/Demo/Drivers" -I"/home/user/workspace/FreeRtosDemoPosix/FreeRTOS/Source/include" -I"/home/user/workspace/FreeRtosDemoPosix/FreeRTOS/Source/portable/GCC/Posix" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


