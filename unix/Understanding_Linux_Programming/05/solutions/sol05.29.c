/** sol05.29.c
 ** ------------------------------------------------------------
	A version of more0.c that knows about the number of lines on the
	terminal is
	sol05.29.c.

	This program shows how to use ioctl to read the number of
	rows from the driver.  

 ** ------------------------------------------------------------
 **
 **
 *   more03.c - a version of more that uses ioctl to
 *   	        find the number of rows on the screen
 *
	build: cc sol05.29.c -o sol05.29
 */

#include	<stdio.h>

#define	LINELEN	512

void do_more(FILE *);
int  see_more(FILE *);

int main( int ac , char *av[] )
{
	FILE	*fp;

	if ( ac == 1 )
		do_more( stdin );
	else
		while ( --ac )
			if ( (fp = fopen( *++av , "r" )) != NULL )
			{
				do_more( fp ) ; 
				fclose( fp );
			}
			else
				exit(1);
	return 0;
}

void do_more( FILE *fp )
/*
 *  read PAGELEN lines, then call see_more() for further instructions
 */
{
	char	line[LINELEN];
	int	num_of_lines = 0;
	int	see_more(FILE *), reply;
	FILE	*fp_tty;
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
		if ( c == 'q' )			/* q -> N		*/
			return 0;
		if ( c == ' ' )			/* ' ' => next page	*/
			return -1;		/* -1 means full page	*/
		if ( c == '\n' )		/* Enter key => 1 line	*/
			return 1;		
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
