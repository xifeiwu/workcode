/** sol08.8a.c
 ** ------------------------------------------------------------
	A small extension of waitdemo1.c to create and wait for
	two child processes is
	sol08.8a.c.
	Watching the order of output messages from this program 
	helps clarify the fact that the processes are running in
	parallel and writing to the same output stream.

 ** ------------------------------------------------------------
 **
 **
 *    A version of waitdemo1.c that creates and waits for two children.
 *
 *    build: cc sol08.8a.c -o sol08.8a
 */

#include	<stdio.h>

#define	DELAY	2

#define	oops(s,x) { perror(s); exit(x); }

main()
{
	int  newpid1, newpid2;
	void child_code(), parent_code();

	printf("before: mypid is %d\n", getpid());

	if ( (newpid1 = fork()) == -1 )
		oops("fork",1);
	if ( newpid1 == 0 )
		child_code(DELAY);

	if ( (newpid2 = fork()) == -1 )
		oops("fork",2);
	if ( newpid2 == 0 )
		child_code(DELAY);

	parent_code(newpid1, newpid2);
	return 0;
}
/*
 * new process takes a nap and then exits
 */
void child_code(int delay)
{
	printf("child %d here. will sleep for %d seconds\n", getpid(), delay);
	sleep(delay);
	printf("child done. about to exit\n");
	exit(17);
}
/*
 * parent waits for child then prints a message
 */
void parent_code(int child1, int child2)
{
	int wait_rv;		/* return value from wait() */

	printf("waiting for %d and %d\n", child1, child2);

	wait_rv = wait(NULL);
	printf("Wait returned: %d\n", wait_rv);

	wait_rv = wait(NULL);
	printf("Wait returned: %d\n", wait_rv);
}
