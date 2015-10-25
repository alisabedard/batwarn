#include "config.h"
#include "gamma.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

static FILE *
try_open(const char *fn)
{
	FILE *f;

	f = fopen(fn, "r");
	if(!f)
	{
		perror(fn);
		exit(1);
	}
	return f;
}

static inline bool
is_on_ac()
{
	FILE *f;
	char buf;

	f = try_open(ACSYSFILE);
	fread(&buf, 1, 1, f);
	fclose(f);

	return (bool) (buf - '0');
}

static inline uint8_t
get_charge()
{
	const uint8_t size = 4;

	FILE *f;
	char buf[size];
	uint8_t out;

	/* Indicate good battery status when AC power is restored to restore
	   gamma more quickly.  */
	if(is_on_ac())
		return 100;
	f = try_open(BATSYSFILE);
	fread(&buf, size, 1, f);
	fclose(f);
	out = (uint8_t) atoi(buf);

	return out;
}

static void
handle_low_battery(bool * been_low, bool * gamma_normal,
	const uint8_t charge)
{
	if(!*been_low)
	{
		batwarn_set_gamma(GAMMA_WARNING);
		*gamma_normal = false;
		*been_low = true;
	}
	if(charge < CRIT_PERCENT)
	{
		system(SUSPEND_CMD);
	}
}

static void
handle_normal_battery(bool * been_low, bool * gamma_normal,
	const uint8_t charge)
{
	if(!*gamma_normal)
	{
		batwarn_set_gamma(charge >
			FULL_PERCENT ? GAMMA_FULL : GAMMA_NORMAL);
		*gamma_normal = true;
		*been_low = false;
	}
}

void
batwarn_start_checking()
{
	bool been_low, gamma_normal;
      check:
	gamma_normal = been_low = false;
	{
		uint8_t charge;

		charge = get_charge();
#ifdef DEBUG
		printf("Charge is %d\n", charge);
#endif /* DEBUG */
		if(charge > LOW_PERCENT)
			handle_normal_battery(&been_low,
				&gamma_normal, charge);
		else
			handle_low_battery(&been_low, &gamma_normal,
				charge);
	}
	sleep(been_low ? 1 : WAIT);
	goto check;
}
