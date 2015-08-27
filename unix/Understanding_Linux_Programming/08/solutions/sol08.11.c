/** sol08.11.c
 ** ------------------------------------------------------------
	This enhanced version of psh2.c allows the user
	to exit from the shell by typing exit or by
	sending EOF:
	sol08.11.c.

	The changes were added in the main input loop.

 ** ------------------------------------------------------------
 **
 **
 **  prompting shell version 2 with the "exit" command added
 **
 **		Solves the `one-shot' problem of version 1
 **			Uses execvp(), but fork()s first so that the
 **			shell waits around to perform another command
 **		New problem: shell catches signals.  Run vi, press ^c.
 *
 **    build: cc sol08.11.c -o sol08.11
 **/

#include	<stdio.h>
#include	<signal.h>

#define	MAXARGS		20				/* cmdline args	*/
#define	ARGLEN		100				/* token length	*/

main()
{
	char	*arglist[MAXARGS+1];		/* an array of ptrs	*/
	int	numargs;			/* index into array	*/
	char	argbuf[ARGLEN];			/* read stuff here	*/
	char	*makestring();			/* malloc etc		*/
	char	*fgetsrv;

	numargs = 0;
	while ( numargs < MAXARGS )
	{					
		printf("Arg[%d]? ", numargs);
		fgetsrv = fgets(argbuf, ARGLEN, stdin);
		if ( fgetsrv != NULL && *argbuf != '\n' ){
		
			arglist[numargs++] = makestring(argbuf);
			/*
			 * new code to allow user to exit with 
			 * the "exit" command
			 */
			if ( numargs == 1 && strcmp(arglist[0],"exit") == 0 )
				break;
		}
		else
		{

			if ( numargs > 0 ){		/* any args?	*/
				arglist[numargs]=NULL;	/* close list	*/
				execute( arglist );	/* do it	*/
				numargs = 0;		/* and reset	*/
			}
			/* 
			 * new code added to allow user to exit at EOF
			 */
			if ( fgetsrv == NULL )
				break;
		}
	}
	return 0;
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
