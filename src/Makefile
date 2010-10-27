OPTIMIZE= -O2
MCU_TARGET=atmega16

DEFS=
LIBS=

CC=avr-gcc

OBJCOPY=avr-objcopy
OBJDUMP=avr-objdump
AVRSVF=avrsvf

override CFLAGS= -g -Wall $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
override LDFLAGS= -Wl,-Map,$(APP).map


PROJ=../build/AVR_CAN_proj

SRCS = main.c lcd.c display.c sja_control.c avr_sja1000p.c
OBJS:=$(SRCS:.c=.o)


all: $(PROJ).hex $(PROJ).bin $(PROJ).lst


debug: override CFLAGS += -DDEBUG
debug: all

debug9: override CFLAGS += -DDEBUG=9
debug9: all

	
release: clean
release: all


%.elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)


%.o: %.c $(wildcard *.h)
	$(CC) -c $(CFLAGS) $< -o $@ 


%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@


%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@


%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@


%.svf: %.hex
	$(AVRSVF) -datmega128 -s -e -if$< -pf -vf -f0xFFB0BF -F -l0xFC -L -ov$@ -mp -t2 -wm0 >avrsvf.log



EXTRA_CLEAN_FILES=

clean:
	rm -f $(EXTRA_CLEAN_FILES)
	rm -f $(wildcard *.o *.lst *.map *.elf *.log)
	rm -f $(wildcard *.~*)
	rm -f $(wildcard *.hex)
	rm -f $(APP).bin
