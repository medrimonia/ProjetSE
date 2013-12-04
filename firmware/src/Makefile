##########################################################################
##                                                                      ##
##   Makefile générique pour compiler des programmes pour avr d'Atmel   ##
##   Par Théotime Bollengier                                            ##
##                                                                      ##
##########################################################################


DEVICE       = atmega8
FCPU         = 16000000
TARGET       = main

OPTIMISATION = s
CFLAGS       = -W -Wall -std=c99 

PROGRAMMER   = arduino
PORT         = /dev/ttyUSB0
BAUDRATE     = 19200


##########################################################################

COMPILE = avr-gcc -mmcu=$(DEVICE) -O$(OPTIMISATION) $(CFLAGS) -DF_CPU=$(FCPU)


SRC       = $(wildcard *.c) $(wildcard *.S)
OBJECTS   = $(addprefix compiler_files/, $(SRC:.c=.o))
ASSEMBLE  = $(addprefix compiler_files/, $(SRC:.c=.s))

all: compiler_files/$(TARGET).hex

help:
	@echo "make ............. make the ihex file"
	@echo "make help ........ display this help"
	@echo "make flash ....... upload the ihex file into the µC"
	@echo "make disasm ...... display information from the ihex file"
	@echo "make clean ....... delete objects and hex files"
	@echo "make bin ......... make a raw binary file"
	@echo "make asm ......... compile only, do not assemble or link"
	@echo "make tarball ..... create a tar archive of the project"

flash: compiler_files/$(TARGET).hex
	avrdude -p ${DEVICE} -c $(PROGRAMMER) -P $(PORT) -b $(BAUDRATE) -U flash:w:$<

geteeprom: compiler_files
	avrdude -p ${DEVICE} -c $(PROGRAMMER) -P $(PORT) -U eeprom:r:compiler_files/eeprom:r

compiler_files/%.o : %.c compiler_files
	$(COMPILE) -c $< -o $@ -g

compiler_files/%.o : %.S compiler_files
	$(COMPILE) -x assembler-with-cpp -c $< -o $@ -g

compiler_files/%.s : %.c compiler_files
	$(COMPILE) -S $< -o $@ -g

disasm: compiler_files/$(TARGET).elf
	avr-objdump -h -S $< > compiler_files/${TARGET}.lss

asm: $(ASSEMBLE) 

compiler_files/$(TARGET).hex : compiler_files/$(TARGET).elf
	avr-objcopy -S -j .text -j .data -O ihex $< $@ -g
	avr-size $@

bin: compiler_files/$(TARGET).elf
	avr-objcopy -S -j .text -j .data -O binary $< compiler_files/${TARGET}.bin

compiler_files/$(TARGET).elf : $(OBJECTS)
	$(COMPILE) -o $@ $^

clean: 
	rm -r compiler_files/

tarball: backup_dir
	tar -czvf backup_dir/backup.tgz Makefile $(SRC) $(ASSM) 

compiler_files:
	@mkdir -p compiler_files

backup_dir:
	@mkdir -p backup_dir


