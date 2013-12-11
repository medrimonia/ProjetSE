SRCDIR=src
INCDIR=include

CC=gcc
CFLAGS= -Wall -Wextra -I${INCDIR} -g
LDFLAGS=

SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(SRC:.c=.o)

.PHONY: all depend clean

BINS=test_bit_utils \
     test_driver    \
     test_protocol

all: ${BINS}

%.o: %.c
	${CC} ${CFLAGS} -o $@ -c $^

test_bit_utils: ${OBJ}
	${CC} -o $@  $^ ${LDFLAGS}

test_driver: ${OBJ}
	${CC} -o $@  $^ ${LDFLAGS}

test_protocol: ${OBJ}
	${CC} -o $@  $^ ${LDFLAGS}

depend: .depend

.depend: $(SRC)
	rm -f ./.depend
	$(CC) -I${INCDIR} -MM $^ > ./.depend;

include .depend


clean:
	@find . -name *.o -delete

mrproper: clean
	@rm -f ${BINS}
