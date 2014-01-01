################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Network.cpp \
../Protein.cpp \
../RandomNetwork.cpp \
../main.cpp 

OBJS += \
./Network.o \
./Protein.o \
./RandomNetwork.o \
./main.o 

CPP_DEPS += \
./Network.d \
./Protein.d \
./RandomNetwork.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


