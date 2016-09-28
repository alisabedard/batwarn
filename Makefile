# batwarn - (C) 2015-2016 Jeffrey E. Bedard

# Install location:
PREFIX=/usr

CFLAGS+=-Wall -Wextra
CFLAGS+=-I/usr/X11R6/include
LDFLAGS+=-L/usr/X11R6/lib
LDFLAGS+=-lX11 -lXxf86vm
PROG=batwarn

all: ${PROG}

OBJS=batwarn.o gamma.o main.o
CFLAGS+=-std=c99
CFLAGS+=-D_XOPEN_SOURCE=700

${PROG}: ${OBJS}
	cc -o ${PROG} ${OBJS} ${LDFLAGS}
clean:
	rm -f ${PROG} ${OBJS}
install:
	install -d ${DESTDIR}${PREFIX}/bin
	install -s ${PROG} ${DESTDIR}${PREFIX}/bin/

