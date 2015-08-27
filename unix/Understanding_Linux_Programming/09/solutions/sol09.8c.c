/** sol09.8c.c
 ** ------------------------------------------------------------
	Adding background processing requires minor changes to four files.

		The replacement to smsh2.c is 
		sol09.8a.c,
		the replacement to splitline.c is 
		sol09.8b.c,
	the replacement to execute.c is 
	sol09.8c.c, 
	   This version does not call wait if the command
	   was terminated by the & char

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
 *   modified version of execute.c that takes a background argument
 */

/* execute.c - code used by small shell to execute commands */

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<signal.h>
#include	<sys/wait.h>

int execute(char *argv[], int background)
/*
 * purpose: run a program passing it arguments
 * returns: status returned via wait, or -1 on error
 *  errors: -1 on fork() or wait() errors
 */
{
	int	pid ;
	int	child_info = -1;

	if ( argv[0] == NULL )		/* nothing succeeds	*/
		return 0;

	if ( (pid = fork())  == -1 )
		perror("fork");
	else if ( pid == 0 ){
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execvp(argv[0], argv);
		perror("cannot execute command");
		exit(1);
	}
	else {
		/* note, background  processes will exit, and wait will catch them */
		/* we mop them all up here */
		if ( !background ){
			int rv;
			while( 1 ){
				rv = wait(&child_info);
				if ( rv ==  -1 ){
					perror("wait");
					break;
				}
				if ( rv == pid )
					break;
			}
		}
		else {
			fprintf(stderr,"%d\n", pid);
			child_info = 0;
		}

	}
	return child_info;
}
