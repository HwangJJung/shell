#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#include "jjung_shell.h"
#include "util.h"
#include "common.h"

/* 상수 정의 */
#define MAXLINE		1024
#define MAXARGS		128
#define MAXPATH		1024
#define MAXTHREAD	10


#define DEFAULT_FILE_MODE	0664
#define DEFAULT_DIR_MODE	0775
#define BADFD		(-2)


/* 전역 변수 정의 */
const char delim[] = " \t\n";
pid_t child_pid = -127;

/* 전역 변수 선언 */


/* 함수 선언 */
void myshell_error(char *err_msg);
void process_cmd(char *cmdline);
int builtin_cmd(int argc, char **argv);
int parse_line(char *line, char **argv);
SYMBOL getsymbol(char *word);

// 내장 명령어 처리 함수
int list_files(int argc, char **argv);
int copy_file(int argc, char **argv);
int remove_file(int argc, char **argv);
int move_file(int argc, char **argv);
int change_directory(int argc, char **argv);
int print_working_directory(void);
int make_directory(int argc, char **argv);
int remove_directory(int argc, char **argv);
int copy_directory(int argc, char **argv);



/*
 * main - MyShell's main routine
 */
int main(int argc, char *argv[])
{

	
	
	pid_t	pid;
	int status;		/* child 의 exit 상태 */
	int ret;		/* waitpid 의 리턴 값 */
	char hostname[128];		/* host name이 128 보단 작겠죠 */

	if(gethostname (hostname,128) < 0 )
		strcpy(hostname, "jjung-mac");

	char line[MAXLINE];

	/* 명령어 처리 루프: 셸 명령어를 읽고 처리한다. */
	while (1) {

		// bg 프로세스 기다리기
  		ret = waitpid(-1, &status, WNOHANG);
        if (ret < 0 && errno != ECHILD) /* 자식이 없으면 OK */
            perror("jjung_shell: main");
        else if (ret > 0)
            printf("Child %d exited with status %d\n", ret, status);


        /* 프롬프트에 뿌려주고 다음 줄 얻어오기 */

        printf("<%s@%s> ", getenv("USER"), hostname );
        fflush(stdout);

		if (fgets(line, MAXLINE	, stdin)) {
			if(!strcmp(line, "\n"))	//아무런 값도 없으면 넘어간다.
				continue;

			if (!strcmp(line,"exit\n")) //exit 입력하면 끝내기 
				break;

			process_cmd(line);

		}else {
			if (errno && (errno != ECHILD)) {
				if (errno == EINTR) {
					printf("인터럽당함, 한번더 해보자.\n");
					continue;
				}
				perror("jjung_shell: main");
				exit(EXIT_FAILURE);
			} else
			/* EOF */
				break;
		}
	}	
	/* 프로그램이 도달하지 못한다. */
	return 0;
}


/*
 * process_cmd
 *
 * 명령 라인을 인자 (argument) 배열로 변환한다.
 * 내장 명령 처리 함수를 수행한다.
 * 내장 명령이 아니면 자식 프로세스를 생성하여 지정된 프로그램을 실행한다.
 * 파이프(|)를 사용하는 경우에는 두 개의 자식 프로세스를 생성한다.
 */
void process_cmd(char *line)
{
	int argc;
	char *argv[MAXARGS];
	SYMBOL term;
	int pid;

	// 개행 제거하기 
	
	argc = parse_line(line,argv);

	// 인자가 몇개인지 찾습니다. 파이프도 찾아봅시다.

	

	/* 내장 명령 처리 함수를 수행한다. */
	//if (builtin_cmd(argc, argv) == 0) {

		// 내장 명령 처리를 완료하고 리턴한다.
	//	return;
	//}

	//if (nargs == 1) {
        /* 명령어 하나로만 실행 */
    //    int ret = start_prog(0, 1, commands[0].argv[0], commands[0].argc, commands[0].argv, 0, 1);
	//free_commands(commands, nargs);
	//free(commands);
	//return ret;
    //}


	/*
	 * 자식 프로세스를 생성하여 프로그램을 실행한다.
	 */

	// 프로세스 생성


	// 자식 프로세스에서 프로그램 실행


	// 파이프 실행이면 자식 프로세스를 하나 더 생성하여 파이프로 연결


	// foreground 실행이면 자식 프로세스가 종료할 때까지 기다린다.

	return;
}


/*
 * parse_line
 *
 * 명령 라인을 인자(argument) 배열로 변환한다.
 * 인자의 개수(argc)를 리턴한다.
 * 파이프와 백그라운드 실행, 리다이렉션을 해석하고 flag와 관련 변수를 
 *   설정한다.
 */

int parse_line(char *line , char **argv)
{	
	char *ptr;
	strcpy(argv,line);
	int narg=1;

	ptr = strtok(argv,delim);
	printf("%s\n", ptr);

	while(ptr = strtok(NULL,delim))
	{
		printf("%s\n", ptr);
		narg++;
	}
	printf("%n", narg);
	argv = malloc((narg +1) * sizeof(char *));
	printf("%d\n", sizeof(argv));
	return sizeof(argv);

	free(argv);
}


/*
 * builtin_cmd
 *
 * 내장 명령을 수행한다.
 * 내장 명령이 아니면 1을 리턴한다.
 */
int builtin_cmd(int argc, char **argv)
{

	// 내장 명령어 문자열과 argv[0]을 비교하여 각각의 처리 함수 호출
	if ((!strcmp(argv[0], "quit") || (!strcmp(argv[0], "exit")))) {
		exit(0);
	}


	// 내장 명령어가 아님.
	return 1;
}

/*
 * 
 * 내장 명령 처리 함수들
 * argc, argv를 인자로 받는다.
 * 
 */
 

void close_pipe(int fd)
{
    int ret;
    while ((ret = close(fd))) {
        if (ret == EBADF || ret == EIO) {
            perror("jjung_shell: close_pipe");
            exit(EXIT_FAILURE);
        }
    }
}

void free_commands(struct command *cmds, int len)
{
    int i;
    for (i = 0; i < len; i++) {
	free(cmds[i].argv);
    }
}

int list_files(int argc, char **argv)
{
	return 0;
}


int copy_file(int argc, char **argv)
{
	return 0;
}


int remove_file(int argc, char **argv)
{
	return 0;
}


int move_file(int argc, char **argv)
{
	return 0;
}


int change_directory(int argc, char **argv)
{
	return 0;
}


int print_working_directory(void)
{
	return 0;
}


int make_directory(int argc, char **argv)
{
	return 0;
}


int remove_directory(int argc, char **argv)
{
	return 0;
}


int copy_directory(int argc, char **argv)
{
	return 0;
}

typedef enum {NEUTRAL, GTGT, INQUOTE, INWORD} STATUS;

SYMBOL getsymbol(char *word)
{
	STATUS state;
	int c;
	char *w;
	
	state = NEUTRAL;
	w = word;

	while ((c = getchar()) != EOF) 
	{
		switch (state) 
		{
			case NEUTRAL :
				switch (c) 
				{
					case ';' :
						return S_SEMI;

					case '&' :
						return S_AMP;

					case '|' :
						return S_BAR;

					case '<' :
						return S_LT;

					case '\n':
						return S_NL;

					case ' ' :
					case '\t':
						continue;

					case '>' : 
						state = GTGT;
						continue;

					case '"' : 
						state = INQUOTE;
						continue;

					default  : 
						state = INWORD;
						*w++ = c;
						continue;
				}

			case GTGT:
				if (c == '>')
				{
					return S_GTGT;
				}
				ungetc(c, stdin);
				return S_GT;

			case INQUOTE:
				switch (c) 
				{
					case '\\' : 
						*w++ = getchar();
						continue;

					case '"'  : 
						*w = '\0';
						return S_WORD;

					default   : 
						*w++ = c;
						continue;
				}

			case INWORD:
				switch (c ) 
				{
					case ';' :
					case '&' :
					case '|' :
					case '<' :
					case '>' :
					case '\n':
					case ' ' :
					case '\t': 
						ungetc(c, stdin);
						*w = '\0';
						return S_WORD;

					default  : 
						*w++ = c;
						continue;
				}
		}
	}

   return S_EOF;
}

