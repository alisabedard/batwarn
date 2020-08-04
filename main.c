// batwarn - (C) 2015-2017 Jeffrey E. Bedard
#include "batwarn.h"
#include "config.h"
#include "gamma.h"
#include "util.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <unistd.h>

static void exit_cb(void)
{
  batwarn_set_gamma(BATWARN_GAMMA_NORMAL);
}
_Noreturn void usage(const int ec)
{
  puts(
    "batwarn -dhHp:s\n"
    "-c PERCENT	Set the critical battery level.\n"
    "-d		Do not fork a daemon; run in the foreground.\n"
    "-h		Show this usage information.\n"
    "-H		Enable hibernation at critical battery level.\n"
    "-p PERCENT	Set the warning percent for gamma change.\n"
    "-s		Enable suspend at critical battery level.\n"
    "Copyright 2017-2019, Jeffrey E. Bedard <jefbed@gmail.com>\n"
    "Version " BATWARN_VERSION
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
      //usage(*argv, optstr, sizeof(optstr), 0);
    default: // usage
      usage(1);
      ///usage(*argv, optstr, sizeof(optstr), 1);
    }
  return flags;
}
int main(int argc, char **argv)
{
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
