// batwarn - (C) 2015-2016 Jeffrey E. Bedard

#define _POSIX_C_SOURCE 1
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "gamma.h"
#include "config.h"

static void
signal_handler()
{
	batwarn_set_gamma(GAMMA_NORMAL);
#ifdef DEBUG
	puts("SIGNAL");
#endif /* DEBUG */
	exit(0);
}

void
setup_signal_handler()
{
	struct sigaction sa;

	sa.sa_handler = signal_handler;
	sigaction(SIGINT, &sa, NULL);
}
