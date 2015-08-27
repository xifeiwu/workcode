/** sol07.9.c
 ** ------------------------------------------------------------
	A ticker-tape/marquee display viewer for files is
	sol07.9.c.
	This program is much more interesting than problem 7.8.
	The solution here uses an object-oriented approach by
	creating a ticker-tape-display object and feeding characters
	into that object.  That solution is good preparation for
	some pipe/socket and client/server oriented solutions 
	later in the book.

 ** ------------------------------------------------------------
 **
 **
 *   Ticker tape display of file contents
 *
 *   usage: ttape len row col speed [file ..]
 *
 *   places a ticker tape display at (row,col) and runs
 *   the data past it one char per T where T is 
 *   1000/speed milliseconds. That is, a speed of 1000 is 
 *   one char per 1 millisecond , and a speed of 1 is one
 *   char per second.
 *
 *   build:  cc sol07.9.c -lcurses -o sol07.9
 *   
 *   If filename is not provided read from stdin; a potentially
 *   useful application.
 *
 *   This program uses sort of an object oriented approach by
 *   creating a ticker object into which characters are fed.
 */

#include	<curses.h>
#include	<stdio.h>
#include	<unistd.h>
#include	<string.h>

/* coulda been an enum */
#define		T_INIT	0
#define		T_ADDCH	1

main(int ac, char *av[])
{
	FILE	*data;
	int	params[4], speed;
	char	*buf;
	int	i;

	/*
	 * setup the args, the buffers, and screen 
	 */
	if ( ac < 5 ){
		fprintf(stderr,"usage: ttape len row col speed [file..]\n");
		exit(1);
	}
	params[0] = atoi(av[1]);
	params[1] = atoi(av[2]);
	params[2] = atoi(av[3]);
	speed     = atoi(av[4]);
	params[3] = 1000000 / ( speed > 0 ? speed : 1 );

	ticker(T_INIT,0,params);

	/*
	 * process all input data
	 */

	if ( ac == 5 )
		tickfile(stdin);
	else
		for(i=5; i<ac; i++)
			if ( data = fopen(av[i],"r") ){
				tickfile(data);
				fclose(data);
			}
			else {
				perror(av[i]);
				break;
			}
	endwin();
}

/*
 * tickfile(FILE *)
 *   send the data from stream into the ticker object
 */

tickfile(FILE *fp)
{
	int	c;

	while( (c = getc(fp)) != EOF ){
		if ( c == '\n' || c == '\r' || c == '\t' )
			c = ' ';
		ticker(T_ADDCH,c,NULL);
	}
}

/*
 * a ticker object takes an operation as first arg and
 * then args as rest. Ops are T_INIT, T_SHOW
 */

ticker(int op, int c, int params[])
{
	static int   row, col, len, delay;
	static char *buf;		

	/* constructor */
	if ( op == T_INIT ){
		len = params[0];
		row = params[1];
		col = params[2];
		delay = params[3];
		if ( (buf=(char*)malloc(len+1)) == NULL ){
			perror("out of memory");
			exit(1);
		}
		memset(buf,' ',len);
		buf[len] = '\0';
		initscr();
		clear();
	}
	/* add a char to the ticker */
	else if ( op == T_ADDCH )
	{
		/* shift buffer leftward */
		memmove(buf,buf+1,len-1);
		buf[len-1] = c;
		mvaddstr(row,col,buf);
		move(LINES-1,COLS-1);
		refresh();
		usleep(delay);
	}
}

