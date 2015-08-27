/** sol11.7.c
 ** ------------------------------------------------------------
	A version of smsh1.c (from Chapter 9) that supports the
	-c option is
	sol11.7.c.
	The addition to your own shell may be different.

 ** ------------------------------------------------------------
 **
 ** 
 *   A modified version of smsh1.c that supports the -c option
 *
 *   build:  cc sol11.7.c splitline.c execute.c -o sol11.7

 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<signal.h>
#include	"smsh.h"

#define	DFL_PROMPT	"> "

int main(int ac, char *av[])
{
	char	*cmdline, *prompt, **arglist;
	int	result;
	void	setup();
	int	resultcode(int);

	/* if -c, then handle the command and get out */
	if ( ac > 2 && strcmp(av[1],"-c") == 0 ){
		return resultcode(execute(splitline(av[2]))) ;
	}

	prompt = DFL_PROMPT ;
	setup();

	while ( (cmdline = next_cmd(prompt, stdin)) != NULL ){
		if ( (arglist = splitline(cmdline)) != NULL  ){
			result = execute(arglist);
			freelist(arglist);
		}
		free(cmdline);
	}
	return 0;
}
/*
 * the $? value returned by sh is the exit value from child
 * if child exited normally, but is the termination code + 128
 * if not
 */

int resultcode(int r)
{
	if ( r & 0x7F )
		r |= 128;
	else
		r >>= 8;
	return r;
}


void setup()
/*
 * purpose: initialize shell
 * returns: nothing. calls fatal() if trouble
 */
{
	signal(SIGINT,  SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n)
{
	fprintf(stderr,"Error: %s,%s\n", s1, s2);
	exit(n);
}

