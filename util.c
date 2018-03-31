// Copyright 2017-2018, Jeffrey E. Bedard
#include "util.h"
#include <fcntl.h> // for open()
#include <stdlib.h> // for abort(), exit()
#include <sys/wait.h> // for wait()
#include <unistd.h> // for write(), fork(), execl()
typedef char fd_t;
static void batwarn_print(const char * restrict msg)
{
	unsigned short l = 0;
	while (msg[++l]);
	write(STDERR_FILENO, msg, l);
}
static void check(int fail_condition, const char * restrict message)
{
	if (fail_condition) {
		batwarn_print(message);
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
				batwarn_print("Command exited abnormally\n");
			else
				batwarn_print("Command exited normally\n");
		} else if (WIFSIGNALED(s))
			batwarn_print("Terminated by a signal\n");
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
static fd_t get_fd(const char * fn)
{
	const fd_t fd = open(fn, O_RDONLY);
	check(fd < 0, fn);
	return fd;
}
int bw_get_value(const char * fn)
{
	const fd_t fd = get_fd(fn);
	enum {READ_SZ = 4};
	char buf[READ_SZ];
	check(read(fd, buf, READ_SZ) < 0, fn);
	check(close(fd) < 0, "close()");
	return atoi(buf);
}
