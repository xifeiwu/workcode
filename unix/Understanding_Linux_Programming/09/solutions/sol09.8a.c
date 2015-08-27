/** sol09.8a.c
 ** ------------------------------------------------------------
	Adding background processing requires minor changes to four files.

	The replacement to smsh2.c is 

	sol09.8a.c,
	  This main loop is told by the next_cmd function that the
	  command ends in an &.  The main loop then passes that
	  fact to the process function.

		the replacement to splitline.c is 
		sol09.8b.c,
		the replacement to execute.c is 
		sol09.8c.c, 
		the replacement to process2.c is 
		sol09.8d.c, and
		the replacement to smsh.h is 
		sol09.8.h.

	Therefore, linking:

	cc sol09.8a.c sol09.8b.c sol09.8c.c sol09.8d.c \
		controlflow.c builtin.c varlib.c -o sol8

	will produce a shell that supports use of the & symbol to run 
	a command in the background.  The splitline replacement also 
	includes the feature to launch several commands on one line.  
	Therefore

	cmd1 & cmd2 & cmd3 ; cmd4 ; cmd5

	will work as expected.  As before, you can replace 
	controlflow.c with sol09.6.c or sol09.7.c for else support, 
	and you can replace builtin.c with sol09.5.c for exit and cd.

 ** ------------------------------------------------------------
 **
 **
 **	A version of smsh2 that handles background processing by
 **	using the termination char next_cmd provides to tell
 **	process() to wait or not
 **/
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<signal.h>
#include	<sys/wait.h>
#include	"sol09.8.h"

#define	DFL_PROMPT	"> "

int main()
{
	char	*cmdline, *prompt, **arglist;
	int	result, process(char **, int);
	void	setup();
	int	termchar;
	int	background;

	prompt = DFL_PROMPT ;
	setup();

	while ( (cmdline = next_cmd(prompt, stdin, &termchar)) != NULL ){
		if ( (arglist = splitline(cmdline)) != NULL  ){
			background = ( termchar == '&' ) ;
			result = process(arglist,background);
			freelist(arglist);
		}
		free(cmdline);
	}
	return 0;
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
	fprintf(stderr,"Error: %s %s\n", s1, s2);
	exit(n);
}
