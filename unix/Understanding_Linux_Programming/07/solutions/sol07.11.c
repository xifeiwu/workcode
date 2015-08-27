/** sol07.11.c
 ** ------------------------------------------------------------
	A verson of ticker_demo.c that uses a global variable
	so the signal handler can inform the main function when
	the program should exit is
	sol07.11.c.
	This change is minor, but the technique is useful.
	The advantage of telling main when to exit is that
	main can call any wrap-up functions.  It is cleaner
	design to exit from main.  The disadvantage is that
	global variables are risky for the usual reasons.

 ** ------------------------------------------------------------
 *
 *   A slight revision of ticker_demo that uses a global variable
 *   to tell the main() function when to exit.
 *
 *   build: cc sol07.11.c -o sol07.11
 */

#include	<stdio.h>
#include        <sys/time.h>
#include        <signal.h>

int done;		/* countdown uses this to tell main it is done */

int main()
{
	void	countdown(int);

	signal(SIGALRM, countdown);
	if ( set_ticker(500) == -1 )
		perror("set_ticker");
	else
	{
		done = 0;
		while( !done )
			pause();
	}
	return 0;
}

void countdown(int signum)
{
	static int num = 10;
	printf("%d ..", num--);
	fflush(stdout);
	if ( num < 0 ){
		printf("DONE!\n");
		done = 1;
	}
}

/* [from set_ticker.c]
 * set_ticker( number_of_milliseconds )    
 *      arranges for interval timer to issue SIGALRM's at regular intervals
 *      returns -1 on error, 0 for ok
 *      arg in milliseconds, converted into whole seconds and microseconds
 *      note: set_ticker(0) turns off ticker
 */

int set_ticker( int n_msecs )
{
        struct itimerval new_timeset;
        long    n_sec, n_usecs;

        n_sec = n_msecs / 1000 ;		/* int part	*/
        n_usecs = ( n_msecs % 1000 ) * 1000L ;	/* remainder	*/

        new_timeset.it_interval.tv_sec  = n_sec;        /* set reload       */
        new_timeset.it_interval.tv_usec = n_usecs;      /* new ticker value */
        new_timeset.it_value.tv_sec     = n_sec  ;      /* store this       */
        new_timeset.it_value.tv_usec    = n_usecs ;     /* and this         */

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}


