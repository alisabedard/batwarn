// batwarn - (C) 2015-2016 Jeffrey E. Bedard

#include <unistd.h>
#include "batwarn.h"

int
main(void)
{
#ifndef DEBUG
	if(fork() != 0)
		goto end;
#endif//!DEBUG
	setup_signal_handler();
	batwarn_start_checking();

#ifndef DEBUG
      end:
#endif//!DEBUG
	return 0;
}
