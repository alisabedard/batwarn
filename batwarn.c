// batwarn - (C) 2015-2016 Jeffrey E. Bedard

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include "batwarn.h"
#include "config.h"
#include "gamma.h"
#include "signal.h"

static void die(const char * msg, const char * arg)
{
	fprintf(stderr, "%s: %s\n", msg, arg);
	exit(1);
}

static FILE * try_open(const char * restrict fn)
{
	FILE * f=fopen(fn,"r");
	if(!f) die("Cannot open file", fn);
	return f;
}

static bool is_on_ac(const uint8_t flags)
{
	char buf;
	FILE *f=try_open(ACSYSFILE);
	const size_t r=fread(&buf, 1, 1, f);
	fclose(f);
	if(flags & BW_DEBUG)
		fprintf(stderr, "Buffer for ACSYSFILE: %d\n", buf-'0');
	return r?buf-'0':true;
}

static int8_t get_charge(const uint8_t flags)
{
	/* Indicate good battery status when AC power is restored to restore
	   gamma more quickly.  */
	if(is_on_ac(flags))
		return 100;
	const uint8_t sz=4;
	char buf[sz];
	FILE *f=try_open(BATSYSFILE);
	const size_t r=fread(&buf, 1, sz, f);
	if(!r) die("Could not read charge", BATSYSFILE);
	fclose(f);
	if(flags & BW_DEBUG)
		fprintf(stderr, "Buffer for BATSYSFILE: %s\n", buf);
	return (uint8_t) atoi(buf);
}

#define BW_BEEN_LOW 1
#define BW_GAMMA_NORMAL 2

static void handle_low_battery(uint8_t *flags , const uint8_t charge)
{
	if(!(*flags&BW_BEEN_LOW)) {
		batwarn_set_gamma(GAMMA_WARNING);
		*flags&=~BW_GAMMA_NORMAL;
		*flags|=BW_BEEN_LOW;
	}
	if(charge < CRIT_PERCENT) {
		const int r=system(SUSPEND_CMD);
		if(*flags & BW_DEBUG)
			fprintf(stderr, "%s RETURNED %d", SUSPEND_CMD, r);
		if(r) {
			fprintf(stderr, "Warning:  Could not execute %s.",
				SUSPEND_CMD);
		}
	}
}

static void handle_normal_battery(uint8_t *flags, const uint8_t charge)
{
	if(!(*flags&BW_GAMMA_NORMAL)) {
		batwarn_set_gamma(charge >
			FULL_PERCENT ? GAMMA_FULL : GAMMA_NORMAL);
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
		handle_normal_battery(&flags, charge);
	else
		handle_low_battery(&flags, charge);
	sleep(flags&BW_BEEN_LOW? 1 : WAIT);
	goto check;
}

