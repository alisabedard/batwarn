// batwarn - (C) 2015-2016 Jeffrey E. Bedard

#include "batwarn.h"

#include "config.h"
#include "gamma.h"

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

static uint8_t low_percent;

void batwarn_set_percent(const uint8_t pct)
{
	low_percent = pct;
}

__attribute__((noreturn,nonnull(1),pure))
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

static uint8_t handle_low_battery(uint8_t flags , const uint8_t charge)
{
	if (!(flags & BW_BEEN_LOW)) {
		batwarn_set_gamma(GAMMA_WARNING);
		flags &= ~BW_GAMMA_NORMAL;
		flags |= BW_BEEN_LOW;
	}
	if (charge < CRIT_PERCENT) {
		static const char nex[] = "Could not execute command:  ";
		if (flags & BW_HIBERNATE) {
			if (system(HIBERNATE_CMD))
				die(nex, HIBERNATE_CMD);
		} else if (flags & BW_SUSPEND) {
			if (system(HIBERNATE_CMD))
				die(nex, SUSPEND_CMD);
		}
	}
	if (charge < CRIT_PERCENT && system(SUSPEND_CMD))
		die("Could not execute command:  ", SUSPEND_CMD);
	return flags;
}

static uint8_t handle_normal_battery(uint8_t flags)
{
	if (!(flags & BW_GAMMA_NORMAL)) {
		batwarn_set_gamma(GAMMA_NORMAL);
		flags |= BW_GAMMA_NORMAL;
		flags &= ~BW_BEEN_LOW;
	}
	return flags;
}
#ifdef DEBUG
#include <stdio.h>
#endif//DEBUG
void batwarn_start_checking(uint8_t flags)
{
	uint8_t charge;
	if (!low_percent)
		low_percent = LOW_PERCENT;
#ifdef DEBUG
	printf("low_percent: %d\n", low_percent);
#endif//DEBUG
check:
	flags = (charge = get_charge()) > low_percent
		? handle_normal_battery(flags)
		: handle_low_battery(flags, charge);
	sleep(flags & BW_BEEN_LOW? 1 : WAIT);
	goto check;
}

