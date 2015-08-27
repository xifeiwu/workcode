/** sol07.16.c
 ** ------------------------------------------------------------
	A version of a reaction time tester is
	sol07.16.c.
	This program uses curses and requires using
	gettimeofday to obtain the current time at higher
	resolution than time(2) provides.

	The design of a user interface is important in this
	program. The interface affects the results.  The program
	contains ideas that apply to any program that needs to
	measure times or intervals of input from external devices.

 ** ------------------------------------------------------------
 **
 **
 *   reaction time tester
 *
 *   This program tests reaction time by
 *   displaying a digit in the center of the screen and waiting
 *   for the user to type that digit on the keyboard.  The test
 *   is repeated ten times and some basic statistics on response
 *   time are presented.
 *
 *   build: cc sol07.16.c -lcurses -o sol07.16
 */

#include	<stdio.h>
#include	<sys/time.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<curses.h>

#define	NTESTS	10

main()
{
	long	results[NTESTS];
	int	i;

	/*
	 * setup
	 */

	srand(getpid());
	initscr();
	noecho();
	crmode();

	for( i=0; i<NTESTS; i++)
		results[i] = dotest(i+1);
	endwin();

	report(results,NTESTS);
}

/*
 * do one test, report the number in microseconds
 */

char charset[] = "0123456789";

#define	RANGE	3		/* range in seconds */

dotest(int n)
{
	int	waitfor;
	char	digit;
	struct	timeval start, done;
	int	c;
	int	rv;

	waitfor = 1000 + (rand() % (RANGE*1000));

	digit = charset[rand() % strlen(charset)];

	/* alert the user */

	move(LINES/2,COLS/2-4);
	printw("TEST  %2d", n);
	move(LINES-1,COLS-1);
	refresh();
	sleep(2);
	clear();
	refresh();

	/* show the digit */
	move(LINES/2,COLS/2);
	addch(digit);
	refresh();

	gettimeofday(&start, 0);
	while( ( c = getch()) != EOF && c != digit )
		;
	gettimeofday(&done,0);

	/* hide the digit */
	move(LINES/2,COLS/2);
	addch(' ');
	refresh();

	/* return the time */
	if ( c == EOF )
		rv = -1L;
	else
		rv = 1000000 * (done.tv_sec - start.tv_sec) 
			+ ( done.tv_usec - start.tv_usec );
	return rv;
}

/*
 * report results
 */

report(long data[], int n)
{
	int	i;
	long	sum;
	long	min, max;

	printf("%3d. %8.4f\n", 1, data[0]/1000000.0);
	for( min = max = sum = data[0], i = 1; i<n ; i++ )
	{
		printf("%3d. %8.4f\n", i+1, data[i]/1000000.0);
		if ( data[i]>max )
			max = data[i];
		if ( data[i]<min )
			min = data[i];
		sum += data[i];
	}
	printf(" max %8.4f   min %8.4f   avg %8.4f\n", 
			 max/1000000.0, min/1000000.0, 
			(sum/(1000000.0*n)));
}

