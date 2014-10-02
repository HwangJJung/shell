#ifndef __JJUNGSH_H__
#define __JJUNGSH_H__

#define ERROR		(-1)
#define BADFD		(-2)

#define MAXFNAME	10
#define MAXARG		10
#define MAXWORD		20
#define MAXFD		20
#define MAXVAR		50
#define MAXNAME		20

#define TRUE		1
#define FALSE		0

typedef int BOOLEAN;


typedef enum
{
	S_WORD,
	S_BAR,
	S_AMP,
	S_SEMI,
	S_GT,
	S_GTGT,
	S_LT,
	S_NL,
	S_EOF
} SYMBOL;

static void sigtstp_handler (int signo);
int kill (pid_t pid, int signo);

 
#endif
