#define GAM_NAME "gam "
#define GAM_VERSION "version 0.1 "
#define GAM_DESC GAM_NAME "- tiny command line utility to change X11 gamma\n"
#define GAM_COPYING "Copyright 2016, Jeffrey E. Bedard\n"
#include "gamma.h"
#include <stdlib.h>
#include <unistd.h>
#define GAM_OPTS "g:hrv"
#define GAM_HELP GAM_DESC GAM_COPYING \
	"gam -[" GAM_OPTS "] [GAMMA]\n" \
	"[GAMMA]\n" \
	"-g GAMMA\tset gamma value\n" \
	"-r\t\trestore gamma to 1.0\n" \
	"-v\t\tshow gam version\n" \
	"-h\t\tshow gam usage\n"
int main(int argc, char ** argv)
{
	char optstring[] = "g:hrv";
	int opt;
	while ((opt = getopt(argc, argv, optstring)) != -1)
		switch (opt) {
		case 'g':
set_gamma:
			batwarn_set_gamma(atof(optarg));
			return 0;
		case 'r':
			batwarn_set_gamma(1.0);
			return 0;
		case 'v':
			goto version;
		case 'h':
		default:
			goto usage;
		}
	if (argc < 2)
		goto usage;
	optarg=argv[argc - 1];
	goto set_gamma;
usage:
#define PRINT(buf) write(1, buf, sizeof(buf))
	{
		char ubuf[] = GAM_HELP;
		PRINT(ubuf);
	}
version:
	{
		char vbuf[] = GAM_NAME GAM_VERSION "\n";
		PRINT(vbuf);
	}
	return 0;
}
