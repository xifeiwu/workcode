/** sol14.15.c
 ** ------------------------------------------------------------
	A version of tanimate.c in which some of the strings bounce
	vertically while some bounce horizontally is
	sol14.15.c.

	This program does not include new ideas about threads, but
	produces amusing results.  The complicated question, not
	addressed in the solution provided, is collisions.  What
	can the program do when two messages move into the same
	space on the screen?  It can be a valuable discussion project
	to start with the provided solution and explore the problems
	involved in identifying and doing something with collisions.
	How much locking do you need to do?  What do you need to lock?

 ** ------------------------------------------------------------
 **
 **
 *  A version of tanimate in which some objects move vertically
 *  and some move horizontally.
 *
 *  The interesting question for this program is how to handle
 *  collisions.  How can you tell when two messags are about
 *  to be in the same space when both messages are moving?  By the
 *  time you think you know where one message is, it may have moved.
 *  Furthermore, the user can change the direction at any time.
 *  Chaos is pretty easy to produce; producing true cooperation 
 *  is a lot of work.
 */

/* tanimate.c: animate several strings using threads, curses, usleep() 
 *
 *	bigidea one thread for each animated string
 *		one thread for keyboard control
 *		shared variables for communication
 *	to do   needs locks for shared variables
 *	        nice to put screen handling in its own thread
 *
 *	build:	cc sol14.15.c -lcurses -lpthread -o sol14.15
 */

#include	<stdio.h>
#include	<string.h>
#include	<curses.h>
#include	<pthread.h>
#include	<stdlib.h>
#include	<unistd.h>

#define	MAXMSG	10		/* limit to number of strings	*/
#define	TUNIT   20000		/* timeunits in microseconds */

struct	propset {
		char	*str;	/* the message   */
		int	row;	/* the row       */
		int	prevr;	/* previous row	 */
		int	col;	/* col		 */
		int	vert;	/* a boolean	 */
		int	delay;  /* delay in time units */
		int	dir;	/* +1 or -1	*/
	};

pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;

int setup(int nstrings, char *strings[], struct propset props[]);
void *animate(void *arg);

int main(int ac, char *av[])
{
	int	       c;		/* user input		*/
	pthread_t      thrds[MAXMSG];	/* the threads		*/
	struct propset props[MAXMSG];	/* properties of string	*/
	void	       *animate();	/* the function		*/
	int	       num_msg ;	/* number of strings	*/
	int	     i;

	if ( ac == 1 ){
		printf("usage: tanimate string ..\n"); 
		exit(1);
	}

	num_msg = setup(ac-1,av+1,props);

	/* create all the threads */
	for(i=0 ; i<num_msg; i++)
		if ( pthread_create(&thrds[i], NULL, animate, &props[i])){
			fprintf(stderr,"error creating thread");
			endwin();
			exit(0);
		}

	/* process user input */
	while(1) {
		c = getch();
		if ( c == 'Q' ) break;
		if ( c == ' ' )
			for(i=0;i<num_msg;i++)
				props[i].dir = -props[i].dir;
		if ( c >= '0' && c <= '9' ){
			i = c - '0';
			if ( i < num_msg )
				props[i].dir = -props[i].dir;
		}
	}

	/* cancel all the threads */
	pthread_mutex_lock(&mx);
	for (i=0; i<num_msg; i++ )
		pthread_cancel(thrds[i]);
	endwin();
	return 0;
}

int setup(int nstrings, char *strings[], struct propset props[])
{
	int num_msg = ( nstrings > MAXMSG ? MAXMSG : nstrings );
	int i;

	initscr();
	/* assign rows and velocities to each string */
	srand(getpid());
	for(i=0 ; i<num_msg; i++){
		props[i].str = strings[i];	/* the message	*/
		props[i].row = i;		/* the row	*/
		props[i].delay = 1+(rand()%15);	/* a speed	*/
		props[i].dir = ((rand()%2)?1:-1);	/* +1 or -1	*/
		props[i].vert = (rand()%2);	/* 0 or 1 */
		props[i].col = (rand()%(COLS-strlen(strings[i])-3));
		props[i].prevr = -1;
	}

	/* set up curses */
	crmode();
	noecho();
	clear();
	mvprintw(LINES-1,0,"'Q' to quit, '0'..'%d' to bounce",num_msg-1);

	return num_msg;
}

/* the code that runs in each thread */
/* 
 * First lock curses, then update screen, 
 * then unlock curses, then compute new position
 */
void *animate(void *arg)
{
	struct propset *info = arg;		/* point to info block	*/
	int	len = strlen(info->str)+2;	/* +2 for padding	*/

	while( 1 )
	{
		/** part 1: wait a bit **/
		usleep(info->delay*TUNIT);

		/** part 2: update the screen */
		/*
		 * THE SCREEN IS A SHARED RESOURCE SO MUST BE LOCKED
		 *   --- CRITICAL SECTION STARTS HERE ----
		 */
		 pthread_mutex_lock(&mx);	

		   if ( info->vert ){
			if ( info->prevr != -1 ){
		   	    move( info->prevr, info->col );
			    printw("%*s", strlen(info->str), "");
			}
			move( info->row, info->col );
			addstr( info->str );
			info->prevr = info->row;
		   } 
		   else {
			move( info->row, info->col );
		   	addch(' ');
		   	addstr( info->str );
		   	addch(' ');
		   }
		   move(LINES-1,COLS-1);	/* park cursor		*/
		   refresh();			/* and show it		*/

		 pthread_mutex_unlock(&mx);	
		 /*
		  * ------ CRITICAL SECTION ENDS HERE ---------------- */

		/** Part 3: Compute new position **/

		/* if vertical motion,.. move that way */
		if ( info->vert ){
			info->row += info->dir;
			if ( info->row <= 0 && info->dir == -1 )
				info->dir = 1;
			else if ( info->row >= LINES-2 && info->dir == 1 )
				info->dir = -1;
		}
		else {
			info->col += info->dir;

			if ( info->col <= 0 && info->dir == -1 )
				info->dir = 1;
			else if (  info->col+len >= COLS && info->dir == 1 )
				info->dir = -1;
		}
	}
}
