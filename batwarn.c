// batwarn - (C) 2015-2016 Jeffrey E. Bedard

#include "batwarn.h"

#include "config.h"
#include "gamma.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

__attribute__((noreturn,nonnull(1)))
static void die(const char * restrict msg, const char * restrict arg)
{
	uint_fast16_t l = 0;
	while(msg[++l]);
	write(2, msg, l);
	if (arg) {
		l = 0;
		while(arg[++l]);
		write(2, arg, l);
	}
	write(2, "\n", 1);
	exit(1);
}

static int get_fd(const char * fn)
{
	const int fd = open(fn, O_RDONLY);
	if (fd > -1)
		return fd;
	die("Cannot open ", fn);
}

static int get_value(const char * fn)
{
	int fd = get_fd(fn);
enum {READ_SZ = 4};
	char buf[READ_SZ];
	if (read(fd, buf, READ_SZ) == -1)
		die("Cannot read ", fn);
	close(fd);
	return atoi(buf);
}

static int8_t get_charge(const uint8_t flags)
{
	/* Indicate good battery status when AC power is restored to restore
	   gamma more quickly.  */
	if(get_value(ACSYSFILE))
		return 100;
	enum { BATSYSFILE_BUF_SZ=4 };
	char buf[BATSYSFILE_BUF_SZ];
	int fd = open(BATSYSFILE, O_RDONLY);
	read(fd, buf, BATSYSFILE_BUF_SZ);
	close(fd);
	if(flags & BW_DEBUG)
		fprintf(stderr, "Buffer for BATSYSFILE: %s\n", buf);
	return (uint8_t) atoi(buf);
}

static void handle_critical_battery(const bool debug)
{
	// Attempt to suspend the system
	const int r=system(SUSPEND_CMD);
	if(debug)
		fprintf(stderr, "%s RETURNED %d", SUSPEND_CMD, r);
	if(r) {
		fprintf(stderr, "Warning:  Could not execute %s.",
			SUSPEND_CMD);
	}
}

__attribute__((nonnull))
static void handle_low_battery(uint8_t *flags , const uint8_t charge)
{
	if(!(*flags&BW_BEEN_LOW)) {
		batwarn_set_gamma(GAMMA_WARNING);
		*flags&=~BW_GAMMA_NORMAL;
		*flags|=BW_BEEN_LOW;
	}
	if(charge < CRIT_PERCENT)
		  handle_critical_battery(*flags & BW_DEBUG);
}

__attribute__((nonnull))
static void handle_normal_battery(uint8_t *flags)
{
	if(!(*flags&BW_GAMMA_NORMAL)) {
		batwarn_set_gamma(GAMMA_NORMAL);
		*flags|=BW_GAMMA_NORMAL;
		*flags&=~BW_BEEN_LOW;
	}
}

void batwarn_start_checking(uint8_t flags)
{
	uint8_t charge;
check:
	charge = get_charge(flags);
	if(flags & BW_DEBUG)
		fprintf(stderr, "Charge is %d\n", charge);
	if(charge > LOW_PERCENT)
		handle_normal_battery(&flags);
	else
		handle_low_battery(&flags, charge);
	sleep(flags&BW_BEEN_LOW? 1 : WAIT);
	goto check;
}

