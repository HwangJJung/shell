
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"

pid_t child_pid = -127;

int main()
{
	if (signal(SIGTSTP, sigtstp_handler) == SIG_ERR) {
        fprintf(stderr, "Can't handle SIGINT\n");
        exit(0);  
    }

	int pid, fd;
	SYMBOL term;

	
	print_prompt();


	while (TRUE)
	{
		
		term = parse(&pid, FALSE, NULL);

		 
		
		if (term != S_AMP && pid != 0)
		{
				waitfor(pid);
		}

		if (term == S_NL)
		{
			print_prompt();
		}

		for (fd=3; fd<MAXFD; fd++)
		{
			close(fd);
		}
	}
}


static void sigtstp_handler(int signo)
{
    if (child_pid > 0) {
        kill(child_pid, signo);
        child_pid = -127;
    }
}