// batwarn - (C) 2015-2016 Jeffrey E. Bedard #include <stdbool.h>
#include "batwarn.h"
#include "config.h"
#include "gamma.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static const char * restrict helptext =
"USAGE:  batwarn [-d][-h]\n"
"DESCRIPTION:  batwarn is a simple utility to change X11 gamma to reflect\n"
"       battery status, such that the user may promptly restore AC power.\n"
"\t-d    enable runtime debug options, do not fork\n"
"\t-h    show this usage information\n"
"Copyright 2016, Jeffrey E. Bedard\n"
"Project page:  https://github.com/jefbed/batwarn\n";

static void exit_cb(void)
{
	fputs("Restoring gamma...\n", stderr);
	batwarn_set_gamma(GAMMA_NORMAL);
}

int main(int argc, char **argv)
{
	static uint8_t flags;
	int opt;
	while((opt=getopt(argc, argv, "dh")) != -1) {
		switch(opt) {
		case 'd':
			flags |= BW_DEBUG;
			break;
		case 'h':
		default:
			fputs(helptext, stderr);
			return 1;
		}
	}

	if(!(flags & BW_DEBUG) && (fork() != 0))
		return 0;
	signal(SIGINT, exit);
	atexit(exit_cb);
	batwarn_start_checking(flags);
	return 0;
}

