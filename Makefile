# batwarn - (C) 2015-2016 Jeffrey E. Bedard

# Install location:
PREFIX=/usr
CFLAGS+=-Wall -W
#CFLAGS+=-ggdb
#CFLAGS+=-DDEBUG

LIBS=-lX11 -lXxf86vm
PROG=batwarn
all: ${PROG}
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
CFLAGS+=-std=c11
CFLAGS+=-D_XOPEN_SOURCE=700
${PROG}: ${OBJS}
	cc -o ${PROG} ${OBJS} ${LIBS}
clean:
	rm -f ${PROG} ${OBJS}
install:
	install -d ${DESTDIR}${PREFIX}/bin
	install -s ${PROG} ${DESTDIR}${PREFIX}/bin/

