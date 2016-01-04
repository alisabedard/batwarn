// batwarn - (C) 2015-2016 Jeffrey E. Bedard

#include "batwarn.h"

#define _POSIX_C_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <sysexits.h>

static FILE *
try_open(const char *fn)
{
	FILE *f=fopen(fn, "r");
	if(!f) ERROR(EX_NOINPUT, fn);
	return f;
}

static bool
is_on_ac()
{
	char buf;
	FILE *f=try_open(ACSYSFILE);
	const size_t r=fread(&buf, 1, 1, f);
	fclose(f);
	LOG("Buffer for ACSYSFILE: %d", buf-'0');
	return r?buf-'0':true;
}

static int8_t
get_charge()
{
	/* Indicate good battery status when AC power is restored to restore
	   gamma more quickly.  */
	if(is_on_ac())
		return 100;
	const uint8_t sz=4;
	char buf[sz];
	FILE *f=try_open(BATSYSFILE);
	const size_t r=fread(&buf, 1, sz, f);
	if(!r) ERROR(EX_NOINPUT, "Could not get charge");
	fclose(f);
	LOG("Buffer for BATSYSFILE: %s", buf);
	return (uint8_t) atoi(buf);
}

#define BEEN_LOW 1
#define GAMMA_IS_NORMAL 2

static void
handle_low_battery(uint8_t *flags , const uint8_t charge 
#ifndef SUSPEND
	__attribute__((unused)))
#else//SUSPEND
	)
#endif//!SUSPEND
{
	if(!(*flags&BEEN_LOW))
	{
		batwarn_set_gamma(GAMMA_WARNING);
		*flags&=~GAMMA_IS_NORMAL;
		*flags|=BEEN_LOW;
	}
#ifdef SUSPEND
	if(charge < CRIT_PERCENT)
	{
		const int r=system(SUSPEND_CMD);
		LOG("%s RETURNED %d", SUSPEND_CMD, r);
		if(r)
		{
#ifdef STDIO
			fprintf(stderr, "Could not execute %s.",
				SUSPEND_CMD);
#endif//STDIO
			exit(EX_UNAVAILABLE);
		}
	}
#endif//SUSPEND
}

static void
handle_normal_battery(uint8_t *flags, const uint8_t charge)
{
	if(!(*flags&GAMMA_IS_NORMAL))
	{
		batwarn_set_gamma(charge >
			FULL_PERCENT ? GAMMA_FULL : GAMMA_NORMAL);
		*flags|=GAMMA_IS_NORMAL;
		*flags&=~BEEN_LOW;
	}
}

void
batwarn_start_checking()
{
	uint8_t flags=0;
check:
	{
		const uint8_t charge = get_charge();
#ifdef DEBUG
		printf("Charge is %d\n", charge);
#endif /* DEBUG */
		if(charge > LOW_PERCENT)
			handle_normal_battery(&flags, charge);
		else
			handle_low_battery(&flags, charge);
	}
	sleep(flags&BEEN_LOW? 1 : WAIT);
	goto check;
}
