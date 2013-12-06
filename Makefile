SRC_FOLDER=src
HEADER_FOLDER=include
CFLAGS=-Wextra -Wall -I${HEADER_FOLDER} -g

BINS=test_bit_utils \
     test_driver

binaries: ${BINS}

%.o: %.c
	${CC} -o $@ -c $< ${CFLAGS}

test_bit_utils: ${SRC_FOLDER}/test_bit_utils.o \
                ${SRC_FOLDER}/bit_utils.o
	${CC} -o $@  $^ ${LDFLAGS}

test_driver: ${SRC_FOLDER}/bit_utils.o   \
             ${SRC_FOLDER}/driver.o      \
             ${SRC_FOLDER}/test_driver.o \
             ${SRC_FOLDER}/mask.o
	${CC} -o $@  $^ ${LDFLAGS}

${SRC_FOLDER}/bit_utils.o: ${HEADER_FOLDER}/bit_utils.h \
                           ${HEADER_FOLDER}/protocol.h

${SRC_FOLDER}/test_bit_utils.o: ${HEADER_FOLDER}/bit_utils.h \
                                ${HEADER_FOLDER}/protocol.h

${SRC_FOLDER}/test_driver.o: ${HEADER_FOLDER}/driver.h

${SRC_FOLDER}/driver.o: ${HEADER_FOLDER}/driver.h    \
                        ${HEADER_FOLDER}/bit_utils.h \
                        ${HEADER_FOLDER}/protocol.h

${SRC_FOLDER}/mask.o: ${HEADER_FOLDER}/mask.h      \
                      ${HEADER_FOLDER}/bit_utils.h

.PHONY: clean mrproper

clean:
	rm -rf ${SRC_FOLDER}/*.o

mrproper: clean
	rm -rf test_bit_utils