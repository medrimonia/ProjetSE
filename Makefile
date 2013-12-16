################################################################################
#                                 COMMON RULES                                 #
################################################################################
COMMON_SRC_DIR=src
COMMON_INC_DIR=include
CFLAGS=-Wall -Wextra -I${COMMON_INC_DIR}

FIRMWARE_SRC_DIR=firmware/src
FIRMWARE_INC_DIR=firmware/include
DRIVER_DIR=driver

COMMON_SRC   = $(wildcard $(COMMON_SRC_DIR)/*.c)
FIRMWARE_SRC = $(wildcard $(FIRMWARE_SRC_DIR)/*.c)
DRIVER_SRC   = $(wildcard $(DRIVER_DIR)/*.c)

.PHONY: all depend clean local_build cross_build

BINS=test_bit_utils     \
     test_driver        \
     test_protocol      \
     mok_firmware       \
     fake_device        \
     test_communication

all: ${BINS} all_cross

################################################################################
#                               LOCAL BUILD RULES                              #
################################################################################
LOC_COMPILER=gcc
LOC_CFLAGS= ${CFLAGS} -g

LOC_COMMON_OBJ   = $(addprefix local_build/, $(COMMON_SRC:.c=.o))
LOC_FIRMWARE_OBJ = $(addprefix local_build/, $(FIRMWARE_SRC:.c=.o))
LOC_DRIVER_OBJ   = $(addprefix local_build/, $(DRIVER_SRC:.c=.o))

local_build/src/%.o: src/%.c local_build
	${LOC_COMPILER} ${LOC_CFLAGS} -o $@ -c $<

local_build/firmware/%.o: firmware/%.c local_build
	${LOC_COMPILER} ${LOC_CFLAGS} -I${FIRMWARE_INC_DIR} -o $@ -c $<

local_build/driver/%.o: driver/%.c local_build
	${LOC_COMPILER} ${LOC_CFLAGS} -I${DRIVER_DIR} -o $@ -c $<

local_build:
	@mkdir -p local_build
	@mkdir -p local_build/driver
	@mkdir -p local_build/driver/tests
	@mkdir -p local_build/firmware
	@mkdir -p local_build/firmware/src
	@mkdir -p local_build/firmware/tests
	@mkdir -p local_build/src
	@mkdir -p local_build/src/tests

test_communication: local_build/driver/tests/test_communication.o \
                    ${LOC_COMMON_OBJ} ${LOC_DRIVER_OBJ}
	${LOC_COMPILER} -o $@  $^ ${LDFLAGS}

fake_device: local_build/firmware/firmware.o \
             ${LOC_FIRMWARE_OBJ} ${LOC_COMMON_OBJ}
	${LOC_COMPILER} -o $@  $^ ${LDFLAGS}

test_bit_utils: local_build/src/tests/test_bit_utils.o ${LOC_COMMON_OBJ}
	${LOC_COMPILER} -o $@  $^ ${LDFLAGS}

test_driver: local_build/driver/tests/test_driver.o \
             ${LOC_COMMON_OBJ} ${LOC_DRIVER_OBJ}
	${LOC_COMPILER} -o $@  $^ ${LDFLAGS}

test_protocol: local_build/src/tests/test_protocol.o \
               ${LOC_COMMON_OBJ} ${LOC_DRIVER_OBJ}
	${LOC_COMPILER} -o $@  $^ ${LDFLAGS}

mok_firmware: local_build/firmware/tests/mok_firmware.o \
              ${LOC_FIRMWARE_OBJ} ${LOC_COMMON_OBJ}
	${LOC_COMPILER} -o $@  $^ ${LDFLAGS}


################################################################################
#                           CROSS COMPILING BUILD RULES                        #
################################################################################

CROSS_COMPILER = avr-gcc
DEVICE         = atmega8
FCPU           = 16000000
TARGET         = firmware

CROSS_CFLAGS   = $(LOC_CFLAGS) -DEMBEDDED -Os -I${FIRMWARE_INC_DIR}

PROGRAMMER     = arduino
PORT           = /dev/ttyUSB0
BAUDRATE       = 19200

CROSS_COMPILE  = $(CROSS_COMPILER) -mmcu=$(DEVICE) $(CROSS_CFLAGS) -DF_CPU=$(FCPU)

CROSS_COMMON_OBJ   = $(addprefix cross_build/, $(COMMON_SRC:.c=.o))
CROSS_FIRMWARE_OBJ = $(addprefix cross_build/, $(FIRMWARE_SRC:.c=.o))
CROSS_COMMON_ASS   = $(addprefix cross_build/, $(COMMON_SRC:.c=.s))
CROSS_FIRMWARE_ASS = $(addprefix cross_build/, $(FIRMWARE_SRC:.c=.s))

all_cross: cross_build/$(TARGET).hex

flash: cross_build/$(TARGET).hex
	avrdude -p ${DEVICE} -c $(PROGRAMMER) -P $(PORT) -b $(BAUDRATE) -U flash:w:$<

geteeprom: cross_build
	avrdude -p ${DEVICE} -c $(PROGRAMMER) -P $(PORT) -U eeprom:r:cross_build/eeprom:r

cross_build/%.o : %.c cross_build
	$(CROSS_COMPILE) -c $< -o $@ -g

cross_build/%.o : %.S cross_build
	$(CROSS_COMPILE) -x assembler-with-cpp -c $< -o $@ -g

cross_build/%.s : %.c cross_build
	$(CROSS_COMPILE) -S $< -o $@ -g

disasm: cross_build/$(TARGET).elf
	avr-objdump -h -S $< > cross_build/${TARGET}.lss

asm: $(ASSEMBLE)

cross_build/$(TARGET).hex : cross_build/$(TARGET).elf
	avr-objcopy -S -j .text -j .data -O ihex $< $@ -g
	avr-size $@

bin: cross_build/$(TARGET).elf
	avr-objcopy -S -j .text -j .data -O binary $< cross_build/${TARGET}.bin

cross_build/$(TARGET).elf : cross_build/firmware/firmware.o \
														$(CROSS_COMMON_OBJ)             \
													  $(CROSS_FIRMWARE_OBJ)
	$(CROSS_COMPILE) -o $@ $^

cross_build:
	@mkdir -p cross_build
	@mkdir -p cross_build/src
	@mkdir -p cross_build/firmware
	@mkdir -p cross_build/firmware/src


#depend needs to be updated with the new Makefile

depend: .depend

.depend: $(SRC)
	rm -f ./.depend
	$(CC) -I${INCDIR} -MM $^ > ./.depend;

include .depend


clean:
	@rm -rf local_build

mrproper: clean
	@rm -f ${BINS}
