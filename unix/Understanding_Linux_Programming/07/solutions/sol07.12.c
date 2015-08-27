/** sol07.12.c
 ** ------------------------------------------------------------
	A version of sigdemo3.c that uses one handler for
	both SIGINT and SIGQUIT is
	sol07.12.c.
	This program shows how one handler can be used for
	more than one type of signal.  The handler contains
	a line of common code and two lines of signal-specific
	code.

	This program raises the question of how a single
	handler for two different signals affects the arrival
	of SIGX while the program is handling SIGY.  The
	handler is the same, but the signals are different.
	Can an instance of SIGQUIT interrupt the handling
	of SIGINT, even if the handler is the same?  That is,
	does the handler or the signal determine if a handler
	is interrupted.  On Linux 2.2.14, the single handler
	version operates the same way the version with two
	different handlers does.

 ** ------------------------------------------------------------
 **
 **
 *   A revision of sigdemo3.c to show how one handler can service
 *   multiple signals.  The handler is called with the signal number
 *   so one function can take different actions based on the argument.
 *
 *	purpose:   show answers to signal questions
 *	question1: does the handler stay in effect after a signal arrives?
 *	question2: what if a signalX arrives while handling signalX?
 *      question3: what if a signalX arrives while handling signalY?
 *      question4: what happens to read() when a signal arrives?
 *
 *      build: cc sol07.12.c -o sol07.12
 */

#include	<stdio.h>
#include	<signal.h>

#define	INPUTLEN	100

main(int ac, char *av[])
{
	void	bothhandler(int);
	char	input[INPUTLEN];
	int	nchars;

	signal( SIGINT,  bothhandler );		/* set handler */
	signal( SIGQUIT, bothhandler );		/* set handler */

	do {
		printf("\nType a message\n");
		nchars = read(0, input, (INPUTLEN-1));
		if ( nchars == -1 )
			perror("read returned an error");
		else {
			input[nchars] = '\0';
			printf("You typed: %s", input);
		}
	} 
	while( strncmp( input , "quit" , 4 ) != 0 );
}

void bothhandler(int s)
{
	printf(" Received signal %d .. waiting\n", s );
	if ( s == SIGINT ){
		sleep(2);
		printf("  Leaving inthandler \n");
	}
	else if ( s == SIGQUIT ){
		sleep(3);
		printf("  Leaving quithandler \n");
	}
}

