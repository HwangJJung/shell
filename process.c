if(buf[strlen(cmdline) - 1] == "\n")
				buf[strlen(cmdline) - 1] = 0;

			if((pid = fork()) < 0) {
				err_sys("fork Error");
			} else if ( pid == 0) { 	// 자식 프로세스 
				execlp(buf, buf, (char *) 0);
				err_ret("couldn't execute: %s" , buf);
				exit(127);
			}

			if ((pid = waitpid(pid, &status, 0)) < 0){
				err_sys("waitpid error");
				printf("%s" , prompt);
			}
		}

		// 명령 라인 읽기 
		if (fgets(cmdline, MAXLINE, stdin) == NULL) {
			return 1;
		}

		// 명령 라인 처리
		process_cmd(cmdline);

		fflush(stdout);
	}
		

		/* Ctrl-z 시그널 처리.
	* shell이 스톱하면 안되잖아요. 되나 이거... */

	if(signal(SIGTSTP, sigtstp_handler) == SIG_ERR) {
		fprintf(stderr, "Can't handle SIGINT\n");
		exit(EXIT_FAILURE);
	}

static void sigtstp_handler (int signo);

int nargs = 1;
	int i;

	for(i= 0; i < strlen(line); i++) {
		if (line[i] == '|' && ( i != 0 && i != strlen(line) - 1))
			nargs++;
		else if (line[i] == '|' && (i == 0 || i == strlen(line) - 1))
			/* Syntax error */
			return 1;
	}
	
	struct command *commands = malloc(nargs * sizeof(struct command));
    int cur_arg;
    for (cur_arg = 0; cur_arg < nargs; cur_arg++) {
    	char *arg;
    	if (cur_arg == 0)
    		arg = strtok(line, "|");
    	else
    		arg = strtok(NULL, "|");
    	//파이프 단위로 자른다.
 
    	char *ptr = arg;
    	char last = ' ';
    	int toks= 1;

 	  	//옵션이 몇개 있는지 센다.

    	while(*ptr) {
    		// <ctype.h>에 있는 함수. 공백인지 검사해준다.
    		if (isspace(*ptr) && !isspace(last)) 
    			toks++;
    		last = *ptr;
    		ptr++;  
    	}
    	if (isspace(last))
    		toks--;

    	commands[cur_arg].argv = malloc((toks +1) * sizeof(char *));
    	commands[cur_arg].argc = toks;

    	// 명령 라인을 해석하여 인자 (argument) 배열로 변환한다.
    	//
    	printf("cur_arg.argc: %n cur_arg.argv: %s arg: %s ",commands[cur_arg].argc,commands[cur_arg].argv,arg );
    	parse_line(arg, commands[cur_arg].argv, commands[cur_arg].argc);

    }


int parse_line(int *waitpid, BOOLEAN makepipe, int *pipefdp)
{ 
	SYMBOL symbol, term;
	int argc, sourcefd, destfd;
	int pid, pipefd[2];
	char *argv[MAXARGS+1], sourcefile[MAXFNAME];
	char destfile[MAXFNAME];
	char word[MAXWORD];
	BOOLEAN append;

	argc =0;
	sourcefd = 0;
	destfd = 1;

	while(true)
	{
		switch ( symbol = getsymbol(word))
		{
			case S_WORD :
			//최대 인자 밖인가.
			if(argc == MAXARGS)
			{
				fprintf(stderr, "Too many args.\n");
				break;
			}

			argv[argc] = (char *) malloc(strlen(word)+1);

			if(argv[argc] == NULL)
			{
				fprintf(stderr, "out of arg memory.\n");
				break;
			}

			strcpy(argv[argc], word);

			argc++;
			continue;

			case S_LT	:

			// 이미 만들어져있으면.. ㅠ 
			if(makepipe) 
				{
					fprintf(stderr, "Extra <.\n");
					break;
				}

				if(getsymbol(sourcefile) != S_WORD) 
				{
					fprintf(stderr, "Illegal <.\n");
					break;
				}

				sourcefd = BADFD;
				continue;

			case S_GT 	:
			case S_GTGT	:

				if (destfd != 1)
				{
					fprintf(stderr, "Extra > or >>.\n");
					break;
				}

				if(getsymbol(destfile) != S_WORD)
				{
					fprintf(stderr, "Illegal > or >>.\n");
				}

				destfd = BADFD;
				append = (symbol == S_GTGT);
				continue;

			case S_BAR  :
			case S_AMP  :
			case S_SEMI :
			case S_NL   :

				argv[argc] = NULL;

				if(symbol == S_BAR)
				{
					if (destfd != 1)
					{
					fprintf(stderr, "> or >> conflicts with |.\n");
					break;
					}

					term = parse(waitpid, true, &destfd);
				}

				else
				{
					term = symbol;
				}

				if(makepipe)
					if (pipe(pipefd) == ERROR)
					{
						syserr("pipe");
					}
					*pipefdp = pipefd[1];
					sourcefd = pipefd[0];
				}

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






	int nchunk = 1;
	int i;

	for(i= 0; i < strlen(line); i++) {
		if(line[i] == )
			strtok
		if (line[i] == '|' && ( i != 0 && i != strlen(line) - 1))
			nchunk++;
		else if (line[i] == '|' && (i == 0 || i == strlen(line) - 1))
			/* Syntax error */
			return 1;
	}

	
	