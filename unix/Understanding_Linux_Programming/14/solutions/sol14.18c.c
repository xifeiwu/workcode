/** sol14.18c.c
 ** ------------------------------------------------------------
	The animation engines for the tanimate program is
	  sol14.18c.c
	This program contains the code to run the animation
	and call a screen program to display the strings.

	The screen program is called
	  sol14.18s.c

	This program reads keyboard input in one thread
	and waits and animates in other threads.  

	The program uses datagram sockets to send requests to
	the display server.  It also demonstrates the use of
	connect for dgram sockets in order to define a destination
	for a dgram socket.

 **/
/* sol14.18c.c: animate several strings using threads, usleep() 
 *
 *	bigidea one thread for each animated string
 *		one thread for keyboard control
 *		datagram socket to send display controls
 *
 *	usage:  sol14.18c [-h hostname] [-p portnum] string1 ..
 *
 *	build:	cc sol14.18c.c sol14.18disp.c dgram.c -lpthread -o sol14.18c
 *
 *	notes:  default host is localhost
 *		default port is 2828
 */

#include	<stdio.h>
#include	<string.h>
#include	<pthread.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<termios.h>

#include	"sol14.18disp.h"

#define	MAXMSG	10		/* limit to number of strings	*/
#define	TUNIT   20000		/* timeunits in microseconds */

#define	MSGLEN	100	

struct	propset {
		char	*str;	/* the message */
		int	row;	/* the row     */
		int	delay;  /* delay in time units */
		int	dir;	/* +1 or -1	*/
		int	cols;	/* width of display */
	};

int	setup(int nstrings, char *args[], struct propset props[]);
void	*animate(void *arg);
int	randnum(int n);
int	set_cr_noecho_mode();
int	tty_mode(int how);
void	usage();

int main(int ac, char *av[])
{
	int	       c;		/* user input		*/
	pthread_t      thrds[MAXMSG];	/* the threads		*/
	struct propset props[MAXMSG];	/* properties of string	*/
	void	       *animate();	/* the function		*/
	int	       num_msg ;	/* number of strings	*/
	int	     i;

	if ( ac == 1 )
		usage();

	num_msg = setup(ac-1,av+1,props);
	if ( num_msg == -1 ){
		perror("setting terminal");
		exit(1);
	}

	/* create all the threads */
	for(i=0 ; i<num_msg; i++)
		if ( pthread_create(&thrds[i], NULL, animate, &props[i])){
			fprintf(stderr,"error creating thread");
			exit(0);
		}

	/* process user input */
	while(1) {
		c = getchar();
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
	tty_mode(1);
	return 0;
}

int setup(int nstrings, char *args[], struct propset props[])
{
	int i;
	int lines, cols;
	char msg[MSGLEN];
	char *strings[MAXMSG];
	int num_msg = 0;
	char *hostname = NULL;
	int portnum    = 0;

	/*
	 * extract args and string from command line
	 */
	for(i = 0 ; i< nstrings; i++ )
	{
		if( strcmp(args[i],"-h") == 0 ){
			if ( ++i < nstrings )
				hostname = args[i];
			else
				usage();
		}
		else if ( strcmp(args[i],"-p") == 0 ){
			if ( ++i < nstrings )
				portnum = atoi(args[i]);
			else
				usage();
		}
		else if ( num_msg < MAXMSG )
			strings[num_msg++] = args[i];
	}

	/* set up screen control and screen */
	display_init(hostname,portnum);
	lines = atoi(display_info("lines", msg, MSGLEN));
	cols  = atoi(display_info("cols", msg, MSGLEN));
	sprintf(msg, "'Q' to quit, '0'..'%d' to bounce",num_msg-1);
	display_draw(-1,0,msg);

	/* assign rows and velocities to each string */
	srand(getpid());
	for(i=0 ; i<num_msg; i++){
		props[i].str = strings[i];	/* the message	*/
		props[i].row = i;		/* the row	*/
		props[i].delay = 1+(randnum(15));	/* a speed	*/
		props[i].dir = ((randnum(2)%2)?1:-1);	/* +1 or -1	*/
		props[i].cols = cols;
	}

	/* set up tty mode */
	tty_mode(0);
	if ( set_cr_noecho_mode() == -1 )
		return -1;

	return num_msg;
}

/* the code that runs in each thread */
void *animate(void *arg)
{
	struct propset *info = arg;		/* point to info block	*/
	int	len = strlen(info->str)+2;	/* +2 for padding	*/
	int	col = randnum(info->cols-len-3);	/* space for padding	*/
	char	*padded;

	/* make a padded version of the string */
	padded = (char *)malloc(3+strlen(info->str));
	if ( padded == NULL ){
		perror("malloc");
		exit(2);
	}
	sprintf(padded," %s ",info->str);

	while( 1 )
	{
		usleep(info->delay*TUNIT);

		display_draw(info->row,col,padded);

		/* move item to next column and check for bouncing	*/

		col += info->dir;

		if ( col <= 0 && info->dir == -1 )
			info->dir = 1;
		else if (  col+len >= info->cols && info->dir == 1 )
			info->dir = -1;
	}
}
/*
 * returns a random number in range 0 .. n-1
 */
int randnum(int n)
{
	return (int)((double)rand()/((double)RAND_MAX+1)*n);
}
/*
 * from play_again2.c
 */
int set_cr_noecho_mode()
/* 
 * purpose: put file descriptor 0 into chr-by-chr mode and noecho mode
 *  method: use bits in termios
 */
{
	struct	termios	ttystate;

	if (tcgetattr(0,&ttystate) == -1 )	/* read curr. setting	*/
		return -1;
	ttystate.c_lflag    	&= ~ICANON;	/* no buffering		*/
	ttystate.c_lflag    	&= ~ECHO;	/* no echo either	*/
	ttystate.c_cc[VMIN]  	=  1;		/* get 1 char at a time	*/
	if ( tcsetattr(0,TCSANOW,&ttystate) == -1 )/* install settings	*/
		return -1;
	return 0;
}

/* how == 0 => save current mode,  how == 1 => restore mode */
int tty_mode(int how)
{
	static struct termios original_mode;
	if ( how == 0 )
		return tcgetattr(0, &original_mode);
	else
		return tcsetattr(0, TCSANOW, &original_mode); 
}

void usage()
{
	printf("usage: sol14.18c [-l host] [-p port]  string ..\n"); 
	exit(1);
}
