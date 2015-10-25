# Copyright 2015, Jeffrey E. Bedard

# Install location:
PREFIX=/usr
CFLAGS=-Os -Wall -W -Werror
LIBS=-lX11 -lXxf86vm
PROG=batwarn

all: ${PROG}
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
${PROG}: ${OBJS}
	cc -o ${PROG} ${OBJS} ${LIBS}
clean:
	rm -f ${PROG} ${OBJS}
install:
	install -d ${DESTDIR}${PREFIX}/bin
	install -s ${PROG} ${DESTDIR}${PREFIX}/bin/

