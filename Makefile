# ==============================================================================
#
#	 ███╗   ███╗ ██████╗ ███╗   ██╗ ██████╗ ██╗     ██╗████████╗██╗  ██╗
#	 ████╗ ████║██╔═══██╗████╗  ██║██╔═══██╗██║     ██║╚══██╔══╝██║  ██║
#	 ██╔████╔██║██║   ██║██╔██╗ ██║██║   ██║██║     ██║   ██║   ███████║
#	 ██║╚██╔╝██║██║   ██║██║╚██╗██║██║   ██║██║     ██║   ██║   ██╔══██║
#	 ██║ ╚═╝ ██║╚██████╔╝██║ ╚████║╚██████╔╝███████╗██║   ██║   ██║  ██║
#	 ╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝ ╚══════╝╚═╝   ╚═╝   ╚═╝  ╚═╝
#
#                           ░▒▓█ _OBELISK_ █▓▒░
#
#   Makefile
#   Author     : MrZloHex
#   Date       : 2025-02-02
#   Version    : 1.0
#
#   Description:
#       This Makefile compiles and links the tma project sources.
#       It searches recursively under the "src" directory for source files,
#       compiles them into "obj", and links the final executable in "bin".
#
#   Warning    : This Makefile is so cool it might make your terminal shine!
# ==============================================================================
#
# Verbosity: Set V=1 for verbose output (full commands) or leave it unset for cool, quiet messages.
V ?= 0
ifeq ($(V),0)
	Q = @
else
	Q =
endif

BUILD ?= debug


CC      	 = arm-none-eabi-gcc
AS 			 = arm-none-eabi-gcc -x assembler-with-cpp
CP 			 = arm-none-eabi-objcopy
SZ 			 = arm-none-eabi-size


DEFINES = -DUSE_HAL_DRIVER -DSTM32F103xE -DSTM32F1

INCLUDES = 	-ICore/Inc -IDrivers/STM32F1xx_HAL_Driver/Inc \
			-IDrivers/STM32F1xx_HAL_Driver/Inc/Legacy \
			-IDrivers/CMSIS/Device/ST/STM32F1xx/Include \
			-IDrivers/CMSIS/Include

INCLUDES 	   += -ILibraries/fram_stm32 \
				  -ILibraries/lcd_i2c_stm32 \
				  -ILibraries/mjson \
				  -ILibraries/sth2x

COM_FLAGS = -mcpu=$(CPU) -mthumb -fdata-sections -ffunction-sections -Wall -Wextra

ASFLAGS = $(COM_FLAGS)


CFLAGS  = $(COM_FLAGS) -Wpedantic
CFLAGS += -std=c2x -Wstrict-aliasing
CFLAGS += -Wno-old-style-declaration
CFLAGS += -MMD -MP
CFLAGS += $(INCLUDES) $(DEFINES)

ifeq ($(BUILD),debug)
	CFLAGS += -O0 -g -gdwarf-2
	ASFLAGS += -Og
else ifeq ($(BUILD),release)
	CFLAGS += -O2 -Werror
	ASFLAGS += -Os
else
	$(error Unknown build mode: $(BUILD). Use BUILD=debug or BUILD=release)
endif

LDSCRIPT 	= STM32F103RCTx_FLASH.ld
LLIBS 		= -lc -lm -lnosys
LDFLAGS 	= -mcpu=cortex-m3 -T$(LDSCRIPT) $(LLIBS)
LDFLAGS    += -Wl,-Map=$(BIN)/$(TARGET).map,--cref -Wl,--gc-sections




TARGET 		= obelisk

CPU 		= cortex-m3

SRC 		= Core Drivers Libraries
ASM 		= startup_stm32f103xe.s
OBJ 		= obj
BIN 		= bin
LIB 		=


SOURCES 	= $(shell find $(SRC) -type f -name '*.c')
OBJECTS 	= $(addprefix $(OBJ)/, $(SOURCES))
OBJECTS    += $(addprefix $(OBJ)/, $(ASM))

ifneq ($(strip $(LIB)),)
LIBRARY 	= $(wildcard $(LIB)/*.c)
OBJECTS    += $(addprefix $(OBJ)/, $(LIBRARY))
endif

OBJECTS    := $(patsubst %.c, %.o, $(OBJECTS))
OBJECTS    := $(patsubst %.s, %.o, $(OBJECTS))


all: $(BIN)/$(TARGET).elf $(BIN)/$(TARGET).hex $(BIN)/$(TARGET).bin

$(BIN)/$(TARGET).elf: $(OBJECTS)
	@mkdir -p $(BIN)
	@echo "  CCLD     $(patsubst $(BIN)/%,%,$@)"
	$(Q) $(CC) -o $@ $^ $(LDFLAGS)
	
$(OBJ)/%.o: %.c
	@mkdir -p $(@D)
	@echo "  CC       $(patsubst $(OBJ)/%,%,$@)"
	$(Q) $(CC) -o $@ -c $< $(CFLAGS) -Wa,-a,-ad,-alms=$(OBJ)/$(notdir $(<:.c=.lst))

$(OBJ)/%.o: %.s
	@mkdir -p $(@D)
	@echo "  AR       $(patsubst $(OBJ)/%,%,$@)"
	$(Q) $(AS) -o $@ -c $< $(ASFLAGS)

ifneq ($(strip $(LIB)),)
$(OBJ)/%.o: $(LIB)/%.c
	@mkdir -p $(@D)
	@echo "  CC       $(patsubst $(OBJ)/%,%,$@)"
	$(Q) $(CC) -o $@ -c $< $(CFLAGS)
endif

$(BIN)/%.hex: $(BIN)/%.elf
	@echo "  CP 	 $(patsubst $(BIN)/%,%,$@)"
	$(Q) $(CP) -O ihex $< $@

$(BIN)/%.bin: $(BIN)/%.elf
	@echo "  CP 	 $(patsubst $(BIN)/%,%,$@)"
	$(Q) $(CP) -O binary -S $< $@

clean:
	$(Q) rm -rf $(OBJ) $(BIN)

debug:
	$(MAKE) BUILD=debug all

release:
	$(MAKE) BUILD=release all

.PHONY: all clean debug release

-include $(OBJECTS:.o=.d)
