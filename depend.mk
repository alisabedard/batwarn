batwarn.o: batwarn.c batwarn.h config.h gamma.h log.h util.h
gam.o: gam.c gamma.h
gamma.o: gamma.c gamma.h
main.o: main.c batwarn.h config.h gamma.h version.h
sgam.o: sgam.c gamma.h
util.o: util.c util.h
version.o: version.c version.h config.h
