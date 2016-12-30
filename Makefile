# batwarn - (C) 2015-2017 Jeffrey E. Bedard
# Install location:
PREFIX=/usr
CFLAGS+=-Wall -Wextra
# Porting:
# FreeBSD:
CFLAGS+=-I/usr/local/include
LDFLAGS+=-L/usr/local/lib
# OpenBSD:
CFLAGS+=-I/usr/X11R6/include
LDFLAGS+=-L/usr/X11R6/lib
# NetBSD:
CFLAGS+=-I/usr/pkg/include
CFLAGS+=-I/usr/X11R7/include
LDFLAGS+=-L/usr/pkg/lib
LDFLAGS+=-L/usr/X11R7/lib
LDFLAGS+=-Wl,-R/usr/X11R7/lib
LDFLAGS+=-Wl,-R/usr/X11R6/lib
LDFLAGS+=-lX11 -lXxf86vm
PROG=batwarn
all: ${PROG}
OBJS=batwarn.o gamma.o main.o
CFLAGS+=-std=c99
CFLAGS+=-D_XOPEN_SOURCE=700
CFLAGS+=${debug_flags}
${PROG}: ${OBJS} gam sgam
	${CC} -o ${PROG} ${OBJS} ${LDFLAGS}
gamobj=gam.o gamma.o
gam: ${gamobj}
	${CC} -o gam ${gamobj} ${LDFLAGS}
sgamobj=sgam.o gamma.o
sgam: ${sgamobj}
	${CC} -o sgam ${sgamobj} ${LDFLAGS}
debug: clean
	make debug_flags='-DDEBUG'
clean:
	rm -f ${PROG} ${OBJS} gam ${gamobj} ${sgamobj}
install:
	install -d ${DESTDIR}${PREFIX}/bin
	install -s ${PROG} ${DESTDIR}${PREFIX}/bin/
	install -s gam ${DESTDIR}${PREFIX}/bin/
	install -s sgam ${DESTDIR}${PREFIX}/bin/
cppcheck:
	cppcheck --enable=all --inconclusive --std=c11 . \
		-DDEBUG 2> cppcheck.log
#EOF
