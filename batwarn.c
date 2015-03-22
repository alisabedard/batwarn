#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <signal.h>

#define LOW_PERCENT 10
#define WAIT 60

#define XGAMMA_CMD "xgamma -q -gamma"
#define GAMMA_NORMAL 1.25
#define GAMMA_WARNING 5.0

uint8_t
get_charge()
{
/* Command to parse */
	const char * command = "ibam --percentbattery";
/* 'ibam --percentbattery | head -n1 | wc' - 3 to determine buffer size */
	const uint8_t size=31;
/* Characters until ':' in program output */
	const uint8_t offset=20;

	FILE *pipe;
	char buf[size];
	uint8_t out;
	
	pipe=popen(command, "r");
	fread(&buf, size, 1, pipe);
	pclose(pipe);
	out=(uint8_t)atoi(buf+offset);
#ifdef DEBUG
	printf("Charge is %d\n", out);
#endif /* DEBUG */

	return out;
}

void
set_gamma(const float value)
{
	char * cmd=XGAMMA_CMD;
	const uint8_t buf_size=14+sizeof(cmd);
	char buf[buf_size];

	snprintf(buf, buf_size, "%s %f", cmd, value);
	system(buf);
}

static void 
signal_handler()
{
	set_gamma(GAMMA_NORMAL);
#ifdef DEBUG
	puts("SIGNAL");
#endif /* DEBUG */
	exit(0);
}

void
setup_signal_handler()
{
	struct sigaction sa;

	sa.sa_handler=signal_handler;
	sigaction(SIGINT, &sa, NULL);
}

static void
handle_low_battery(bool *been_low, bool *gamma_normal)
{
	if(!*been_low)
	{
		set_gamma(GAMMA_WARNING);
		*gamma_normal=false;
		*been_low=true;
	}
}

static void
handle_normal_battery(bool *been_low, bool *gamma_normal)
{
	if(!*gamma_normal)
	{
		set_gamma(GAMMA_NORMAL);
		*gamma_normal=true;
		*been_low=false;
	}
}

int
main()
{
	if(fork()!=0)
		exit(0);
#ifdef DEBUG
	puts("child running");
#endif /* DEBUG */
	nice(19);
	setup_signal_handler();
	check:
	{
		bool been_low, gamma_normal;
		
		gamma_normal=been_low=false;

		if(get_charge()>LOW_PERCENT)
			handle_normal_battery(&been_low, &gamma_normal);
		else
			handle_low_battery(&been_low, &gamma_normal);
		sleep(been_low?1:WAIT);
	}
	goto check;
}

