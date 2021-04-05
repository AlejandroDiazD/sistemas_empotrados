################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/Source/portable/MemMang/heap_3.c 

OBJS += \
./FreeRTOS/Source/portable/MemMang/heap_3.o 

C_DEPS += \
./FreeRTOS/Source/portable/MemMang/heap_3.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/Source/portable/MemMang/%.o: ../FreeRTOS/Source/portable/MemMang/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/osboxes/workspace/FreeRtosPosixPlantControlEjercicio3Template/FreeRTOS/Source/include" -I"/home/osboxes/workspace/FreeRtosPosixPlantControlEjercicio3Template/FreeRTOS/Source/portable/GCC/Posix" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


