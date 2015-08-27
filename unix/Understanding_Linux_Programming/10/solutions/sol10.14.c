/** sol10.14.c
 ** ------------------------------------------------------------
	A program that makes a pipeline from an arbitrary number
	of single-word commands is
	sol10.14.c.
	The solution is recursive in a very elegant way.  

 ** ------------------------------------------------------------
 **
 **
 **  pipe-n.c  - run three commands in a pipeline
 *
 *    usage: pipe3 cmd1 cmd2 cmd3
 *   effect: cmd1 | cmd2 | cmd3
 *     note: these commands must be single names, no arguments allowed
 *     note: the program uses a recursive function that calls fork
 *           to recurse with new processes.  You may need to draw some
 *           pictures to see what is going on.

 *	build: cc sol10.14.c -o sol10.14
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<unistd.h>

#define	oops(s,x)  { perror(s); exit(x); }

int main(int ac, char *av[])
{
	int	apipe[2];
	pid_t	pid;

	/*
	 * simple case: no command to execute
	 */

	if ( ac == 1 ){
		fprintf(stderr,"usage: pipe-n cmd1 cmd2 ..\n");
		exit(1);
	}
	/*
	 * base case: one command to execute, no pipe needed
	 */
	if ( ac == 2 ){
		execlp(av[1],av[1],NULL);
		oops(av[1],2);
	}
	/*
	 * inductive case: parent will be the last command in
	 * the pipeline, and the child will will be the
	 * rest.  That is, the child always writes into the pipe
	 * and the parent reads from the pipe and writes to stdout
	 * Note that nobody waits for anyone.  They all exit in
	 * time and init mops them up.  We hope.
	 */

	if ( pipe(apipe) == -1 )
		oops("pipe",3);

	pid = fork();
	if ( pid == -1 )
		oops("fork",4);

	/* child will write stdout into pipe[1] */
	if ( pid == 0 ){
		if ( dup2(apipe[1],1) == -1 )
			oops("dup2",5);
		if ( close(apipe[1]) == -1 || close(apipe[0]) == -1 )
			oops("close pipe",6);
		main(ac-1,av);			/* thanks to rebecca morse */
	}
	/* parent will read stdin from pipe[0] */
	else {
		if ( dup2(apipe[0],0) == -1 )
			oops("dup2",7);
		if ( close(apipe[0]) == -1 || close(apipe[1]) == -1 )
			oops("close pipe",8);
		execlp(av[ac-1],av[ac-1],NULL);
		oops(av[ac-1],9);
	}
	return 0;
}

