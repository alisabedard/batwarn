// batwarn - (C) 2015-2016 Jeffrey E. Bedard
#include "batwarn.h"
#include "config.h"
#include "gamma.h"

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

static void exit_cb(void)
{
	batwarn_set_gamma(GAMMA_NORMAL);
}

int main(int argc, char **argv)
{
	uint8_t flags = 0;
	static const char optstr[] = "dhHp:s";
	int8_t opt;
	int8_t ec = 1;
	while ((opt = getopt(argc, argv, optstr)) != -1) {
		switch (opt) {
		case 'd': // debug
			flags |= BW_DEBUG;
			break;
			exit(0);
		case 'H': // enable hibernate at critical
			flags |= BW_HIBERNATE;
			break;
		case 'p': // warning percentage
			batwarn_set_percent(atoi(optarg));
			break;
		case 's': // enable suspend
			flags |= BW_SUSPEND;
			break;
		case 'h': // help
			ec = 0; // fall through
		default: { // usage
			uint8_t l = 0;
			while (argv[0][++l]);
			write(1, *argv, l);
			write(1, " -", 2);
			write(1, optstr, sizeof(optstr));
			write(1, "\n", 1);
			return ec;
		}
		}
	}
	if(!(flags & BW_DEBUG) && (fork() != 0))
		return 0;
	signal(SIGINT, exit);
	atexit(exit_cb);
	batwarn_start_checking(flags);
	return ec;
}

