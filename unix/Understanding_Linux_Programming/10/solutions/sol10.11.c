/** sol10.11.c
 ** ------------------------------------------------------------
	A version of whotofile that appends the output of who
	by using output redirection is
	sol10.11.c.
	Notice this version sets the O_APPEND mode for the
	file instead of using lseek and writing there.

 ** ------------------------------------------------------------
 **
 **
 *   A version of whotofile.c that appends output of who to a file.
 *   This version opens the file for O_APPEND to make sure no info
 *   gets lost.
 *
 * whotofile.c
 *	purpose: show how to redirect output for another program
 *	   idea: fork, then in the child, redirect output, then exec

 *	build: cc sol10.11.c -o sol10.11
 */
#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include	<stdlib.h>
#include	<fcntl.h>
#include	<unistd.h>

#define	DFLMODE	0644

int main(int ac, char **av)
{
	pid_t	pid ;
	char	*file = "userlist";

	if ( ac > 1 )
		file = av[1];

	printf("About to run who into a file\n");

	/* create a new process or quit */
	if( (pid = fork() ) == -1 ){
		perror("fork"); exit(1);
	}
	/* child does the work */
	if ( pid == 0 ){
		/* close then open */
		close(1);
		if ( open(file,O_WRONLY|O_CREAT|O_APPEND,DFLMODE) != 1 )
			perror(file);
		else {
			execlp( "who", "who", NULL );	/* and run	*/
			perror("execlp");
		}
		exit(1);
	}
	/* parent waits then reports */
	if ( pid != 0 ){
		wait(NULL);
		printf("Done running who.  results in %s\n", file);
	}
	return 0;
}
