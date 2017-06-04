// batwarn - (C) 2015-2017 Jeffrey E. Bedard
#include "batwarn.h"
#include <fcntl.h> // for open()
#include <stdlib.h> // for atoi()
#include <unistd.h> // for sleep()
#include "config.h"
#include "gamma.h"
#include "log.h"
#include "util.h"
static uint8_t low_percent;
void batwarn_set_percent(const uint8_t pct)
{
	low_percent = pct;
}
static int8_t get_charge(void)
{
	/* Indicate good battery status when AC power is restored to restore
	   gamma more quickly.  */
	return bw_get_value(BATWARN_SYS_AC_FILE) ? 100
		: bw_get_value(BATWARN_SYS_BATTERY_FILE);
}
static void handle_critical_battery(const uint8_t flags)
{
	if (flags & BATWARN_HIBERNATE)
		bw_execute(BATWARN_HIBERNATE_COMMAND);
	else if (flags & BATWARN_SUSPEND)
		bw_execute(BATWARN_SUSPEND_COMMAND);
}
static uint8_t handle_low_battery(uint8_t flags , const uint8_t charge)
{
	if (!(flags & BATWARN_BEEN_LOW)) {
		batwarn_set_gamma(BATWARN_GAMMA_WARNING);
		flags &= ~BATWARN_BATWARN_GAMMA_NORMAL;
		flags |= BATWARN_BEEN_LOW;
	}
	if (charge < BATWARN_PERCENT_CRITICAL)
		handle_critical_battery(flags);
	return flags;
}
static uint8_t handle_normal_battery(uint8_t flags)
{
	if (!(flags & BATWARN_BATWARN_GAMMA_NORMAL)) {
		batwarn_set_gamma(BATWARN_GAMMA_NORMAL);
		flags |= BATWARN_BATWARN_GAMMA_NORMAL;
		flags &= ~BATWARN_BEEN_LOW;
	}
	return flags;
}
uint8_t get_flags(const uint8_t charge, const uint8_t flags)
{
	return charge > low_percent ? handle_normal_battery(flags)
		: handle_low_battery(flags, charge);
}
void batwarn_start_checking(uint8_t flags)
{
	bw_execute("echo batwarn started at `date`");
	// Delay for checking system files:
	enum {
#ifdef DEBUG
		BATWARN_WAIT_SECONDS = 1
#else//!DEBUG
		BATWARN_WAIT_SECONDS = 60
#endif//DEBUG
	};
	if (!low_percent)
		low_percent = BATWARN_PERCENT_LOW;
	LOG("low_percent: %d\n", low_percent);
check:
	flags = get_flags(get_charge(), flags);
	LOG("charge: %d\n", charge);
	// If low, check more frequently to restore gamma quickly:
	sleep((flags & BATWARN_BEEN_LOW) ? 1 : BATWARN_WAIT_SECONDS);
	goto check;
}
