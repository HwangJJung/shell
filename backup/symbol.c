#include <stdio.h>

typedef enum
{S_WORD, S_BAR, S_AMP, S_SEMI, S_GT, S_GTGT, S_LT, S_NL, S_EOF} SYMBOL;

typedef enum {NEUTRAL, GTGT, INQUOTE, INWORD} STATUS;

SYMBOL getsymbol(char *word)
{
	//변수정의
	STATUS state;
	int c;
	char *w;

	state = NEUTRAL; 			//status initialize
	w = word;					// word와 pointer간 섞일 경우 word에 값 보존


	while ((c = getchar()) != EOF)
	{
		switch (state)
		{
			// 처음에 상태를 NEUTRAL 로 초기화 했으므로 NEUTRAL 로
			case NEUTRAL :
				switch (c)
				{
					// 쉘에 ; & | < \n 문자 입력시 문자 리턴.

					case ';' : return S_SEMI;
					case '&' : return S_AMP;
					case '|' : return S_BAR;
					case '<' : return S_LT;
					case '\n': return S_NL;

					//공백문자나 탭일 경우 다시 문자를 입력 받는다.

					case ' ' :
					case '\t': continue;

					case '>' :						// 리다이렉션
						state = GTGT; 			// >> 상태가 될 수 있으므로 state 상태를 바꾸고 
						continue;				//다음 문자 입력

					case '"' :
						state = INQUOTE;		// "인 경우 문자열로 간주
						continue;

					
					default	:
						state = INWORD;
						*w++ = c;				// 문자를 w가 가리키는 곳에 저장하고 w 주소를 1증가... !!
						continue;
				}

			case GTGT:					//하나의 > 가 입력되어 있다면 GTGT로 만든후 > 가 추가될 경우 
											// GTGT 리턴, 아닐경우 GT 리턴

				if ( c=='>') 
					{
						return S_GTGT;
					}
				ungetc(c, stdin);
				return S_GT;


			case INQUOTE:
				switch (c)
				{
					case '\\' :				// \\문자 특수 
						*w++ = getchar();
						continue;

					case '"' :
					*w++ = '\0';
					return S_WORD;
				default :
					*w++ = c;
					continue;
				}

			case INWORD :
				switch (c)
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
			 		    *w = '\0';				// 종료문자 추가하고 
			            return S_WORD;			// S_WORD 리턴

			         default  : 
			         	*w++ = c;				// 다른 문자인경우에는 문자저장하고 w 주소증가
				        continue;				// 문자 입력으로 계속
			    }
		}

	}
	return S_EOF;
}
