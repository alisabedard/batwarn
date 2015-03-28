# Copyright 2015, Jeffrey E. Bedard

CFLAGS=-Os -g -pipe -W -Wall -Werror
#CFLAGS+=-DDEBUG

PROG=batwarn
all: ${PROG}
OBJS=${PROG}.o main.o signal.o
${PROG}: ${OBJS}
	cc -o ${PROG} ${OBJS}
clean:
	rm ${PROG} ${OBJS}
install:
	install -s ${PROG} /usr/local/bin

