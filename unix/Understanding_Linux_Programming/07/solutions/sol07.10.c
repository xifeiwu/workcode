/** sol07.10.c
 ** ------------------------------------------------------------
	A version of hello5.c that uses usleep() and has varying
	speed is
	sol07.10.c.
	This version similates simple harmonic motion by varying
	the speed according to the sine function.  The timing
	is isolated in a function called delay().  That function
	could be replaced by one that returns a delay based on
	the acceleration of gravity. Or any other function for
	that matter.

 ** ------------------------------------------------------------
 **
 **
 *   A version of hello5.c with varying speed.  This program
 *   varies the speed according to the sin() function in
 *   order to depict simple harmonic motion.
 *
 *   The screen is COLS positions wide, and the width of half
 *   a sine wave is pi radians.  The speed is therefore
 *
 *     speed(col) = sin( col * pi / COLS )
 *
 *   The speed of motion is determined by the length of the delay.
 *   That is:
 *
 *     delay = 1/speed
 *
 *    build:  cc sol07.10.c -lm -lcurses -o sol07.10
 *
 *    usage: hello5 [rate]
 */

#include	<curses.h>
#include	<math.h>
#include	<stdlib.h>

#define	RATE	10

main(int ac, char *av[])
{
	char	message[] = "Hello";
	char	blank[]   = "     ";
	long	rate = RATE;
	int	dir = +1;
	int	pos = 0;
	int	rightedge;
	int	row;
	int	width;
	long	delay(int,int);

	/*
	 * setup 
	 */

	initscr();
	clear();
	rightedge = COLS-strlen(message);
	row       = LINES/2 ;

	if ( ac > 1 && atoi(av[1]) > 0 )
		rate = atoi(av[1]);

	/*
	 * the loop
	 */

	while(1){
		move(row,pos);
		addstr( message );	/* draw string		*/
		move(LINES-1,COLS-1);	/* park the cursor	*/
		refresh();		/* show string		*/
		usleep(rate*delay(pos,rightedge));
		move(row,pos);		/* erase string		*/
		addstr( blank );
		pos += dir;		/* advance position	*/
		if (pos >= rightedge)
			dir = -1;
		if ( pos <= 0 )
			dir = +1;
	}
}

/*
 * function that computes the delay from the current position.
 * This function finds a delay based on the sine function.  At
 * the center of the screen, the delay is shortest and at the
 * extremes, the delay is greatest.  The fudge value was used
 * to adjust the motion to look natural.  
 */

#define	FUDGE	0.05

long
delay(int pos,int width)
{
	double speed ;
	long   rv;

	/* 
	 * value of sine here will be between 0 and 1.
	 * A speed of 0 is not good, because motion will stop.
	 * Therefore, we fudge it so the speed is never 0 but
	 * instead ranges from FUDGE to 1.
	 */
	speed = sin( M_PI * ((double) pos)/width );
	speed = FUDGE + ( speed * (1.0 - FUDGE) );

	/* delay is inversely proportional to speed */

	rv = (int) (1000.0 / speed );
	return rv;
}
