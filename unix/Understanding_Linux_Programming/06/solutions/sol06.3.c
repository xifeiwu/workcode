/** sol06.3.c
 ** ------------------------------------------------------------
	A version of play_again3.c that implements timeout on input
	by using VTIME in the driver is:
	sol06.3.c.
	Except for using the timeout feature in select, this method
	is the cleanest, simplest solution.

 ** ------------------------------------------------------------
 **
 **
 *  play_again3.c with termios timeout rather than no-blocking
 *
 *	purpose: ask if user wants another transaction
 *	 method: set tty into chr-by-chr, no-echo mode
 *		 set timeout to 6 seconds
 *		 read char, return result
 *	returns: 0=>yes, 1=>no, 2=>timeout
 *
	build: cc sol06.3.c -o sol06.3
 */
#include	<stdio.h>
#include	<termios.h>
#include	<fcntl.h>
#include	<string.h>

#define	ASK		"Do you want another transaction"
#define TIMEOUT  6                                       /* time per try */
#define	BEEP       putchar('\a')                         /* alert user */

main()
{
	int	response;

	tty_mode(0);				/* save current mode	*/
	set_input_mode(TIMEOUT);		/* -icanon -echo vtime	*/
	response = get_response(ASK);		/* get some answer	*/
	tty_mode(1);				/* restore orig mode	*/
	putchar('\n');
	return response;
}

get_response( char *question )
/*
 * purpose: ask a question and wait for a y/n answer or maxtries
 *  method: use getchar and complain about non-y/n input
 * returns: 0=>yes, 1=>no, 2=>timeout
 */
{
	int	input;

	printf("%s (y/n)?", question);			/* ask		*/
	fflush(stdout);					/* force output	*/
	while ( 1 ){
		input = tolower(getchar());            /* get next chr */
		if ( input == 'y' )
			return 0;
		if ( input == 'n' )
			return 1;
		if ( input == EOF )			/* timeout	*/
			return 2;			/* sayso	*/
		BEEP;
	}
}

set_input_mode(int timeout)
/* 
 * purpose: put file descriptor 0 into chr-by-chr mode and noecho mode
 *          AND set VTIME to 10*timeout
 *          See manpage for termios to learn how values of VMIN and
 *          VTIME work
 *  method: use bits in termios
 */
{
	struct	termios	ttystate;

	tcgetattr( 0, &ttystate);		/* read curr. setting	*/
	ttystate.c_lflag    	&= ~ICANON;	/* no buffering		*/
	ttystate.c_lflag    	&= ~ECHO;	/* no echo either	*/
	ttystate.c_cc[VMIN]  	=  0;		/* any # of chars is ok	*/
	ttystate.c_cc[VTIME]  	=  10*timeout;	/* timeout on input	*/
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
