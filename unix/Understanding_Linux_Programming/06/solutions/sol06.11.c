/** sol06.11.c
 ** ------------------------------------------------------------
	A version of more0.c that runs in non-canonical, noecho mode,
	handles signals, and knows about the number of lines on the
	terminal is
	sol06.11.c.
	This program shows how much progress we have made since
	Chapter 1.  Since we already know about stat, we could even
	add to the prompt the percentage of the file viewed.

 ** ------------------------------------------------------------
 **
 **
 *   more04.c - A version that runs in cbreak, noecho mode,
 *   		ioctl to find the number of rows on the screen
 *
	build: cc sol06.11.c -o sol06.11
 */

#include	<stdio.h>
#include	<signal.h>
#include	<termios.h>

#define	LINELEN	512
#define	oops(s)  { perror(s); exit(1); }

void do_more(FILE *);
int  see_more(FILE *);

int main(int ac,char *av[])
{
	FILE	*fp;
	int	rv = 0;

	settings(0);
	if ( ac == 1 )
		do_more(stdin);
	else
		while ( --ac )
			if ( (fp=fopen(*++av,"r")) != NULL ) {
				do_more(fp) ; 
				fclose(fp);
			}
			else {
				perror(*av);
				rv = 1;
				break;
			}
	settings(1);
	return rv;
}

/*
 * called for SIGINT and SIGQUIT
 * - Just reset terminal and get out
 */
void handler(int s)
{
	settings(1);
	exit(0);
}

/*
 * program settings are turned on and off here
 */
settings(int how)
{
	void handler(int);
	int	rv;

	static struct termios	term_orig;

	/* read settings */
	if ( how == 0 )
	{
		struct termios		term_new;

		rv = tcgetattr(0, &term_orig);	/* get settings */
		if ( rv == -1 )
			oops("getting settings");
		term_new = term_orig;
		term_new.c_lflag   &= ~ICANON;	/* noncanonical	*/
		term_new.c_lflag   &= ~ECHO;	/* no echo 	*/
		term_new.c_cc[VMIN] = 1;	/* chr-by-chr	*/

		rv = tcsetattr(0, TCSANOW, &term_new);
		if ( rv == -1 )
			oops("changing settings");

		signal(SIGINT,handler);
		signal(SIGQUIT,handler);
	}
	else 
	{
		tcsetattr(0, TCSANOW, &term_orig);
	}
}

void do_more( FILE *fp )
/*
 *  read PAGELEN lines, then call see_more() for further instructions
 */
{
	char	line[LINELEN];
	FILE	*fp_tty;
	int	num_of_lines = 0;
	int	see_more(FILE *), reply;
	int	pagelen = screen_rows(1);

	fp_tty = fopen( "/dev/tty", "r" );	   /* NEW: cmd stream   */
	if ( fp_tty == NULL )			   /* if open fails     */
		exit(1);                           /* no use in running */

	while ( fgets( line, LINELEN, fp ) ){		/* more input	*/
		if ( num_of_lines == pagelen ) {	/* full screen?	*/
			reply = see_more(fp_tty);  /* NEW: pass FILE *  */
			if ( reply == 0 )		/*    n: done   */
				break;
			if ( reply == -1 )
				num_of_lines = 0;
			else
				num_of_lines -= reply;	/* reset count	*/
		}
		if ( fputs( line, stdout )  == EOF )	/* show line	*/
			exit(1);			/* or die	*/
		num_of_lines++;				/* count it	*/
	}
}

int see_more(FILE *cmd)				   /* NEW: accepts arg  */
/*
 *	print message, wait for response, return # of lines to advance
 *	q means no, space means yes, CR means one line
 *	Returns -1 for spacebar (ie. full screen)
 */
{
	int	c;

	printf("\033[7m more? \033[m");		/* reverse on a vt100	*/
	while( (c=getc(cmd)) != EOF )		/* NEW: reads from tty  */
	{
		printf("\r       \r");		/* erase message	*/
		if ( c == 'q' )			/* q -> N		*/
			return 0;
		if ( c == ' ' )			/* ' ' => next page	*/
			return -1;		/* -1 means full page	*/
		if ( c == '\n' )		/* Enter key => 1 line	*/
			return 1;		
		printf("\033[7m more? \033[m");	/* put it back		*/
	}
	return 0;
}

#include <sys/ioctl.h>

/*
 *  returns the number of rows associated with fd
 *  or 24 if that number cannot be determined
 */
int 
screen_rows(fd)
{
	struct winsize wbuf;

	if ( ioctl(fd, TIOCGWINSZ, &wbuf) != -1 )
		return wbuf.ws_row;
	return 24;
}
