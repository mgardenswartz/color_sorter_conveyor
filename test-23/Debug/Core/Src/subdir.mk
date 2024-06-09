################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/ColoredBlock.cpp \
../Core/Src/ConveyorBelt.cpp \
../Core/Src/MotorControl.cpp \
../Core/Src/PololuEncoder.cpp \
../Core/Src/PololuServo.cpp \
../Core/Src/RCChannel.cpp \
../Core/Src/RemoteControlControl.cpp \
../Core/Src/TB6612FNG_Motor.cpp \
../Core/Src/VL53L0X.cpp \
../Core/Src/main.cpp 

C_SRCS += \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c 

C_DEPS += \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d 

OBJS += \
./Core/Src/ColoredBlock.o \
./Core/Src/ConveyorBelt.o \
./Core/Src/MotorControl.o \
./Core/Src/PololuEncoder.o \
./Core/Src/PololuServo.o \
./Core/Src/RCChannel.o \
./Core/Src/RemoteControlControl.o \
./Core/Src/TB6612FNG_Motor.o \
./Core/Src/VL53L0X.o \
./Core/Src/main.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o 

CPP_DEPS += \
./Core/Src/ColoredBlock.d \
./Core/Src/ConveyorBelt.d \
./Core/Src/MotorControl.d \
./Core/Src/PololuEncoder.d \
./Core/Src/PololuServo.d \
./Core/Src/RCChannel.d \
./Core/Src/RemoteControlControl.d \
./Core/Src/TB6612FNG_Motor.d \
./Core/Src/VL53L0X.d \
./Core/Src/main.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/VL53L0X/platform/src -I../Drivers/VL53L0X/platform/inc -I../Drivers/VL53L0X/core/inc -I../Drivers/VL53L0X/core/src -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/VL53L0X/platform/src -I../Drivers/VL53L0X/platform/inc -I../Drivers/VL53L0X/core/inc -I../Drivers/VL53L0X/core/src -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/ColoredBlock.cyclo ./Core/Src/ColoredBlock.d ./Core/Src/ColoredBlock.o ./Core/Src/ColoredBlock.su ./Core/Src/ConveyorBelt.cyclo ./Core/Src/ConveyorBelt.d ./Core/Src/ConveyorBelt.o ./Core/Src/ConveyorBelt.su ./Core/Src/MotorControl.cyclo ./Core/Src/MotorControl.d ./Core/Src/MotorControl.o ./Core/Src/MotorControl.su ./Core/Src/PololuEncoder.cyclo ./Core/Src/PololuEncoder.d ./Core/Src/PololuEncoder.o ./Core/Src/PololuEncoder.su ./Core/Src/PololuServo.cyclo ./Core/Src/PololuServo.d ./Core/Src/PololuServo.o ./Core/Src/PololuServo.su ./Core/Src/RCChannel.cyclo ./Core/Src/RCChannel.d ./Core/Src/RCChannel.o ./Core/Src/RCChannel.su ./Core/Src/RemoteControlControl.cyclo ./Core/Src/RemoteControlControl.d ./Core/Src/RemoteControlControl.o ./Core/Src/RemoteControlControl.su ./Core/Src/TB6612FNG_Motor.cyclo ./Core/Src/TB6612FNG_Motor.d ./Core/Src/TB6612FNG_Motor.o ./Core/Src/TB6612FNG_Motor.su ./Core/Src/VL53L0X.cyclo ./Core/Src/VL53L0X.d ./Core/Src/VL53L0X.o ./Core/Src/VL53L0X.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su

.PHONY: clean-Core-2f-Src

