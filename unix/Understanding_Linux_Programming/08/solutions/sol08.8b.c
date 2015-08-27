/** sol08.8b.c
 ** ------------------------------------------------------------
	A larger extension of waitdemo1.c that creates and waits
	for a number of processes specified on the command line
	is
	sol08.8b.c.
	This program raises an interesting question about
	computing random numbers with rand().  If the random
	delay is computed in the child, every child gets the
	same random number.  The reason is that the rand()
	function generates random numbers be applying a mathematical
	operation to the last random number it generated.  If the
	parent asks for the random number, then the copy of that
	last number is in the parent, available to be used the
	next time.  If the child computes the random number, the
	copy of that new number is stored in the child, and the
	parent's copy is unaffected.

 ** ------------------------------------------------------------
 **
 **
 *    A version of waitdemo1.c that creates and waits for as many
 *    children as you specify on the command line.
 *
 *    usage: sol08.8b numchildren
 *
 *    Interesting sidelight: if the delay is computed in the child,
 *    then every child gets the same random number.  That is, if the
 *    code reads:
 *
 *		child_code(1 + (rand()%MAXDELAY));
 *
 *    then every child gets the same value.  Explain.
 *
 **    build: cc sol08.8b.c -o sol08.8b
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<unistd.h>


#define	oops(s,x) { perror(s); exit(x); }
#define	MAXDELAY	10

main(int ac, char *av[])
{
	void 	child_code(int), parent_code(int);
	pid_t	pid;
	int	i,n;
	int	delay;

	/* 
	 * setup
	 */
	if ( ac == 1 ){
		fprintf(stderr,"usage: sol08.8b numberofchildren\n");
		exit(1);
	}
	n = atoi(av[1]);
	srand(getpid());

	printf("before: mypid is %d\n", getpid());

	/*
 	 * main loop
	 */
	for(i=0; i<n ; i++){
		delay = 1 + (rand() % MAXDELAY);
		pid = fork();
		if ( pid == -1 )
			oops("fork", 2);
		if ( pid == 0 )
			child_code(delay);
	}
	parent_code(n);
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
 * parent waits for children then prints a message
 */
void parent_code(int n)
{
	int wait_rv;		/* return value from wait() */

	printf("waiting for %d children\n", n);

	while( ( wait_rv = wait(NULL) ) != -1 )
		printf("Wait returned: %d. %d to go\n", wait_rv, --n);
}
