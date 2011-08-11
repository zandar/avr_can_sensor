OPTIMIZE= -O2
MCU_TARGET=atmega32

DEFS=
LIBS=

CC=avr-gcc

OBJCOPY=avr-objcopy
OBJDUMP=avr-objdump
AVRSVF=avrsvf

# bootloader option
BOOTLOADER=avrload.sh
BAUD=19200

override CFLAGS= -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS= -Wl,-Map,$(APP).map

PROJ=../build/AVR_CAN_proj

SRCS = main.c lcd.c display.c sja_control.c avr_sja1000p.c sensor.c timer.c fsm.c
OBJS:=$(SRCS:.c=.o)

all: $(PROJ).hex

%.elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

load:
	$(BOOTLOADER) $(BAUD) $(PROJ)
