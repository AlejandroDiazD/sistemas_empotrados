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
	gcc -I"/home/osboxes/workspace/FreeRtosPosixPlantControlEjercicio3Template/FreeRTOS/Source/include" -I"/home/osboxes/workspace/FreeRtosPosixPlantControlEjercicio3Template/FreeRTOS/Source/portable/GCC/Posix" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


