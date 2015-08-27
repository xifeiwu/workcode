/** sol06.6.c
 ** ------------------------------------------------------------
	A version of rotate.c that supports line editing is
	sol06.6.c.
	Writing this program is good preparation for reading
	the source to the editing part of the terminal driver.
	The system V terminal driver was notorious for allowing
	the user to backspace off the left margin, consuming the
	shell prompt and data on the previous line.

	Note: This program handles tab characters the best it
	      can given that it does not keep track of which
	      characters the user has typed.  The tab key will
	      advance the cursor to the next tab column (traditionally
	      8 chars apart).  Pressing the erase key still causes
	      the cursor to back up one char, but the program knows
	      where on the screen it is.
 ** ------------------------------------------------------------
 **
 **
 * An enhanced version of rotate.c that runs in noncanonical mode but
 * also supports simple line editing.
 *
 * This program turns off canonical mode and turns off echo before
 * before it begins processing input and turns the modes back when
 * the letter "Q" is received.
 *
 * rotate.c : map a->b, b->c, .. z->a
 *   purpose: useful for showing tty modes

	build: cc sol06.6.c -o sol06.6
 */

#include	<stdio.h>
#include	<ctype.h>
#include	<termios.h>

#define	TABWIDTH	8

int main()
{
	int	c;
	int	erase, kill;
	int	pos;	/* position from left margin */

	/* set up */
	tty_mode(0);			/* store it */
	get_control_chars(&erase,&kill);
	set_cr_noecho_mode();		/* set -icanon, -echo */
	pos = 0;

	/* loop */
	while ( ( c=getchar() ) != EOF && c != 'Q' )
	{
		/* handle erase by backing up one spot */
		if ( c == erase ) {
			if ( pos > 0 ){
				printf("\b \b");
				pos--;
			}
			continue;
		}

		/* handle kill by backing up until left margin */
		if ( c == kill ){
			while( pos > 0 ){
				printf("\b \b");
				pos--;
			}
			continue;
		}

		/* handle mapping */
		if ( c == 'z' )  
			c = 'a';
		else if (islower(c))
			c++;

		putchar(c);

		/* handle line position */
		if ( isprint(c) )
			pos++;
		else if ( c == '\t' )
			pos += ( TABWIDTH-(pos%TABWIDTH) );
		else if ( c == '\n' )
			pos = 0;
	}

	/* wrapup */
	tty_mode(1);			/* restore it */
	return 0;
}

/*
 * Read current erase and kill chars from driver.
 * Put those values into the ints pointed to by args
 */
get_control_chars(int *erasep, int *killp)
{
	struct	termios	ttystate;
	tcgetattr( 0, &ttystate);		/* read curr. setting	*/

	*erasep = ttystate.c_cc[VERASE];
	*killp  = ttystate.c_cc[VKILL];
}

set_cr_noecho_mode()
/* 
 * purpose: put file descriptor 0 into chr-by-chr mode and noecho mode
 *    ALSO: turn off ISIG in lflags to prevent generation of SIGINT SIGQUIT
 *  method: use bits in termios
 */
{
	struct	termios	ttystate;

	tcgetattr( 0, &ttystate);		/* read curr. setting	*/
	ttystate.c_lflag    	&= ~ICANON;	/* no buffering		*/
	ttystate.c_lflag    	&= ~ECHO;	/* no echo either	*/
	ttystate.c_cc[VMIN]  	=  1;		/* get 1 char at a time	*/
	ttystate.c_lflag    	&= ~ISIG;	/* no input signals	*/
	tcsetattr( 0 , TCSANOW, &ttystate);	/* install settings	*/
}

/* how == 0 => save current mode,  how == 1 => restore mode */
/* this version handles termios and fcntl flags             */

tty_mode(int how)
{
	static struct termios original_mode;
	static int            original_flags;
	if ( how == 0 )
		tcgetattr(0, &original_mode);
	else 
		tcsetattr(0, TCSANOW, &original_mode); 
}
