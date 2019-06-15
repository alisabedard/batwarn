// batwarn - (C) 2015-2017 Jeffrey E. Bedard
#include "batwarn.h"
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"
#include "gamma.h"
#include "version.h"
static void exit_cb(void)
{
	batwarn_set_gamma(BATWARN_GAMMA_NORMAL);
}
static void print_version(void)
{
	short s;
	const char * v = batwarn_get_version(&s);
	write(1, v, s);
}
_Noreturn void usage(const int ec)
{
	int fd = open(BATWARN_USAGE, O_RDONLY);
	/* The following is useful for uninstalled binaries, such as on
	 * systems on which you do not have root access, and for viewing
	 * usage prior to installation.  */
	if (fd < 0) // fail to the current directory:
		fd = open("usage.txt", O_RDONLY);
	if (fd >= 0) {
		/* Note:  Adjust this size upwards if usage.txt
		   no longer fits in 512 bytes.  */
		enum { SZ = 512 };
		char buf[SZ];
		// Write how much we read.
		write(1, buf, read(fd, buf, SZ));
		// No need to close since exit will do it for us...
	}
	print_version();
	exit(ec);
}
static uint8_t parse_argv(int argc, char ** argv, uint8_t flags)
{
	int8_t opt;
	const char optstr[] = "dhHp:s";
	while((opt = getopt(argc, argv, optstr)) != -1)
		switch (opt) {
		case 'c': // set critical percentage
			batwarn_set_critical(atoi(optarg));
			break;
		case 'd': // debug
			flags |= BATWARN_DEBUG;
			break;
		case 'H': // enable hibernate at critical
			flags |= BATWARN_HIBERNATE;
			break;
		case 'p': // warning percentage
			batwarn_set_percent(atoi(optarg));
			break;
		case 's': // enable suspend
			flags |= BATWARN_SUSPEND;
			break;
		case 'h': // help
			usage(0);
			//usage(*argv, optstr, sizeof(optstr), 0);
		default: // usage
			usage(1);
			///usage(*argv, optstr, sizeof(optstr), 1);
		}
	return flags;
}
int main(int argc, char **argv)
{
	const uint8_t flags = parse_argv(argc, argv, 0);
	if(!(flags & BATWARN_DEBUG) && (fork() != 0))
		return 0;
	signal(SIGINT, exit);
	signal(SIGTERM, exit);
	atexit(exit_cb);
	batwarn_start_checking(flags);
	return 0;
}
