TARGET = main
LD_SCRIPT = stm32f030f4p6.ld
MCU_SPEC = cortex-m0 
MCU_DEF = STM32F030x6

TOOL = ./build arm-none-eabi
CC = $(TOOL)-gcc
OBJCOPY = $(TOOL)-objcopy
OBJDUMP = $(TOOL)-objdump
SIZE = $(TOOL)-size

CFLAGS  = -Wall -g -std=gnu99 -Os -nostdlib -lgcc
CFLAGS += -mcpu=$(MCU_SPEC) -mthumb
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wl,-Map=$(TARGET).map
CFLAGS += -D$(MCU_DEF)

LDFLAGS  = -nostdlib -ffreestanding -lgcc
LDFLAGS += -mcpu=$(MCU_SPEC) -mthumb

AS_SRC  = ./device/startup_stm32f030x6.s
C_SRC += ./src/main.c ./src/system_stm32f0xx.c ./src/tinyprintf.c

LSCRIPT_INC = ./device/ld
INCLUDE = -I./inc
INCLUDE += -I./inc/cmsis

OBJS =  $(AS_SRC:.s=.o)
OBJS += $(C_SRC:.c=.o)

.PHONY: all flash clean

all: $(TARGET).bin

%.o : %.s
	$(CC) -c $(CFLAGS) $< -o $@

%.o : %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $^ $(LDFLAGS) -L$(LSCRIPT_INC) -T$(LD_SCRIPT) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -S -O binary $< $@
	$(SIZE) $<

flash: 
	st-flash write $(TARGET).bin 0x08000000

clean:
	rm -f $(OBJS)
	rm -f $(TARGET).elf
	rm -f $(TARGET).bin
	rm -f $(TARGET).map
