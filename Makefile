# batwarn - (C) 2015-2016 Jeffrey E. Bedard

# Install location:
PREFIX=/usr

CFLAGS+=-Wall -Wextra

# Porting:
CFLAGS+=-I/usr/local/include
CFLAGS+=-I/usr/pkg/include
CFLAGS+=-I/usr/X11R7/include
CFLAGS+=-I/usr/X11R6/include
LIBS+=-L/usr/local/lib
LIBS+=-L/usr/pkg/lib
LIBS+=-L/usr/X11R7/lib
LIBS+=-L/usr/X11R6/lib
LIBS+=-lX11 -lXxf86vm
PROG=batwarn

all: ${PROG}

OBJS=batwarn.o gamma.o main.o
CFLAGS+=-std=c11
CFLAGS+=-D_XOPEN_SOURCE=700

${PROG}: ${OBJS}
	cc -o ${PROG} ${OBJS} ${LIBS}
clean:
	rm -f ${PROG} ${OBJS}
install:
	install -d ${DESTDIR}${PREFIX}/bin
	install -s ${PROG} ${DESTDIR}${PREFIX}/bin/

