/* 
 * File:   main.c
 * Author: Jeffrey E. Bedard
 *
 * Created on March 27, 2015, 11:40 PM
 */

#include <unistd.h>
#include <sysexits.h>
#include <stdio.h>
#include <stdlib.h>
#include "signal.h"
#include "batwarn.h"

static void
parse_command_line(int argc, char** argv)
{  
	int opt;
	
           while ((opt = getopt(argc, argv, "")) != -1) {
		   switch(opt) {
               default: 
                   fprintf(stderr, "Usage: %s\n", argv[0]);
                   exit(EX_USAGE);
               }
           }
}

int
main(int argc, char** argv)
{
	parse_command_line(argc, argv);
#ifndef DEBUG
	if (fork() != 0)
		goto end;
#endif /* !DEBUG */
	nice(19);
	setup_signal_handler();
	batwarn_start_checking();

end:
	return EX_OK;
}