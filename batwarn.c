// batwarn - (C) 2015-2018 Jeffrey E. Bedard
#include "BatwarnFlags.h"
#include "config.h"
#include "gamma.h"
#include <errno.h> // for errno, EWOULDBLOCK
#include <fcntl.h> // for open
#include <stdint.h> // for (u)intN*_t
#include <stdio.h> // for puts, fputs, fprintf
#include <stdlib.h> // for abort, exit
#include <sys/file.h> // for flock, LOCK_EX
#include <sys/wait.h> // for wait
#include <unistd.h> // for write, fork, execl, sleep
typedef int16_t fd_t;
static void check(int fail_condition, const char * restrict message)
{
  if (fail_condition) {
    puts(message);
    exit(1);
  }
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
        puts("Command exited abnormally");
      else
        puts("Command exited normally");
    } else if (WIFSIGNALED(s))
      puts("Terminated by a signal");
  }
}
void batwarn_execute(const char * restrict cmd)
{
  pid_t fval = fork();
  check(fval < 0, "fork failed");
  if(fval) // in parent process
    check(signal(SIGCHLD, sig_child_cb) == SIG_ERR,
      "signal()"); // attach signal handler
  else // in child process
    check(execl("/bin/sh", "sh", "-c", cmd, NULL) < 0, "execl()");
}
int batwarn_get_value(const char * fn)
{
  enum {READ_SZ = 4};
  char buf[READ_SZ];
  fd_t const fd = open(fn, O_RDONLY);
  check(fd < 0, fn);
  read(fd, buf, READ_SZ);
  close(fd);
  return atoi(buf);
}
static uint8_t critical_percent=BATWARN_PERCENT_CRITICAL,
               low_percent=BATWARN_PERCENT_LOW;
void batwarn_set_percent(uint8_t const charge)
{
  low_percent = charge;
}
void batwarn_set_critical(uint8_t const charge)
{
  critical_percent = charge;
}
static uint8_t get_charge(void)
{
  /* Indicate good battery status when AC power is restored to restore
     gamma more quickly.  */
  return batwarn_get_value(BATWARN_SYS_AC_FILE) ? 100 :
    batwarn_get_value(BATWARN_SYS_BATTERY_FILE);
}
static void perform_action_for_charge(uint8_t const charge,
  uint8_t const flags) {
  if (charge < BATWARN_PERCENT_CRITICAL) {
    if (flags & BATWARN_ENABLE_HIBERNATE)
      batwarn_execute(BATWARN_HIBERNATE_COMMAND);
    else if (flags & BATWARN_ENABLE_SUSPEND)
      batwarn_execute(BATWARN_SUSPEND_COMMAND);
  }
}
void batwarn_start_checking(uint8_t flags)
{
  //  batwarn_execute("echo batwarn started at `date`");
  if (!low_percent)
    low_percent = BATWARN_PERCENT_LOW;
  fprintf(stderr, "low_percent: %d\n", low_percent);
  for (;;) {
    uint8_t const charge = get_charge();
    fprintf(stderr, "charge: %d\n", charge);
    batwarn_set_gamma(charge <= BATWARN_PERCENT_LOW ? BATWARN_GAMMA_WARNING :
      BATWARN_GAMMA_NORMAL);
    perform_action_for_charge(charge, flags);
    sleep(3);
  }
}
static void exit_cb(void)
{
  batwarn_set_gamma(BATWARN_GAMMA_NORMAL);
}
_Noreturn void usage(const int ec)
{
  fputs(
    "batwarn -dhHp:s\n"
    "-c PERCENT	Set the critical battery level.\n"
    "-d		Do not fork a daemon; run in the foreground.\n"
    "-h		Show this usage information.\n"
    "-H		Enable hibernation at critical battery level.\n"
    "-p PERCENT	Set the warning percent for gamma change.\n"
    "-s		Enable suspend at critical battery level.\n"
    "Copyright 2017-2020, Jeffrey E. Bedard <jefbed@gmail.com>\n"
    "Version " BATWARN_VERSION, ec ? stderr : stdout
  );
  exit(ec);
}
static uint8_t parse_argv(int argc, char ** argv, uint8_t flags)
{
  int8_t opt;
  const char optstr[] = "c:dhHp:s";
  while((opt = getopt(argc, argv, optstr)) != -1)
    switch (opt) {
    case 'c': // set critical percentage
      batwarn_set_critical(atoi(optarg));
      break;
    case 'd': // debug
      flags |= BATWARN_ENABLE_DEBUG;
      break;
    case 'H': // enable hibernate at critical
      flags |= BATWARN_ENABLE_HIBERNATE;
      break;
    case 'p': // warning percentage
      batwarn_set_percent(atoi(optarg));
      break;
    case 's': // enable suspend
      flags |= BATWARN_ENABLE_SUSPEND;
      break;
    case 'h': // help
      usage(0);
    default: // usage
      usage(1);
    }
  return flags;
}
int main(int argc, char **argv)
{
  /* Use flock to ensure only one instance is running.  This makes
   * it easier to put batwarn in .xinitrc without having to do
   * process management.  TODO:  Allow one instance of batwarn per
   * display.  */
  fd_t const lock_fd = open("/tmp/batwarn.lock", O_RDWR|O_CREAT, 0666);
  flock(lock_fd, LOCK_EX | LOCK_NB);
  if (errno == EWOULDBLOCK) {
    fputs("batwarn already running!\n", stderr);
    exit(1);
  } else {
    const uint8_t flags = parse_argv(argc, argv, 0);

    if ((flags & BATWARN_ENABLE_DEBUG) || !fork()) {
      fputs("batwarn polling...\n", stderr);
      signal(SIGINT, exit);
      signal(SIGTERM, exit);
      atexit(exit_cb);
      batwarn_start_checking(flags);
    }
  }
  return 0;
}
