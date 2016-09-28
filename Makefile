# batwarn - (C) 2015-2016 Jeffrey E. Bedard

# Install location:
PREFIX=/usr

CFLAGS+=-Wall -Wextra

# Porting:
CFLAGS+=-I/usr/local/include
CFLAGS+=-I/usr/pkg/include
CFLAGS+=-I/usr/X11R7/include
CFLAGS+=-I/usr/X11R6/include
LDFLAGS+=-L/usr/local/lib
LDFLAGS+=-L/usr/pkg/lib
LDFLAGS+=-L/usr/X11R7/lib
LDFLAGS+=-L/usr/X11R6/lib

LDFLAGS+=-lX11 -lXxf86vm
PROG=batwarn

all: ${PROG}

OBJS=batwarn.o gamma.o main.o
CFLAGS+=-std=c99
CFLAGS+=-D_XOPEN_SOURCE=700
CFLAGS+=${debug_flags}

${PROG}: ${OBJS}
	${CC} -o ${PROG} ${OBJS} ${LDFLAGS}
debug: clean
	make debug_flags='-DDEBUG'
clean:
	rm -f ${PROG} ${OBJS}
install:
	install -d ${DESTDIR}${PREFIX}/bin
	install -s ${PROG} ${DESTDIR}${PREFIX}/bin/
#EOF
