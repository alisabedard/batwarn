// batwarn - (C) 2015-2016 Jeffrey E. Bedard

#include "signal.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "gamma.h"
#include "config.h"

static void restore_gamma(void) // Program exited
{
	fputs("Restoring gamma...\n", stderr);
	batwarn_set_gamma(GAMMA_NORMAL);
}

__attribute__((noreturn))
static void cb() // Signal received
{
	fputs("Signal received...\n", stderr);
	exit(1); // calls restore_gamma via atexit.
}

void setup_signal_handler(void)
{
	sigaction(SIGINT, &(struct sigaction){.sa_handler=cb}, NULL);
	atexit(restore_gamma);
}
