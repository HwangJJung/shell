#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include "common.h"


int check_arg(char *av[], const char *opt)
{
	int count = 0;

	//arg 쳌쳌쳌
	while(*av != '\0')
	{
		// 옵션처리.
		if(!strcmp(av[count], opt))
		{
			return TRUE;
		}

		av++;
	}

	return FALSE;
}


void cmd_cd(int ac, char *av[])
{
	char *path;

	// 디렉토리 위치 
	if(ac > 1)
	{
		path = av[1];
	}
	
	// 현재 디렉토리가 HOME이 아니면 . 으로 지정.
	else if((path = (char*)getenv("HOME")) == NULL)
	{
		
		path = ".";
	}

	// 에러가 날 지도 모르죠.
	if(chdir(path) == ERROR)
	{
		fprintf(stderr, "%s: bad directory.\n", path);
	}
}

//강제종료
void cmd_exit()
{
	exit(1);
}

//-a, -l 만 가능한 ls.
void cmd_ls(int ac, char *av[])
{
	DIR *dp;
	struct dirent *entry;
	char *path;
	int count;
	int opt_a;
	int opt_l;

	// ls 만 쳤 을 경 우
	if(ac < 2)
	{
		path = ".";
	}
	
	
	else
	{
		path = av[1];
	}

	// 디렉토리가 안열리면 리스트를 보여줄 수 없어요.
	if((dp = opendir(path)) == NULL)
	{
		fprintf(stderr, "Can't open directory: %s", av[1]);
		return;
	}

	// 옵션이 있나없나. 
	opt_a = check_arg(av, "-a");
	opt_l = check_arg(av, "-l");

	count = 0;

	
	while((entry = readdir(dp)) != NULL)
	{
		// -a 옵션이 있다면. 
		if(!opt_a)
		{
			if((*entry).d_name[0] == '.')
			{
				continue;
			}
		}
		
	
		printf("%s\t", (*entry).d_name);
		
		if(opt_l)
		{
			printf("\n");
		}
		
		
		else
		{
			if(count > 3)
			{
				printf("\n");
				count = 0;
			}
			else
			{
				count++;
			}
		}
	}
	closedir(dp);
	printf("\n");
}


void cmd_cp(int ac, char *av[])
{
	FILE *src;
	FILE *dst;
	char ch;

	// 인자가 최소 3개는 되야합니다.
	if(ac < 3)
	{
		fprintf(stderr, "Not enough arguments.\n");
		return;
	}

	// 소스파일이 열리지 않는경우
	if((src = fopen(av[1], "r")) == NULL)
	{
		fprintf(stderr, "%s: Can't open file.\n", av[1]);
		return;
	}

	// 도착파일이 열리지 않는경우
	if((dst = fopen(av[2], "w")) == NULL)
	{
		fprintf(stderr, "%s: Can't open file.\n", av[2]);
		return;
	}

	
	while(!feof(src))
	{
		ch = (char) fgetc(src);

		if(ch != EOF)
		{
			fputc((int)ch, dst);
		}
	}

	// -v 진행상황 중계 ~
	if(check_arg(av, "-v"))
	{
		printf("cp %s %s\n", av[1], av[2]);
	}

	fclose(src);
	fclose(dst);
}

void cmd_rm(int ac, char *av[])
{
	
	if(ac < 2)
	{
		fprintf(stderr, "Not enough arguments.\n");
		return;
	}

	
	unlink(av[1]);

	
	if(check_arg(av, "-v"))
	{
		printf("rm %s\n", av[1]);
	}
}


void cmd_mv(int ac, char *av[])
{
	FILE *src;
	FILE *dst;
	char ch;

	
	if(ac < 3)
	{
		fprintf(stderr, "Not enough arguments.\n");
		return;
	}


	if((src = fopen(av[1], "r")) == NULL)
	{
		fprintf(stderr, "%s: Can't open file.\n", av[1]);
		return;
	}

	if((dst = fopen(av[2], "w")) == NULL)
	{
		fprintf(stderr, "%s: Can't open file.\n", av[2]);
		return;
	}

	
	while(!feof(src))
	{
		ch = (char) fgetc(src);

		if(ch != EOF)
		{
			fputc((int)ch, dst);
		}
	}

	fclose(src);
	fclose(dst);

	
	unlink(av[1]);

	
	if(check_arg(av, "-v"))
	{
		printf("mv %s %s\n", av[1], av[2]);
	}
}


void cmd_mkdir(int ac, char *av[])
{
	// ÀÎÀÚ°¡ ¾øÀ» °æ¿ì ¿¡·¯
	if(ac < 2)
	{
		fprintf(stderr, "Not enough arguments.\n");
		return;
	}

	// µð·ºÅä¸®¸¦ »ý¼ºÇÑ´Ù.
	if(mkdir(av[1], 0755))
	{
		fprintf(stderr, "Make directory failed.\n");
	}
}


void cmd_rmdir(int ac, char *av[])
{
	
	if(ac < 2)
	{
		fprintf(stderr, "Not enough arguments.\n");
		return;
	}


	if(rmdir(av[1]))
	{
		fprintf(stderr, "Remove directory failed.\n");
	}
}


void cmd_cat(int ac, char *av[])
{
	int ch;
	FILE *fp;

	
	if(ac < 2)
	{
		fprintf(stderr, "Not enough arguments");
		return;
	}


	if((fp = fopen(av[1], "r")) == NULL)
	{
		fprintf(stderr, "No such file on directory.\n");
		return;
	}

	while((ch = getc(fp)) != EOF)
	{
		putchar(ch);
	}

	fclose(fp);
}
