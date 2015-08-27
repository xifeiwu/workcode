/** sol08.9.c
 ** ------------------------------------------------------------
	A program that uses SIGCHLD to notify the parent that
	a child has exited is
	sol08.9.c.
	This solution shows how a program can perform other tasks while
	a child process runs.  A parent does not have to stop
	activity to wait for a child.

 ** ------------------------------------------------------------
 **
 **
 *   A version of waitdemo2.c that uses SIGCHLD to notify the parent
 *   when a child is done.  
 *
 **    build: cc sol08.9.c -o sol08.9
 */

#include	<stdio.h>
#include	<sys/types.h>
#include	<unistd.h>
#include	<signal.h>

#define	DELAY	5

main()
{
	pid_t  newpid;
	void child_code(), parent_code();

	printf("before: mypid is %d\n", getpid());

	if ( (newpid = fork()) == -1 )
		perror("fork");
	else if ( newpid == 0 )
		child_code(DELAY);
	else
		parent_code(newpid);
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
void parent_code(int childpid)
{
	void get_child_status(int);

	signal(SIGCHLD, get_child_status);

	while(1){
		printf("waiting for %d...\n", childpid);
		sleep(1);
	}
}

void
get_child_status(int s)
{
	int wait_rv;		/* return value from wait() */
	int child_status;
	int high_8, low_7, bit_7;

	wait_rv = wait(&child_status);
	printf("done waiting for child. Wait returned: %d\n", wait_rv);

	high_8 = child_status >> 8;     /* 1111 1111 0000 0000 */
	low_7  = child_status & 0x7F;   /* 0000 0000 0111 1111 */
	bit_7  = child_status & 0x80;   /* 0000 0000 1000 0000 */
	printf("status: exit=%d, sig=%d, core=%d\n", high_8, low_7, bit_7);

	exit(0);
}
