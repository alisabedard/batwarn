CFLAGS=-Os -g -pipe -W -Wall -Werror

all: batwarn
batwarn: batwarn.o
batwarn.o: batwarn.c
clean:
	rm batwarn.o batwarn
install:
	install -s batwarn /usr/local/bin

