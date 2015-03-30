#include "config.h"
#include "gamma.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

static bool
is_on_ac()
{
	FILE *f;
	char buf;

	f = fopen(ACSYSFILE, "r");
	if (!f) {
		perror(ACSYSFILE);
		return false;
	}
	fread(&buf, 1, 1, f);
	fclose(f);

	return (bool) (buf - '0');
}

static uint8_t
get_charge()
{
#ifdef IBAM
	/* Command to parse */
	const char *command = "ibam --percentbattery";
	/* 'ibam --percentbattery | head -n1 | wc' - 3 
	 * to determine buffer size */
	const uint8_t size = 31;
	/* Characters until ':' in program output */
	const uint8_t offset = 20;
#else
	const uint8_t size = 4;
	const char *sysfile = SYSFILE;
#endif

	FILE *file;
	char buf[size];
	uint8_t out;

	/* Indicate good battery status when AC power is restored to restore
	   gamma more quickly.  */
	if (is_on_ac())
		return 100;

#ifdef IBAM	
	file = popen(command, "r");
#else /* !IBAM */
	file = fopen(sysfile, "r");
#endif /* IBAM */
	if (!file)
		exit(1);
	fread(&buf, size, 1, file);

#ifdef IBAM
	pclose(file);
#else /* !IBAM */
	fclose(file);
#endif /* IBAM */
	out = (uint8_t) atoi(buf
#ifdef IBAM
		+ offset
#endif /* IBAM */
		);
#ifdef DEBUG
	printf("Charge is %d\n", out);
#endif /* DEBUG */

	return out;
}

static void
handle_low_battery(bool *been_low, bool *gamma_normal)
{
	if (!*been_low) {
		batwarn_set_gamma(GAMMA_WARNING);
		*gamma_normal = false;
		*been_low = true;
	}
}

static void
handle_normal_battery(bool *been_low, bool *gamma_normal)
{
	if (!*gamma_normal) {
		batwarn_set_gamma(GAMMA_NORMAL);
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

	if (get_charge() > LOW_PERCENT)
		handle_normal_battery(&been_low, &gamma_normal);
	else
		handle_low_battery(&been_low, &gamma_normal);
	sleep(been_low ? 1 : WAIT);
	goto check;
}
