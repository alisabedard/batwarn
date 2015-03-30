# Copyright 2015, Jeffrey E. Bedard

#CFLAGS=-Os -g -pipe -W -Wall -Werror -DDEBUG
CFLAGS=-Os -g -pipe -W -Wall -Werror
PROG=batwarn


all: ${PROG}
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
${PROG}: ${OBJS}
	cc -o ${PROG} ${OBJS}
clean:
	rm -f ${PROG} ${OBJS}
install:
	install -s ${PROG} /usr/local/bin

