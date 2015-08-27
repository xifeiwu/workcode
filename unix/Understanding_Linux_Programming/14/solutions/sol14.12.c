/** sol14.12.c
 ** ------------------------------------------------------------
	A version of tbounce1d.c that eliminates global variables
	by storing all state variables in a struct is
	sol14.12.c.

 ** ------------------------------------------------------------
 **
 **
 *   A version of tbounce1d.c with all global state variables 
 *   moved into a struct.  A pointer to that struct can be passed
 *   to any thread or function that needs access.
 *
 *   This version addresses race conditions on the dir variable by
 *   adding code to handle 'impossible' cases in the animation 
 *   function.  If the keyboard handling thread modifies dir at
 *   a critical time, then impossible values can occur.  One method
 *   is to use locks to prevent those values from happening, and
 *   another method is to code for them, assuming them not impossible.
 */

/* tbounce1.c: controlled animation using two threads 
 *	note	one thread handles animation
 *		other thread handles keyboard input
 *	build:	cc sol14.12.c -lcurses -lpthread -o sol14.12
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	<curses.h>
#include	<pthread.h>
#include	<stdlib.h>
#include	<unistd.h>

/* An animation is defined as a set of state variables       */
/* these all are stored in a single struct for easy handling */

struct animation 
	{
		char		*msg;		/* printed thing  */
		int		row;		/* vertical   pos */
		int		col;		/* horizontal pos */
		int		dir;		/* direction	  */
		int		delay;		/* wait between moves */
	};

#define	MESSAGE	" hello "

int main()
{
	int	ndelay;		/* new delay		*/
	int	c;		/* user input		*/
	pthread_t msg_thread;	/* a thread		*/
	void	*moving_msg();
	struct animation a;

	initscr();		/* init curses and tty	*/
	crmode();
	noecho();
	clear();

	a.row   = 10;		/* start here		*/
	a.col   = 0;
	a.dir   = 1;		/* add 1 to row number	*/
	a.delay = 200;		/* 200ms = 0.2 seconds  */
	a.msg   = MESSAGE;

	if ( pthread_create(&msg_thread,NULL,moving_msg,(void*)&a) ){
		fprintf(stderr,"error creating thread");
		endwin();
		exit(0);
	}

	while(1) {
		ndelay = 0;
		c = getch();
		if ( c == 'Q' ) break;
		if ( c == ' ' ) a.dir = -a.dir;
		if ( c == 'f' && a.delay > 2 ) ndelay = a.delay/2;
		if ( c == 's' ) ndelay = a.delay * 2 ;
		if ( ndelay > 0 )
			a.delay = ndelay ;
	}
	pthread_cancel(msg_thread);
	endwin();
	return 0;
}

/*
 * This function runs in an animation thread.
 * The function sleeps, moves a string, sleeps ...
 *
 * The dir and delay members of the struct may change at
 * any time because those are under the control of users
 * in the keybaord handling thread.
 *
 */
void *moving_msg(struct animation *ap)
{
	int	len = strlen(ap->msg)+1;

	while( 1 ) {
		usleep(ap->delay*1000);	/* sleep a while 	*/

		move(ap->row,ap->col);	/* set cursor position	*/
		addstr( ap->msg );	/* redo message		*/
		refresh();		/* and show it		*/

		/* move to next column and check for bouncing	*/

		ap->col += ap->dir;	/* move to new column	*/

		if ( ap->col< 0 ){
			ap->col = 0;
			ap->dir = 1;
		} 
		else if ( ap->col == 0 && ap->dir == -1 )
			ap->dir = 1;
		else if ( ap->col+len > COLS ){
			ap->col = COLS-len;
			ap->dir = -1;
		}
		else if ( ap->col+len == COLS )
			ap->dir = -1;
	}
}
