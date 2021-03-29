# ---------------------------------------- 
# Hardware
DEVICE       = STM32G031xx
MCU          = cortex-m0
ARCH         = armv6-m
FLASH_BASE   = 0x08000000
FLASH_OFFSET = 0
# ---------------------------------------- 
# Directories
SRC_DIR      = ./src
INC_DIR      = ./inc
BIN_DIR      = ./bin
OBJ_DIR      = ./obj
# ---------------------------------------- 
# Toolchain
CC           = arm-none-eabi-gcc
LD           = arm-none-eabi-ld
AR           = arm-none-eabi-ar
AS           = arm-none-eabi-as
CP           = arm-none-eabi-objcopy
OD           = arm-none-eabi-objdump
# ---------------------------------------- 
# Target files
TARGET       = main
BIN          = $(BIN_DIR)/$(TARGET).bin
EXECUTABLE   = $(BIN_DIR)/$(TARGET).elf
# ---------------------------------------- 
# Input files
LDSCRIPT     = ./STM32G031K8Tx_FLASH.ld
SRC          = $(wildcard $(SRC_DIR)/*)
INC          = -I$(INC_DIR)
# ---------------------------------------- 
# Seperate C and asm files
CFILES       = $(filter %.c, $(SRC))
ASMFILES     = $(filter %.s, $(SRC))
# ---------------------------------------- 
# Object files
#COBJ         = $(CFILES:.c=.o)
COBJ         = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(CFILES))
#SOBJ         = $(ASMFILES:.s=.o)
SOBJ         = $(patsubst $(SRC_DIR)/%.s, $(OBJ_DIR)/%.o, $(ASMFILES))
OBJ          = $(COBJ) $(SOBJ)
# ---------------------------------------- 
# Compiler flags
# ---------------------------------------- 
## Compiler flags
LDFLAGS      = -T$(LDSCRIPT),--gc-sections 
LDFLAGS     += -lm

MCFLAGS      = -mcpu=$(MCU) 
MCFLAGS     += -mthumb 
MCFLAGS     += -mlittle-endian 
MCFLAGS     += -mthumb-interwork
MCFLAGS     += -march=$(ARCH)

OPTIMIZE     = -O3 
OPTIMIZE    += -fdata-sections 
OPTIMIZE    += -ffunction-sections 
OPTIMIZE    += -fsingle-precision-constant

DEBUG        = -g3

CFLAGS       = $(MCFLAGS) $(DEBUG)  $(OPTIMIZE) -MP -MMD
ASFLAGS      = $(MCFLAGS) $(DEBUG)
# ---------------------------------------- 
# Defines to be passed to the compiler
DEFINES     = -D$(DEVICE) \
              -DVECT_TAB_OFFSET=$(FLASH_OFFSET)
# ---------------------------------------- 
all: $(BIN)

$(BIN): $(EXECUTABLE)
	$(CP) -O binary $^ $@

$(EXECUTABLE): $(OBJ) $(LDSCRIPT)
	$(CC) $(CFLAGS) $(sort $(OBJ)) -Wl,$(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) -c $(DEFINES) $(INC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	$(AS) -c $(ASFLAGS) $(INC) $< -o $@

clean:
	rm -f $(OBJ) $(BIN) $(EXECUTABLE)

flash:
	st-flash write $(BIN) $(FLASH_BASE)
