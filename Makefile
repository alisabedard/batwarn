# batwarn - (C) 2015-2017 Jeffrey E. Bedard
# Install location:
PREFIX=/usr
CFLAGS+=-Wall -Wextra
CFLAGS+=-ggdb
# Porting:
# FreeBSD:
#CFLAGS+=-I/usr/local/include
#LDFLAGS+=-L/usr/local/lib
# OpenBSD:
#CFLAGS+=-I/usr/X11R6/include
#LDFLAGS+=-L/usr/X11R6/lib
# NetBSD:
#CFLAGS+=-I/usr/pkg/include
#CFLAGS+=-I/usr/X11R7/include
#LDFLAGS+=-L/usr/pkg/lib
#LDFLAGS+=-L/usr/X11R7/lib
#LDFLAGS+=-Wl,-R/usr/X11R7/lib
#LDFLAGS+=-Wl,-R/usr/X11R6/lib

LDFLAGS+=-lX11 -lXxf86vm
PROG=batwarn

prefix=${DESTDIR}${PREFIX}
bindir=${prefix}/bin
docdir=${prefix}/share/doc/${PROG}

OBJS=batwarn.o gamma.o main.o util.o
CFLAGS+=-std=c99
CFLAGS+=-D_XOPEN_SOURCE=700
CFLAGS+=${debug_flags}

all: ${PROG}
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
	install -d ${bindir} ${docdir}
	install -s ${PROG} ${bindir}
	install -s gam ${bindir}
	install -s sgam ${bindir}
	install -m 0644 LICENSE ${docdir}
	install -m 0644 README.md ${docdir}
uninstall:
	rm -f ${bindir}/${PROG} ${bindir}/gam ${bindir}/sgam
	rmdir ${docdir}
cppcheck:
	cppcheck --enable=all --inconclusive --std=c11 . \
		-DDEBUG 2> cppcheck.log
include depend.mk
depend:
	${CC} -E -MM *.c > depend.mk
#EOF
