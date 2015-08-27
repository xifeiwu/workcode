/** sol14.14.c
 ** ------------------------------------------------------------
	A version of tanimate.c that includes separate speed controls
	for all moving items is
	sol14.14.c.

	The changes are limited to adding two more cases to the
	keyboard input handling section.  Each object is driven
	by the values in its property set.  The new control keys
	simply change those control values; the animated object
	behave according to those settings.

 ** ------------------------------------------------------------
 **
 **
 *   A version of tanimate.c that allows speed control on each of the
 *   separate messages.  The method is to press 's' or 'f' followed
 *   by the message number.  For example, to speed up message 3,
 *   the user presses 'f' then '3'.
 */

/* tanimate.c: animate several strings using threads, curses, usleep() 
 *
 *	bigidea one thread for each animated string
 *		one thread for keyboard control
 *		shared variables for communication
 *	to do   needs locks for shared variables
 *	        nice to put screen handling in its own thread
 *
 *	build:	cc sol14.14.c -lcurses -lpthread -o sol14.14
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
		char	*str;	/* the message */
		int	row;	/* the row     */
		int	delay;  /* delay in time units */
		int	dir;	/* +1 or -1	*/
	};

pthread_mutex_t mx = PTHREAD_MUTEX_INITIALIZER;

int get_msgnum(int num_items);
int setup(int nstrings, char *strings[], struct propset props[]);

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
		else if ( c >= '0' && c <= '9' ){
			i = c - '0';
			if ( i < num_msg )
				props[i].dir = -props[i].dir;
		}
		/* slower means bigger delay, double it  */
		else if ( c == 's' ){
			int arg = get_msgnum(num_msg);
			if ( arg >= 0 )
				props[arg].delay <<= 1;
		}
		else if ( c == 'f' ){
			int arg = get_msgnum(num_msg);
			if ( arg >= 0 && props[arg].delay > 2 )
				props[arg].delay >>= 1;
		}
	}

	/* cancel all the threads */
	pthread_mutex_lock(&mx);
	for (i=0; i<num_msg; i++ )
		pthread_cancel(thrds[i]);
	endwin();
	return 0;
}

/*
 * get_msgnum(num_items)
 *   Reads one char.  If that char is a digit in the range '0'..(num_items-1)
 *   then return the value.  Otherwise return -1.
 */
int get_msgnum(int num_items)
{
	char	c = getch();

	if ( c >= '0' && c < '0' + num_items )
		return c - '0';
	return -1;
}

int setup(int nstrings, char *strings[], struct propset props[])
{
	int num_msg = ( nstrings > MAXMSG ? MAXMSG : nstrings );
	int i;

	/* assign rows and velocities to each string */
	srand(getpid());
	for(i=0 ; i<num_msg; i++){
		props[i].str = strings[i];	/* the message	*/
		props[i].row = i;		/* the row	*/
		props[i].delay = 1+(rand()%15);	/* a speed	*/
		props[i].dir = ((rand()%2)?1:-1);	/* +1 or -1	*/
	}

	/* set up curses */
	initscr();
	crmode();
	noecho();
	clear();
	mvprintw(LINES-1,0,"'Q' to quit, '0'..'%d' to bounce",num_msg-1);
	addstr("  Speed control:  s# = slower, f# = faster ");

	return num_msg;
}

/* the code that runs in each thread */
void *animate(void *arg)
{
	struct propset *info = arg;		/* point to info block	*/
	int	len = strlen(info->str)+2;	/* +2 for padding	*/
	int	col = rand()%(COLS-len-3);	/* space for padding	*/

	while( 1 )
	{
		usleep(info->delay*TUNIT);

		pthread_mutex_lock(&mx);	/* only one thread	*/
		   move( info->row, col );	/* can call curses	*/
		   addch(' ');			/* at a the same time	*/
		   addstr( info->str );		/* Since I doubt it is	*/
		   addch(' ');			/* reentrant		*/
		   move(LINES-1,COLS-1);	/* park cursor		*/
		   refresh();			/* and show it		*/
		pthread_mutex_unlock(&mx);	/* done with curses	*/

		/* move item to next column and check for bouncing	*/

		col += info->dir;

		if ( col <= 0 && info->dir == -1 )
			info->dir = 1;
		else if (  col+len >= COLS && info->dir == 1 )
			info->dir = -1;
	}
}
