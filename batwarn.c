// batwarn - (C) 2015-2017 Jeffrey E. Bedard
#include "batwarn.h"
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "config.h"
#include "gamma.h"
#include "log.h"
static uint8_t low_percent;
void batwarn_set_percent(const uint8_t pct)
{
	low_percent = pct;
}
static void print(const char * restrict msg)
{
	uint8_t l = 0;
	while (msg[++l]);
	write(2, msg, l);
}
static void die(const char * restrict msg, const char * restrict arg)
{
	print(msg);
	if (arg)
		print(arg);
	print("\n");
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
	return get_value(BATWARN_SYS_AC_FILE) ? 100
		: get_value(BATWARN_SYS_BATTERY_FILE);
}
static void sig_child_cb(int sig)
{
	if (sig != SIGCHLD)
		abort(); // attached to the wrong signal
	int s;
	// Loop through potentially multiple children
	while (wait(&s) > 0) {
		if (WIFEXITED(s)) {
			if (WEXITSTATUS(s) != 0)
				print("Command exited abnormally\n");
			else
				print("Command exited normally\n");
		} else if (WIFSIGNALED(s))
			print("Terminated by a signal\n");
	}
}
static void execute(const char * cmd)
{
	if (fork() == 0) {
		execl("/bin/sh", "sh", "-c", cmd, NULL);
		print("Cannot execute ");
		print(cmd);
		print("\n");
		exit(1); // error
	} else // in controlling process
		signal (SIGCHLD, sig_child_cb);
}
static void handle_critical_battery(const uint8_t flags)
{
	if (flags & BATWARN_HIBERNATE)
		execute(BATWARN_HIBERNATE_COMMAND);
	else if (flags & BATWARN_SUSPEND)
		execute(BATWARN_SUSPEND_COMMAND);
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
	execute("echo batwarn started at `date`");
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
