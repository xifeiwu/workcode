/** sol08.10.c
 ** ------------------------------------------------------------
	Another program that uses SIGCHLD to notify the parent
	when a child exits is
	sol08.10.c.

	This program creates several children that all exit at
	the same time.  As they all exit, the parent receives
	a lot of SIGCHLDs at once.  When the parent is handling
	one SIGCHLD, other SIGCHLDs are blocked but not queued.
	Therefore, it is likely that some of the signals will
	be lost.  See the discussion in Chapter 7 on how Unix
	handles multiple instances of a signal.  

	Some versions of Unix support queued signals.  Check your
	system to see how it responds.

	A solution is to use waitpid() instead of wait.  
	A detailed explanation of using waitpid() is
	presented in Chapter 12.

 ** ------------------------------------------------------------
 **
 **
/*   A version of waitdemo2.c that creates several children and
 *   uses SIGCHLD for notification.  The number of children is
 *   specified on the command line.
 *
 **    build: cc sol08.10.c -o sol08.10
 */

#include	<stdio.h>
#include	<sys/types.h>
#include	<unistd.h>
#include	<signal.h>

#define	DELAY	5

#define	oops(s,x) { perror(s); exit(x); }

int	num_children;
int	num_died ;

main(int ac, char *av[])
{
	pid_t	newpid;
	void	child_code(), parent_code();
	void	get_child_status(int);
	int	n, i;

	/*
	 * setup
	 */
	if ( ac == 1 ){
		fprintf(stderr,"usage: sol08.10 numberofchildren\n");
		exit(1);
	}
	n = atoi(av[1]);
	signal(SIGCHLD, get_child_status);
	
	/* 
	 * loop: create children
	 */
	num_children = n;
	num_died     = 0;
	for(i=0; i<n; i++){
		if ( (newpid = fork()) == -1 )
			oops("fork",2);
		if ( newpid == 0 )
			child_code(DELAY,i+1);
	}
	parent_code();
}
/*
 * new process takes a nap and then exits
 */
void child_code(int delay,int num)
{
	printf("child %d here. will sleep for %d seconds\n", getpid(), delay);
	sleep(delay);
	printf("child #%d (%d) done. about to exit\n",num,getpid());
	exit(num);
}
/*
 * parent waits for child then prints a message
 */
void parent_code()
{
	void get_child_status(int);

	while(1){
		printf("waiting ..%d children left\n",
				(num_children-num_died));
		sleep(1);
	}
}

void
get_child_status(int s)
{
	int wait_rv;		/* return value from wait() */
	int child_status;
	int high_8, low_7, bit_7;

	signal(SIGCHLD, get_child_status);	/* in case	*/

	wait_rv = wait(&child_status);
	printf("done waiting for child. Wait returned: %d\n", wait_rv);

	high_8 = child_status >> 8;     /* 1111 1111 0000 0000 */
	low_7  = child_status & 0x7F;   /* 0000 0000 0111 1111 */
	bit_7  = child_status & 0x80;   /* 0000 0000 1000 0000 */
	printf("status: exit=%d, sig=%d, core=%d\n", high_8, low_7, bit_7);

	num_died++;
	if ( num_died == num_children )
		exit(0);
}
