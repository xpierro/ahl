################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../BSPTree.cpp \
../BoundingBox.cpp \
../Entity.cpp \
../MathVector.cpp \
../OGLWrapper.cpp \
../Plane.cpp \
../Polygon.cpp \
../ProgressBar.cpp \
../Side.cpp \
../Solid.cpp \
../Test.cpp \
../VMFParser.cpp \
../World.cpp 

OBJS += \
./BSPTree.o \
./BoundingBox.o \
./Entity.o \
./MathVector.o \
./OGLWrapper.o \
./Plane.o \
./Polygon.o \
./ProgressBar.o \
./Side.o \
./Solid.o \
./Test.o \
./VMFParser.o \
./World.o 

CPP_DEPS += \
./BSPTree.d \
./BoundingBox.d \
./Entity.d \
./MathVector.d \
./OGLWrapper.d \
./Plane.d \
./Polygon.d \
./ProgressBar.d \
./Side.d \
./Solid.d \
./Test.d \
./VMFParser.d \
./World.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DILUT_USE_OPENGL -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


