/** sol07.15.c
 ** ------------------------------------------------------------
	A program that blocks on time and handles keybaord events
	as signals is
	sol07.15.c.
	This program uses async input handling to reverse the
	design of the original bounce1d program.  In the original
	program, the program blocked on user input and 
	timer events were handled to drive the animation.
	This version provides some practice in using user input
	as events that can be handled.  This version also provides
	more practice in selecting and comparing design models
	for a system.  Which of the three versions is better or
	easier, or more efficient?  The one with two handlers, the
	one with a timer handler, the one with a keyboard handler?

 ** ------------------------------------------------------------
 **
 **
 *   bounce1d that blocks on time and handles keyboard events as
 *            signals.
 *
 *	purpose	animation with user controlled speed and direction
 *	note	the handler processes keyboard input
 *		the main program sleeps and cllas the animation
 *
 *	build:	cc sol07.15.c -lcurses -o sol07.15
 */
#include	<stdio.h>
#include	<curses.h>
#include	<signal.h>
#include	<fcntl.h>

/* some global settings main and the handler use */

#define	MESSAGE	"hello"
#define BLANK   "     "

int	row;	/* current row		*/
int	col;	/* current column	*/
int	dir;	/* where we are going	*/
int	delay;	/* bigger => slower	*/
int	done;

void	on_input(int);	/* handler for keybd    */
void	enable_kbd_signals();

int main()
{
	initscr();
	crmode();
	noecho();
	clear();

	signal(SIGIO,on_input); /* install a handler        */
	enable_kbd_signals();   /* turn on kbd signals      */

	row   = 10;		/* start here		*/
	col   = 0;
	dir   = 1;		/* add 1 to col number	*/
	delay = 200;		/* 200ms = 0.2 seconds  */
	done  = 0;

	while(!done)
	{
		usleep(delay);
		move_msg();
	}
	endwin();
	return 0;
}

/*
 * called after each sleep period
 */

move_msg()
{
	move( row, col );
	addstr( BLANK );
	col += dir;			/* move to new column	*/
	move( row, col );		/* then set cursor	*/
	addstr( MESSAGE );		/* redo message		*/
	move(0,COLS-1);
	refresh();			/* and show it		*/

	/*
	 * now handle borders
	 */
	if ( dir == -1 && col <= 0 )
		dir = 1;
	else if ( dir == 1 && col+strlen(MESSAGE) >= COLS )
		dir = -1;
}

/*
 *  called when a keystroke appears
 */
void on_input(int signum)
{
	int	c = getch();		  /* grab the char */

	switch ( c ){
		case 'Q':
		case EOF: done = 1;
			  break;
		case ' ': dir = -dir;
			  break;
		case 'f': if ( delay > 2 )
				  delay >>= 1;
			  break;
		case 's': delay <<= 1;
			  break;
	}
}

/*
 * install a handler, tell kernel who to notify on input, enable signals
 */
void enable_kbd_signals()
{
	int  fd_flags;

	fcntl(0, F_SETOWN, getpid());
	fd_flags = fcntl(0, F_GETFL);
	fcntl(0, F_SETFL, (fd_flags|O_ASYNC));
}

