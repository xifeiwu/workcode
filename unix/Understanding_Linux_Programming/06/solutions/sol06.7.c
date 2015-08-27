/** sol06.7.c
 ** ------------------------------------------------------------
	An version of sigdemo1.c that counts the number of
	SIGINTs received is
	sol06.7.c.
	This program provides more practice getting used to
	coding for signal handlers and introduces the question
	of passing data to a handler using a global variable.

 ** ------------------------------------------------------------
 **
 **
 *  Version of sigdemo1.c that counts SIGINTS.
 *  Details: At each SIGINT, the program prints OUCH followed
 *           a number of exclamation points equal to the number of
 *           SIGINTS received.
 *           The program accepts an integer as command line argument.
 *           When the number of SIGINTS reaches that value, the 
 *           program exits.
 *
	build: cc sol06.7.c -o sol06.7
 */

#include	<stdio.h>
#include	<signal.h>

/*
 * number of SIGINTs before quitting
 * This value has to be global so the handler can see it
 */
int quitnum = 10;

main(int ac, char *av[])
{
	void	f(int);			/* declare the handler	*/
	int	i;

	if ( ac>1 && atoi(av[1])>0 )	/* handle args		*/
		quitnum = atoi(av[1]);

	signal( SIGINT, f );		/* install the handler	*/

	while(1){			/* main loop		*/
		printf("hello\n");
		sleep(1);
	}
}

/*
 * the handler for SIGINT
 *   Print a message and count number of SIGINTS received
 *   Quit when the number reaches quitnum
 */

void f(int signum)
{
	static int hits = 0;		/* hit counter		*/
	int	   i;			/* loop counter		*/

	printf("OUCH");
	hits++;
	for(i=0; i<hits; i++)
		putchar('!');
	putchar('\n');
	if ( hits == quitnum )
		exit(0);
}
