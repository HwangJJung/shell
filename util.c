#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "common.h"


void fatal(char *message)
{
	fprintf(stderr, "Error: %s\n", message);
	exit(1); 
}

void syserr(char *message)
{
	fprintf(stderr, "Error: %s (%d", message, errno);
	exit(1);
}

void waitfor(int pid)
{
	int wpid, status;

	while ((wpid = wait(&status)) != pid && wpid != ERROR);
}

void print_prompt()
{
	char *ps;
	char *index;

	
	if((ps = (char*)getenv("PS2")) != NULL)
	{
		
		while(*ps != '\0')
		{
			
			if(*ps == '\\')
			{
				ps++;

				
				if(*ps == 'u')
				{
					printf("%s", getenv("USER"));
				}
				
				
				else if(*ps == 'h')
				{
					printf("%s", getenv("HOSTNAME"));
				}
				
				
				else if(*ps == 'w')
				{
					printf("%s", (char*) getcwd());
				}
			}
			
			else
			{
				printf("%c", *ps);
				ps++;
			}
		}
	}
	
	else
	{
		printf(">> ");
	}
}

BOOLEAN shellcmd(int ac, char *av[], int sourcefd, int destfd)
{
	char *path;

	// cd
	if(!strcmp(av[0], "cd"))
	{
		cmd_cd(ac, av);
	}
	// ls
	else if(!strcmp(av[0], "ls"))
	{
		cmd_ls(ac, av);
	}
	// cp
	else if(!strcmp(av[0], "cp"))
	{
		cmd_cp(ac, av);
	}
	// rm
	else if(!strcmp(av[0], "rm"))
	{
		cmd_rm(ac, av);
	}
	// mv
	else if(!strcmp(av[0], "mv"))
	{
		cmd_mv(ac, av);
	}
	// mkdir
	else if(!strcmp(av[0], "mkdir"))
	{
		cmd_mkdir(ac, av);
	}
	// rmdir
	else if(!strcmp(av[0], "rmdir"))
	{
		cmd_rmdir(ac, av);
	}
	// cat
	else if(!strcmp(av[0], "cat"))
	{
		cmd_cat(ac, av);
	}
	// exit
	else if(!strcmp(av[0], "exit"))
	{
		cmd_exit();
	}
	else
	{
		return FALSE;
	}

	if (sourcefd != 0 || destfd != 1)
	{
		fprintf(stderr, "Ilegal redirection or pipeline.\n");
	}

	return TRUE;
}

