// Copyright 2016, Jeffrey E. Bedard
#include "gamma.h"
#include <stdlib.h>
/* This is a bare bones utility to set the X11 display gamma from
   the command line.  It is suitable for use in shell scripts.  */
int main(int argc, char ** argv)
{
	if (argc < 2) // invalid arguments
		return 1;
	batwarn_set_gamma(atof(argv[1]));
	return 0;
}
