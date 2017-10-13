################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../main.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
main.o

OBJS_AS_ARGS +=  \
main.o

C_DEPS +=  \
main.d

C_DEPS_AS_ARGS +=  \
main.d

OUTPUT_FILE_PATH +=ProjectMain.elf

OUTPUT_FILE_PATH_AS_ARGS +=ProjectMain.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+=  \
C:/Users/lukasah/Documents/embeddedlab/TTK4155/Project/v1/ADCLib/Debug/libADCLib.a \
C:/Users/lukasah/Documents/embeddedlab/TTK4155/Project/v1/CANLib/Debug/libCANLib.a \
C:/Users/lukasah/Documents/embeddedlab/TTK4155/Project/v1/ExtSramLib/Debug/libExtSramLib.a \
C:/Users/lukasah/Documents/embeddedlab/TTK4155/Project/v1/JoystickLib/Debug/libJoystickLib.a \
C:/Users/lukasah/Documents/embeddedlab/TTK4155/Project/v1/MCP2515Lib/Debug/libMCP2515Lib.a \
C:/Users/lukasah/Documents/embeddedlab/TTK4155/Project/v1/menuLib/Debug/libmenuLib.a \
C:/Users/lukasah/Documents/embeddedlab/TTK4155/Project/v1/oledLib/Debug/liboledLib.a \
C:/Users/lukasah/Documents/embeddedlab/TTK4155/Project/v1/SliderLib/Debug/libSliderLib.a \
C:/Users/lukasah/Documents/embeddedlab/TTK4155/Project/v1/SPILib/Debug/libSPILib.a \
C:/Users/lukasah/Documents/embeddedlab/TTK4155/Project/v1/UARTlib/Debug/libUARTlib.a

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler



./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"../../UARTlib" -I"../../ExtSramLib"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g2 -Wall -mmcu=atmega162 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="ProjectMain.map" -Wl,--start-group -Wl,-lm -Wl,-lADCLib -Wl,-lCANLib -Wl,-lExtSramLib -Wl,-lJoystickLib -Wl,-lMCP2515Lib -Wl,-lmenuLib -Wl,-loledLib -Wl,-lSliderLib -Wl,-lSPILib -Wl,-lUARTlib  -Wl,--end-group -Wl,-L"../../UARTlib" -Wl,-L"../../ExtSramLib" -Wl,-L"C:\Users\lukasah\Documents\embeddedlab\TTK4155\Project\v1\ADCLib\Debug" -Wl,-L"C:\Users\lukasah\Documents\embeddedlab\TTK4155\Project\v1\CANLib\Debug" -Wl,-L"C:\Users\lukasah\Documents\embeddedlab\TTK4155\Project\v1\ExtSramLib\Debug" -Wl,-L"C:\Users\lukasah\Documents\embeddedlab\TTK4155\Project\v1\JoystickLib\Debug" -Wl,-L"C:\Users\lukasah\Documents\embeddedlab\TTK4155\Project\v1\MCP2515Lib\Debug" -Wl,-L"C:\Users\lukasah\Documents\embeddedlab\TTK4155\Project\v1\menuLib\Debug" -Wl,-L"C:\Users\lukasah\Documents\embeddedlab\TTK4155\Project\v1\oledLib\Debug" -Wl,-L"C:\Users\lukasah\Documents\embeddedlab\TTK4155\Project\v1\SliderLib\Debug" -Wl,-L"C:\Users\lukasah\Documents\embeddedlab\TTK4155\Project\v1\SPILib\Debug" -Wl,-L"C:\Users\lukasah\Documents\embeddedlab\TTK4155\Project\v1\UARTlib\Debug"  -Wl,--gc-sections -mrelax -mmcu=atmega162  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "ProjectMain.elf" "ProjectMain.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "ProjectMain.elf" "ProjectMain.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "ProjectMain.elf" > "ProjectMain.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "ProjectMain.elf" "ProjectMain.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1061\avr8-gnu-toolchain\bin\avr-size.exe" "ProjectMain.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "ProjectMain.elf" "ProjectMain.a" "ProjectMain.hex" "ProjectMain.lss" "ProjectMain.eep" "ProjectMain.map" "ProjectMain.srec" "ProjectMain.usersignatures"
	