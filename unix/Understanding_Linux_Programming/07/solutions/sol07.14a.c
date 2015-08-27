/** sol07.14a.c
 ** ------------------------------------------------------------
	A program that prints a message after receving a certain
	number of SIGINTs is
	sol07.14a.c.

	and the driver program that sends SIGNTs to a list of
	processes at regular intervals is
	sol07.14b.c.

	This exercise provides practice writing programs that
	send signals, programs that receive signals, and simple
	timers.  The exercise also provides some practice creating
	and managing background processes.  Finally, the exercise
	sets some groundwork for the next chapter on processes.

 ** ------------------------------------------------------------
 **
 **
 *   ouch program - print a message after receiving a certain
 *                  number of SIGINTs.
 *
 *   usage:  ouch message number
 *
 *   build: cc sol07.14a.c -o sol07.14a
 */

#include	<stdio.h>
#include	<signal.h>

char *msg;

main(int ac, char *av[])
{
	void counter(int);

	if ( ac != 3 ){
		fprintf(stderr,"usage: ouch message number\n");
		exit(1);
	}

	msg = av[1];
	counter( -1 * atoi(av[2]) );

	signal(SIGINT, counter);

	while(1)
		pause();
}

/*
 * set and increment counter.  If called with a negative number
 * then set the freq value to that number.  If called with a
 * signal number, then increment counter and say the message;
 */
void
counter(int s)
{
	static int freq;
	static int times = 0;

	if ( s < 0 )
		freq = -1 * s;
	else {
		times++;
		if ( times >= freq ){
			printf("%s\n", msg);
			times = 0;
		}
	}
}
