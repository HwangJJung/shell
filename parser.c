#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "symbol.c"


SYMBOL parse(int *waitpid, BOOLEAN makepipe, int *pipefdp)
{
	SYMBOL symbol, term;
	int argc, sourcefd, destfd;
	int pid, pipefd[2];
	char *argv[MAXARG+1], sourcefile[MAXFNAME];
	char destfile[MAXFNAME];
	char word[MAXWORD];
	BOOLEAN append;

	argc = 0;			
	sourcefd = 0;		
	destfd = 1;	

	while (TRUE) 
	{
		// 어느 단어인지 분석한다.
		switch (symbol = getsymbol(word))		
		{
			// 일반적인 단어일 경우, 최대 인자값을 넘었는지 확인한다. 
			case S_WORD :					
				if(argc == MAXARG) 
				{
					fprintf(stderr, "Too many args.\n");
					break;
				}

				// 인자값만큼 메모리를 할당한다.
				argv[argc] = (char *) malloc(strlen(word)+1);
				
				if(argv[argc] == NULL) 
				{
					fprintf(stderr, "Out of arg memory.\n");
					break;
				}

				// 인자의 값들을 word에 카피한다.
				strcpy(argv[argc], word);
				
				// 그후  argc 를 증가 시킨다.
				argc++;
				continue;

			// < 파이프
			case S_LT   : 
			
				// 파이프 에러 체크. 문자가 한번 더 입력되지 않도록.
				if(makepipe) 
				{
					fprintf(stderr, "Extra <.\n");
					break;
				}

				// 일반 문자가 아닐경우 에러 처리.
				if(getsymbol(sourcefile) != S_WORD) 
				{
					fprintf(stderr, "Illegal <.\n");
					break;
				}

				sourcefd = BADFD;
				continue;

			// > 리 다이렉션
			case S_GT   :
			case S_GTGT :

				// 이미 리다이렉션 된 경우.
				if(destfd != 1) 
				{
					fprintf(stderr, "Extra > or >>.\n");
					break;
				}

				// 도착 파일명이 일반 문자가 아닐경우
				if(getsymbol(destfile) != S_WORD) 
				{
					fprintf(stderr, "Illegal > or >>.\n");
					break;
				}

				// 
				destfd = BADFD;
				append = (symbol == S_GTGT);
				continue;

			// |, &, ;, \n 등의 특수 문자 처리.
			case S_BAR  :
			case S_AMP  :
			case S_SEMI :
			case S_NL   :
			
				argv[argc] = NULL;
				// 파이프
				if(symbol == S_BAR) 
				{	
					//리다이렉션과 엮일 경우
					if(destfd != 1) 
					{
						fprintf(stderr, "> or >> conflicts with |.\n");
						break;
					}
					// 파이프 만들기
					term = parse(waitpid, TRUE, &destfd);
				}
				
				
				else
				{
					term = symbol;
				}

				if (makepipe) 
				{
					if (pipe(pipefd) == ERROR)
					{
						syserr("pipe");
					}
					*pipefdp = pipefd[1];
					sourcefd = pipefd[0];
				}

				// 실제로 맞는 명령어 execute
				pid = execute(argc, argv, sourcefd, sourcefile,
								destfd, destfile,append, term == S_AMP);

				// 파이프 문자가 아닐 경우
				if (symbol != S_BAR)
				{
					*waitpid = pid;
				}

				
				if (argc == 0 && (symbol != S_NL || sourcefd > 1))
				{
					fprintf(stderr, "Missing command.\n");
				}

				
				while (--argc >= 0)
				{
					free(argv[argc]);
				}

				return term;

			case S_EOF : 
				exit(0);
		} 
	}    		
}