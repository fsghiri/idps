################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/fsghi/Downloads/PFA/idps/app/src/app.c \
C:/Users/fsghi/Downloads/PFA/idps/app/src/nfem.c \
C:/Users/fsghi/Downloads/PFA/idps/app/src/ntcm.c 

OBJS += \
./app/src/app.o \
./app/src/nfem.o \
./app/src/ntcm.o 

C_DEPS += \
./app/src/app.d \
./app/src/nfem.d \
./app/src/ntcm.d 


# Each subdirectory must supply rules for building sources it contributes
app/src/app.o: C:/Users/fsghi/Downloads/PFA/idps/app/src/app.c app/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H755xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -c -I../Core/Inc -I../../app/inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
app/src/nfem.o: C:/Users/fsghi/Downloads/PFA/idps/app/src/nfem.c app/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H755xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -c -I../Core/Inc -I../../app/inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
app/src/ntcm.o: C:/Users/fsghi/Downloads/PFA/idps/app/src/ntcm.c app/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H755xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -c -I../Core/Inc -I../../app/inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-app-2f-src

clean-app-2f-src:
	-$(RM) ./app/src/app.cyclo ./app/src/app.d ./app/src/app.o ./app/src/app.su ./app/src/nfem.cyclo ./app/src/nfem.d ./app/src/nfem.o ./app/src/nfem.su ./app/src/ntcm.cyclo ./app/src/ntcm.d ./app/src/ntcm.o ./app/src/ntcm.su

.PHONY: clean-app-2f-src

