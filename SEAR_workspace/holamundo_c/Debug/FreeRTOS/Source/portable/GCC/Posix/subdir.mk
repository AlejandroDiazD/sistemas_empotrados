################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/Source/portable/GCC/Posix/port.c 

OBJS += \
./FreeRTOS/Source/portable/GCC/Posix/port.o 

C_DEPS += \
./FreeRTOS/Source/portable/GCC/Posix/port.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/Source/portable/GCC/Posix/%.o: ../FreeRTOS/Source/portable/GCC/Posix/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/adidi/SEAR_workspace/holamundo_c/FreeRTOS/Source/include" -I"/home/adidi/SEAR_workspace/holamundo_c/FreeRTOS/Source/portable/GCC/Posix" -I"/home/adidi/SEAR_workspace/holamundo_c/FreeRTOS/Source/portable/MemMang" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


