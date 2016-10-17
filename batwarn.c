// batwarn - (C) 2015-2016 Jeffrey E. Bedard
#include "batwarn.h"
#include "config.h"
#include "gamma.h"
#include "log.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
static uint8_t low_percent;
void batwarn_set_percent(const uint8_t pct)
{
	low_percent = pct;
}
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
	if (fd < 0)
		die("Cannot open ", fn);
	return fd;
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
	return get_value(BATWARN_SYS_AC_FILE) ? 100 : get_value(BATWARN_SYS_BATTERY_FILE);
}
static uint8_t handle_low_battery(uint8_t flags , const uint8_t charge)
{
	if (!(flags & BW_BEEN_LOW)) {
		batwarn_set_gamma(BATWARN_GAMMA_WARNING);
		flags &= ~BW_BATWARN_GAMMA_NORMAL;
		flags |= BW_BEEN_LOW;
	}
	if (charge < BATWARN_PERCENT_CRITICAL) {
		static const char nex[] = "Could not execute command:  ";
		if (flags & BW_HIBERNATE) {
			if (system(BATWARN_HIBERNATE_COMMAND))
				die(nex, BATWARN_HIBERNATE_COMMAND);
		} else if (flags & BW_SUSPEND) {
			if (system(BATWARN_HIBERNATE_COMMAND))
				die(nex, BATWARN_SUSPEND_COMMAND);
		}
	}
	if (charge < BATWARN_PERCENT_CRITICAL && system(BATWARN_SUSPEND_COMMAND))
		die("Could not execute command:  ", BATWARN_SUSPEND_COMMAND);
	return flags;
}
static uint8_t handle_normal_battery(uint8_t flags)
{
	if (!(flags & BW_BATWARN_GAMMA_NORMAL)) {
		batwarn_set_gamma(BATWARN_GAMMA_NORMAL);
		flags |= BW_BATWARN_GAMMA_NORMAL;
		flags &= ~BW_BEEN_LOW;
	}
	return flags;
}
void batwarn_start_checking(uint8_t flags)
{
	uint8_t charge;
	if (!low_percent)
		low_percent = BATWARN_PERCENT_LOW;
	LOG("low_percent: %d\n", low_percent);
check:
	flags = (charge = get_charge()) > low_percent
		? handle_normal_battery(flags)
		: handle_low_battery(flags, charge);
	LOG("charge: %d\n", charge);
	sleep((flags & BW_BEEN_LOW) ? 1 : BATWARN_WAIT_SECONDS);
	goto check;
}
