###############################################################################
# Makefile for the project RTOS_Sample
###############################################################################

## General Flags
PROJECT = RTOS_Sample
MCU = atmega32
TARGET = RTOS_Sample.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99       -DF_CPU=12000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=RTOS_Sample.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"E:\MATERI\TERM7\embedded\TUGAS\Tugas UAS\Praktikum DX2\RTOS_Project\." -I"E:\MATERI\TERM7\embedded\TUGAS\Tugas UAS\Praktikum DX2\RTOS_Project\Kernel\include" -I"E:\MATERI\TERM7\embedded\TUGAS\Tugas UAS\Praktikum DX2\RTOS_Project\Kernel\portable\GCC\ATMega323" -I"E:\MATERI\TERM7\embedded\TUGAS\Tugas UAS\Praktikum DX2\RTOS_Project\i2c" -I"E:\MATERI\TERM7\embedded\TUGAS\Tugas UAS\Praktikum DX2\RTOS_Project\srf" 

## Objects that must be built in order to link
OBJECTS = RTOS_Sample.o croutine.o list.o queue.o tasks.o timers.o port.o heap_1.o i2c.o lcd.o game.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) RTOS_Sample.hex RTOS_Sample.eep RTOS_Sample.lss size

## Compile
RTOS_Sample.o: ../RTOS_Sample.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

croutine.o: ../Kernel/croutine.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

list.o: ../Kernel/list.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

queue.o: ../Kernel/queue.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

tasks.o: ../Kernel/tasks.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

timers.o: ../Kernel/timers.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

port.o: ../Kernel/portable/GCC/ATMega323/port.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

heap_1.o: ../Kernel/portable/MemMang/heap_1.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

i2c.o: ../srf/i2c.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

lcd.o: ../lcd.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

game.o: ../game.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) RTOS_Sample.elf dep/* RTOS_Sample.hex RTOS_Sample.eep RTOS_Sample.lss RTOS_Sample.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

