# Copyright 2015, Jeffrey E. Bedard

# Install location:
PREFIX=/usr

#CFLAGS=-Os -g -pipe -W -Wall -Werror -DDEBUG
#CFLAGS=-Os -g -pipe -W -Wall -Werror
LIBS=-lX11 -lXxf86vm
PROG=batwarn


all: ${PROG}
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
${PROG}: ${OBJS}
	cc -o ${PROG} ${OBJS} ${LIBS}
clean:
	rm -f ${PROG} ${OBJS}
install:
	install -s ${PROG} ${PREFIX}/bin/

