CC=gcc
CFLAGS=-Wall -Wextra
LDFLAGS=

SRCDIR=.
SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(SRC:.c=.o)
BIN=atmega8-driver

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJ)

%.o: %.c

clean:
	@rm -f $(BIN)

mrproper: clean
	@rm -f $(OBJ)
