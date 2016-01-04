# batwarn - (C) 2015-2016 Jeffrey E. Bedard

# Install location:
PREFIX=/usr
CFLAGS=-Os -Wall -W

# Uncomment to enable debugging code
#CFLAGS+=-DDEBUG

# Uncomment to enable suspend at critical battery
CFLAGS+=-DSUSPEND

# Uncomment to enable stdio output
CFLAGS+=-DSTDIO

LIBS=-lX11 -lXxf86vm
PROG=batwarn
all: ${PROG}
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
CFLAGS+=-std=c99 
${PROG}: ${OBJS}
	cc -o ${PROG} ${OBJS} ${LIBS}
clean:
	rm -f ${PROG} ${OBJS}
install:
	install -d ${DESTDIR}${PREFIX}/bin
	install -s ${PROG} ${DESTDIR}${PREFIX}/bin/

