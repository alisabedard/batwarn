// batwarn - (C) 2015-2016 Jeffrey E. Bedard

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "batwarn.h"
#include "config.h"
#include "gamma.h"
#include "signal.h"

static const char * restrict helptext =
"USAGE:  batwarn [-d][-h]\n"
"DESCRIPTION:  batwarn is a simple utility to change X11 gamma to reflect\n"
"       battery status, such that the user may promptly restore AC power.\n"
"\t-d    enable runtime debug options, do not fork\n"
"\t-h    show this usage information\n"
"Copyright 2016, Jeffrey E. Bedard\n"
"Project page:  https://github.com/jefbed/batwarn\n";

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
	setup_signal_handler();
	batwarn_start_checking(flags);
	return 0;
}

