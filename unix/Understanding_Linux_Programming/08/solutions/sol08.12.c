/** sol08.12.c
 ** ------------------------------------------------------------
	Regular Unix shells respond to SIGINT by cancelling
	all collected command line input and printing a new prompt.
	Bash ignores SIGQUIT.  A version of psh2.c that supports
	these two features is 
	sol08.12.c.
	This program requires understanding SA_RESTART for sigaction.
	Systems that do not default to autorestart on system calls will
	clear the input and return from fgets() with an error automatically.
	On the other hand, systems that restart systems calls after
	handling signals will not behave the way the shell does.
	To change this behavior, the program calls sigaction() and turns
	off the bit for SA_RESTART.
 ** ------------------------------------------------------------
 **
 **
 *
 * a version of psh2.c that responds to SIGINT and SIGQUIT in the
 * style that bash does.  In particular:
 * 
 *    SIGINT  - flush current input and start over for new command
 *    SIGQUIT - ignore it
 *
 *  Note: This program uses sigaction to force the signal handler to
 *        NOT restart fgets().  The BSD behavior is to restat the
 *        system call after calling the handler.  We do not want that
 *        to happen. Instead, we want fgets() to return an error so
 *        we can print a new prompt and start over.
 *
 **	Solves the `one-shot' problem of version 1
 **		Uses execvp(), but fork()s first so that the
 **		shell waits around to perform another command
 *
 **    build: cc sol08.12.c -o sol08.12 
 **/

#include	<stdio.h>
#include	<signal.h>

#define	MAXARGS		20				/* cmdline args	*/
#define	ARGLEN		100				/* token length	*/

/*
 * used by sigint_handler to tell reader to restart
 */
int	restart = 0;

main()
{
	char	*arglist[MAXARGS+1];		/* an array of ptrs	*/
	int	numargs;			/* index into array	*/
	char	argbuf[ARGLEN];			/* read stuff here	*/
	char	*makestring();			/* malloc etc		*/
	char	*fgetsrv;			/* return from fgets	*/

	void	sigint_handler(int);

	numargs = 0;

	/*
	 * change 1: ignore SIGQUIT
	 */
	signal(SIGQUIT,SIG_IGN);
	setup_sigint(sigint_handler);

	while ( numargs < MAXARGS )
	{					
		printf("Arg[%d]? ", numargs);
		fgetsrv = fgets(argbuf, ARGLEN, stdin);
		/*
		 * there are three possible outcomes from the call to fgets
		 *
		 * outcome 1: caused by SIGINT, so we restart
		 */
		if ( restart ){
			numargs = 0;
			restart = 0;
		}
		/*
		 * outcome 2: Not end of file and not a blank line,
		 * so we add that string to our list
		 */
		else if ( fgetsrv != NULL  && *argbuf != '\n' )
		{
			arglist[numargs++] = makestring(argbuf);
			if ( numargs == 1 && strcmp(arglist[0],"exit")==0 )
				break;
		}
		/*
		 * outcome 3: EOF or blank line, so we execute the list
		 */
		else
		{
			if ( numargs > 0 ){		/* any args?	*/
				arglist[numargs]=NULL;	/* close list	*/
				execute( arglist );	/* do it	*/
				numargs = 0;		/* and reset	*/
			}
			if ( fgetsrv == NULL )
				break;
		}
	}
	return 0;
}

/*
 * Turn off auto restart on interrupted system calls
 * This function will turn off the SA_RESTART bit in the
 * signal handler so sending SIGINT will cancel the call to fgets
 */
setup_sigint(void (*handler)(int))
{
	struct sigaction settings;
	sigset_t         blocked;

	if ( sigaction(SIGINT, NULL, &settings) == -1 )
		perror("Cannot get setttings");
	else
	{
		settings.sa_flags &= ~(SA_RESTART|SA_SIGINFO);
		settings.sa_handler = handler;
		if ( sigaction(SIGINT, &settings, NULL) == -1 )
			perror("cannot set handler");
	}
}
/*
 * called on SIGINT
 * Action: set restart to 1 so that command line reader
 *         will start over
 */
void sigint_handler(int s)
{
	restart = 1;
	printf("Interrupted!\n");
}

execute( char *arglist[] )
/*
 *	use fork and execvp and wait to do it
 */
{
	int	pid,exitstatus;				/* of child	*/

	pid = fork();					/* make new process */
	switch( pid ){
		case -1:	
			perror("fork failed");
			exit(1);
		case 0:
			execvp(arglist[0], arglist);		/* do it */
			perror("execvp failed");
			exit(1);
		default:
			while( wait(&exitstatus) != pid )
				;
			printf("child exited with status %d,%d\n",
					exitstatus>>8, exitstatus&0377);
	}
}
char *makestring( char *buf )
/*
 * trim off newline and create storage for the string
 */
{
	char	*cp, *malloc();

	if ( buf[strlen(buf)-1] == '\n' )
		buf[strlen(buf)-1] = '\0';	/* trim newline	*/
	cp = malloc( strlen(buf)+1 );		/* get memory	*/
	if ( cp == NULL ){			/* or die	*/
		fprintf(stderr,"no memory\n");
		exit(1);
	}
	strcpy(cp, buf);		/* copy chars	*/
	return cp;			/* return ptr	*/
}
