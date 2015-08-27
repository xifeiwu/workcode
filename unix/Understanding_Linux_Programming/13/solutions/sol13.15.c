/** sol13.15.c
 ** ------------------------------------------------------------
	A version of lclnt2 that asks for a new ticket if its
	current ticket is not valid is
	  sol13.15.c.

	Another extension would be for the program to sleep for
	a short while and try again.  The program could ask if
	it should continue to wait for a ticket to become available
	or just wrap up and try again later.

	An interesting extension of this problem would be to
	replace the sleep with a request for a signal.  Say the
	client asks for a ticket but is told there are none.  The
	client could send its pid to the server and ask to be
	sent a signal when a ticket becomes available.  The client
	could then pause until the server sends a signal.  The
	client would present its pid and ask for its ticket.

 ** ------------------------------------------------------------
 **
 **
 *   Version of lclnt2.c that tries to get a new ticket if ticket
 *   validation fails.  This allows the client to continue if the
 *   had been restarted or has otherwise lost a record of this client's
 *   ticket.
 *
 *   build: cc sol13.15.c lclnt_funcs2.c dgram.c -o sol13.15
 */

/****************************************************************************
 * lclnt2.c
 * License server client version 2
 *  link with lclnt_funcs.o dgram.o
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	get_ticket();
void	do_regular_work();
void	release_ticket();
void	shut_down();
int	validate_ticket();
int	setup();

int main(int ac, char *av[])
{
	setup();
	if (get_ticket() != 0 )
		exit(0);	

	do_regular_work();

	release_ticket();
	shut_down();
	return 0;

}
/****************************************************************************
 * do_regular_work  the main work of the application goes here
 */
void do_regular_work()
{
	printf("SuperSleep version 1.0 Running - Licensed Software\n");
	sleep(15);	/* our patented sleep algorithm */

	if ( validate_ticket() != 0 && get_ticket() != 0 ){
		printf("Server errors. Please Try later.\n");
		return;
	}
	sleep(15);
}

