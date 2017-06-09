// Copyright 2017, Jeffrey E. Bedard
#include "util.h"
#include <fcntl.h> // for open()
#include <stdlib.h> // for abort(), exit()
#include <sys/wait.h> // for wait()
#include <unistd.h> // for write(), fork(), execl()
void batwarn_print(const char * restrict msg)
{
	unsigned int l = 0;
	while (msg[++l]);
	write(2, msg, l);
}
void batwarn_quit(const char * restrict msg, const char * restrict arg)
{
	batwarn_print(msg);
	if (arg)
		batwarn_print(arg);
	batwarn_print("\n");
	exit(1);
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
	if (fork() == 0) {
		execl("/bin/sh", "sh", "-c", cmd, NULL);
		batwarn_print("Cannot execute ");
		batwarn_print(cmd);
		batwarn_print("\n");
		exit(1); // error
	} else // in controlling process
		signal (SIGCHLD, sig_child_cb);
}
static int get_fd(const char * fn)
{
	const int fd = open(fn, O_RDONLY);
	if (fd < 0)
		batwarn_quit("Cannot open ", fn);
	return fd;
}
int bw_get_value(const char * fn)
{
	int fd = get_fd(fn);
	enum {READ_SZ = 4};
	char buf[READ_SZ];
	if (read(fd, buf, READ_SZ) == -1)
		batwarn_quit("Cannot read ", fn);
	close(fd);
	return atoi(buf);
}
