SRC_FOLDER=src
HEADER_FOLDER=include
CFLAGS=-Wall -I${HEADER_FOLDER} -g

%.o: %.c
	${CC} -o $@ -c $< ${CFLAGS}

test_bit_utils: ${SRC_FOLDER}/test_bit_utils.o \
                ${SRC_FOLDER}/bit_utils.o
	${CC} -o $@  $^ ${LDFLAGS}

${SRC_FOLDER}/bit_utils.o: ${HEADER_FOLDER}/protocol.h

${SRC_FOLDER}/test_bit_utils.o: ${HEADER_FOLDER}/bit_utils.h

.PHONY: clean mrproper

clean:
	rm -rf ${SRC_FOLDER}/*.c

mrproper: clean
	rm -rf test_bit_utils