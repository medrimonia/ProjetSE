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

.PHONY: all depend clean local_build

BINS=test_bit_utils     \
     test_driver        \
     test_protocol      \
     mok_firmware       \
     fake_device        \
     test_communication

all: ${BINS}

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

#TODO


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
