/** sol10.12.c
 ** ------------------------------------------------------------
	A program that uses close, open, fork, and exec to redirect
	the input of sort to a file is
	sol10.12.c.
	This program is pretty much a mirror of the whotofile.c
	sample in the text.

 ** ------------------------------------------------------------
 **
 **
 *   sortfromfile.c
 *   This program redirects stdin from a file and then runs the
 *   sort tool.  
 *
 *   idea: fork, then in the child, redirect input, then exec

 *	build: cc sol10.12.c -o sol10.12
 */
#include	<stdio.h>
#include	<stdlib.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include	<unistd.h>

#define	DFLMODE	0644

#define oops(s,x) { perror(s); exit(x); }

int main(int ac, char **av)
{
	pid_t	pid ;
	char	*file;

	if ( ac == 1 ){
		fprintf(stderr,"usage: sortfromfile filename\n");
		exit(1);
	}
	file = av[1];
	printf("About to run sort with input from %s\n", file);

	/* create a new process or quit */
	if( (pid = fork() ) == -1 )
		oops("fork", 1);

	/* child does the work */
	if ( pid == 0 ){
		/* close then open */
		close(0);
		if ( open(file,O_RDONLY) != 0 )
			oops(file,2);
		execlp( "sort", "sort", NULL );	/* and run	*/
		oops("sort",3);
	}
	/* parent waits then reports */
	if ( pid != 0 ){
		wait(NULL);
		printf("Done running sort < %s\n", file);
	}
	return 0;
}
