/** sol07.14b.c
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
 *   metronome program
 *       send SIGINT to a list of PIDs every N seconds
 *
 *   usage:  N pid1 pid2 ..
 *
 *   build: cc sol07.14b.c -o sol07.14b
 */

#include	<stdio.h>
#include	<sys/types.h>
#include	<signal.h>

char *msg;

main(int ac, char *av[])
{
	int	i;
	int	sleeptime;

	if ( ac < 3 ){
		fprintf(stderr,"usage: metronome time pid1 ...\n");
		exit(1);
	}

	sleeptime = atoi(av[1]);

	while(1){
		sleep(sleeptime);
		for(i=2;i<ac;i++)
			kill( (pid_t) atoi(av[i]), SIGINT );
	}
}

