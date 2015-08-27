/** sol14.16c.c 
 **
 *   A version of the threaded animation program that uses
 *   one thread for curses and commnuicates with that thread
 *   via a pipe.
 *
 */
/*
 *	bigidea one thread for each animated string
 *		one thread for keyboard control
 *		pipe for communication with curses thread
 *	build:	cc sol14.16.c -lcurses -lpthread -o sol14.16
 *
 *	usage:  sol14.16 str1 str2 ... 2>log
 *	 note:  the program writes to stderr the commands passed to
 *	        the drawing thread for debugging purposes
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
		int	disp;	/* pipe to display thread */
	};

void *display(void *arg);
int  randnum(int n);
void *animate(void *arg);
int  setup(int nstrings, char *strings[], struct propset props[], int *p);

int main(int ac, char *av[])
{
	int	       c;		/* user input		*/
	pthread_t      thrds[MAXMSG];	/* the threads		*/
	pthread_t      display_thread;
	struct propset props[MAXMSG];	/* properties of string	*/
	void	       *animate();	/* the function		*/
	void	       *display();	/* another function	*/
	int	       num_msg ;	/* number of strings	*/
	int	     i;
	int	     displ_read;	/* fd for display thread */

	if ( ac == 1 ){
		printf("usage: tanimate string ..\n"); 
		exit(1);
	}

	num_msg = setup(ac-1,av+1,props,&displ_read);

	/* create all the threads */
	for(i=0 ; i<num_msg; i++)
		if ( pthread_create(&thrds[i], NULL, animate, &props[i])){
			fprintf(stderr,"error creating thread");
			endwin();
			exit(0);
		}

	/* create display thread */
	if ( pthread_create(&display_thread,NULL,display,&displ_read) ){
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
	for (i=0; i<num_msg; i++ )
		pthread_cancel(thrds[i]);
	endwin();
	return 0;
}

/*
 * create the parameter sets for the threads, start up
 * curses, start up the curses thread
 */

int setup(int nstrings, char *strings[], struct propset props[], int *p)
{
	int num_msg = ( nstrings > MAXMSG ? MAXMSG : nstrings );
	int i;
	int apipe[2];

	/* set up inter-thread channel */
	if ( pipe(apipe) == -1 ){
		perror("pipe");
		exit(1);
	}
	*p = apipe[0];

	/* assign rows and velocities to each string */
	srand(getpid());
	for(i=0 ; i<num_msg; i++){
		props[i].str = strings[i];	/* the message	*/
		props[i].row = i;		/* the row	*/
		props[i].delay = 1+(randnum(15));	/* a speed	*/
		props[i].dir = ((randnum(2))?1:-1);	/* +1 or -1	*/
		props[i].disp = apipe[1];
	}

	/* set up curses */
	initscr();
	crmode();
	noecho();
	clear();
	mvprintw(LINES-1,0,"'Q' to quit, '0'..'%d' to bounce",num_msg-1);

	return num_msg;
}

/* the code that runs in each thread */
void *animate(void *arg)
{
	struct propset *info = arg;		/* point to info block	*/
	int	len = strlen(info->str)+2;	/* +2 for padding	*/
	int	col = rand()%(COLS-len-3);	/* space for padding	*/
	char	*request;

	request = (char *) malloc(50+strlen(info->str));
	if ( request == NULL ){
		perror("malloc");
		endwin();
		exit(1);
	}
	while( 1 )
	{
		usleep(info->delay*TUNIT);

		sprintf(request,"DRAW %d %d\n %s \n", info->row,col,info->str);
		write(info->disp,request,strlen(request));

		/* move item to next column and check for bouncing	*/

		col += info->dir;

		if ( col <= 0 && info->dir == -1 )
			info->dir = 1;
		else if (  col+len >= COLS && info->dir == 1 )
			info->dir = -1;
	}
}
/*
 * function that takes requests from animators and
 * controls the screen
 *
 * requests are text messages of the form
 *    "draw row col\n str"
 *
 * reads from a pipe passed as an argument
 */
void *display(void *arg)
{
	FILE	*fp;
	char	b[BUFSIZ];
	int	row, col;

	fp = fdopen(*(int *)arg,"r");
	if ( fp == NULL ){
		perror("fdopen");
		endwin();
		exit(2);
	}
	while( fgets(b,BUFSIZ,fp) != NULL ){
		fprintf(stderr,"CMD: %s", b);
		if ( strncmp(b,"DRAW ",5) != 0 )
			continue;
		if ( sscanf(b+4,"%d%d", &row, &col) != 2 )
			continue;
		fgets(b,BUFSIZ,fp);
		fprintf(stderr,"  ARG: %s", b);
		b[strlen(b)-1] = '\0';
		mvaddstr(row,col,b);
		move(LINES-1,COLS-1);
		refresh();
	}
	return NULL;
}

/*
 * returns a random number in range 0 .. n-1
 */
int randnum(int n)
{
	return (int)((double)rand()/((double)RAND_MAX+1)*n);
}
