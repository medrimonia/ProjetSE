SRCDIR=src
INCDIR=include
FIRMWARE_INCLUDE=firmware/include

CC=gcc
CFLAGS= -Wall -Wextra -I${INCDIR} -I${FIRMWARE_INCLUDE} -g
LDFLAGS=

SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(SRC:.c=.o)

.PHONY: all depend clean

BINS=test_bit_utils \
     test_driver    \
     test_protocol  \
     mok_firmware

all: ${BINS}

%.o: %.c
	${CC} ${CFLAGS} -o $@ -c $<

test_bit_utils: src/tests/test_bit_utils.o ${OBJ}
	${CC} -o $@  $^ ${LDFLAGS}

test_driver: driver/tests/test_driver.o ${OBJ}
	${CC} -o $@  $^ ${LDFLAGS}

test_protocol: src/tests/test_protocol.o ${OBJ}
	${CC} -o $@  $^ ${LDFLAGS}

mok_firmware: firmware/tests/mok_firmware.o firmware/src/firmware_packet_process.o ${OBJ}
	${CC} -o $@  $^ ${LDFLAGS}

depend: .depend

.depend: $(SRC)
	rm -f ./.depend
	$(CC) -I${INCDIR} -MM $^ > ./.depend;

include .depend


clean:
	@rm -f ${OBJ}

mrproper: clean
	@rm -f ${BINS}
