// Copyright 2017-2018, Jeffrey E. Bedard
#include "util.h"
#include <fcntl.h> // for open()
#include <stdio.h> // for puts()
#include <stdlib.h> // for abort(), exit()
#include <sys/wait.h> // for wait()
#include <unistd.h> // for write(), fork(), execl()
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
