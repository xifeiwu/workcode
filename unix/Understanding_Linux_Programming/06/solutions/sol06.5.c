/** sol06.5.c
 ** ------------------------------------------------------------
	A version of rotate.c that controls the terminal driver
	itself and ignores signals is
	sol06.5.c.

 ** ------------------------------------------------------------
 **
 **
 * A version of rotate.c that turns off canonical mode and turns off
 * echo before it begins processing input and turns the modes back
 * when the letter "Q" is received.
 *
 * rotate.c : map a->b, b->c, .. z->a
 *   purpose: useful for showing tty modes

	build: cc sol06.5.c -o sol06.5
 */

#include	<stdio.h>
#include	<ctype.h>
#include	<termios.h>

int main()
{
    int c;

    tty_mode(0);			/* store it */
    set_cr_noecho_mode();		/* set -icanon, -echo */

    while ( ( c=getchar() ) != EOF && c != 'Q' ){
        if ( c == 'z' )  
            c = 'a';
        else if (islower(c))
            c++;
        putchar(c);
    }
    tty_mode(1);			/* restore it */
    return 0;
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
