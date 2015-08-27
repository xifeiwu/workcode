/** sol06.8.c
 ** ------------------------------------------------------------
	A version of sigdemo1.c with a SIGINT handler that asks
	the user if the program should exit is
	sol06.8.c.
	An interesting thing to test is how the program behaves
	if you press Ctrl-C when the program is waiting for the
	yes/no answer.  Experiment with the program to see exactly
	how the Ctrl-C is handled.

 ** ------------------------------------------------------------
 **
 **
 *   sigdemo1 modified to ask user if the program should quit
 *   Note:    What happens if you press Ctrl-C when answer the
 *            question?
 *
	build: cc sol06.8.c -o sol06.8
 */

#include	<stdio.h>
#include	<signal.h>

main()
{
	void	f(int);			/* declare the handler	*/
	int	i;

	signal( SIGINT, f );		/* install the handler	*/
	for(i=0; i<5; i++ ){		/* do something else	*/
		printf("hello\n");
		sleep(1);
	}
}

void f(int signum)			/* this function is called */
{
	char	ans[BUFSIZ];

	printf("   Interrupted!  OK to quit (y/n)? ");
	if ( fgets(ans, BUFSIZ, stdin) != NULL && (ans[0]=='y'||ans[0]=='Y') )
		exit(0);
}
