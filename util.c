// Copyright 2017, Jeffrey E. Bedard
#include "util.h"
#include <stdlib.h> // for abort(), exit()
#include <sys/wait.h> // for wait()
#include <unistd.h> // for write(), fork(), execl()
void bw_print(const char * restrict msg)
{
	unsigned int l = 0;
	while (msg[++l]);
	write(2, msg, l);
}
void bw_die(const char * restrict msg, const char * restrict arg)
{
	bw_print(msg);
	if (arg)
		bw_print(arg);
	bw_print("\n");
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
				bw_print("Command exited abnormally\n");
			else
				bw_print("Command exited normally\n");
		} else if (WIFSIGNALED(s))
			bw_print("Terminated by a signal\n");
	}
}
void bw_execute(const char * restrict cmd)
{
	if (fork() == 0) {
		execl("/bin/sh", "sh", "-c", cmd, NULL);
		bw_print("Cannot execute ");
		bw_print(cmd);
		bw_print("\n");
		exit(1); // error
	} else // in controlling process
		signal (SIGCHLD, sig_child_cb);
}
