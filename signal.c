// batwarn - (C) 2015-2016 Jeffrey E. Bedard

#define _POSIX_C_SOURCE 1
#include <signal.h>
#include <stdlib.h>
#include "log.h"
#include "gamma.h"
#include "config.h"

static void
cb()
{
	batwarn_set_gamma(GAMMA_NORMAL);
	LOG("Signal received!");
	exit(0);
}

void
setup_signal_handler()
{
	struct sigaction sa={.sa_handler=cb};
	sigaction(SIGINT, &sa, NULL);
}
