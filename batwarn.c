// batwarn - (C) 2015-2016 Jeffrey E. Bedard

#include "batwarn.h"

#include "config.h"
#include "gamma.h"

#include <fcntl.h>
#include <stdlib.h>
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

static int8_t get_charge(void)
{
	/* Indicate good battery status when AC power is restored to restore
	   gamma more quickly.  */
	return get_value(ACSYSFILE) ? 100 : get_value(BATSYSFILE);
}

__attribute__((nonnull))
static uint8_t handle_low_battery(uint8_t flags , const uint8_t charge)
{
	if (!(flags & BW_BEEN_LOW)) {
		batwarn_set_gamma(GAMMA_WARNING);
		flags &= ~BW_GAMMA_NORMAL;
		flags |= BW_BEEN_LOW;
	}
	if (charge < CRIT_PERCENT && system(SUSPEND_CMD))
		die("Could not execute command:  ", SUSPEND_CMD);
	return flags;
}

__attribute__((nonnull))
static uint8_t handle_normal_battery(uint8_t flags)
{
	if (!(flags & BW_GAMMA_NORMAL)) {
		batwarn_set_gamma(GAMMA_NORMAL);
		flags |= BW_GAMMA_NORMAL;
		flags &= ~BW_BEEN_LOW;
	}
	return flags;
}

void batwarn_start_checking(uint8_t flags)
{
	uint8_t charge;
check:
	charge = get_charge();
	flags = charge > LOW_PERCENT ? handle_normal_battery(flags)
		: handle_low_battery(flags, charge);
	sleep(flags & BW_BEEN_LOW? 1 : WAIT);
	goto check;
}

