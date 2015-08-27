/** sol11.18s.c
 ** ------------------------------------------------------------
	server
	A sequential number server is
	sol11.18s.c.
	This program is just like the time server except it issues
	a number not a time.  The output is a string, not an integer.
	client
	A sequential number client is
	sol11.18c.c.
	This program is exactly the same code as the time client.

 ** ------------------------------------------------------------
 **
 **
 *   Serial Number Server - Issues the next integer in sequence to each client

 *	build: cc sol11.18s.c -o sol11.18s
 */

#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <unistd.h>
#include  <sys/types.h>
#include  <sys/socket.h>
#include  <netinet/in.h>
#include  <netdb.h>
#include  <time.h>
#include  <strings.h>

#define   PORTNUM  12345  /* the sequence number server port */
#define   HOSTLEN  256
#define   oops(msg)      { perror(msg) ; exit(1) ; }

int main(int ac, char *av[])
{
	struct  sockaddr_in   saddr;   /* build our address here */
	struct	hostent		*hp;   /* this is part of our    */
	char	hostname[HOSTLEN];     /* address 	         */
	int	sock_id,sock_fd;       /* line id, file desc     */

	int	nextnum;		/* the counter		 */

      /*
       * Step 1: ask kernel for a socket
       */

	sock_id = socket( PF_INET, SOCK_STREAM, 0 );    /* get a socket */
	if ( sock_id == -1 ) 
		oops( "socket" );

      /*
       * Step 2: bind address to socket.  Address is host,port
       */

	bzero( (void *)&saddr, sizeof(saddr) ); /* clear out struct     */

	gethostname( hostname, HOSTLEN );       /* where am I ?         */
	hp = gethostbyname( hostname );         /* get info about host  */
	                                        /* fill in host part    */
	bcopy( (void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(PORTNUM);        /* fill in socket port  */
	saddr.sin_family = AF_INET ;            /* fill in addr family  */

	if ( bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0 )
	       oops( "bind" );

      /*
       * Step 3: allow incoming calls with Qsize=1 on socket
       */

	if ( listen(sock_id, 1) != 0 ) 
		oops( "listen" );

      /*
       * initialize the number
       */

	nextnum = 1;

      /*
       * main loop: accept, send a number, close
       */
	while ( 1 ){
		char numbuf[20];

		sock_fd = accept(sock_id, NULL, NULL); /* wait for call */
		if ( sock_fd == -1 )
			oops( "accept" );       /* error getting calls  */

		sprintf(numbuf,"%d\n", nextnum++);
		if ( write(sock_fd, numbuf, strlen(numbuf)) == -1 )
			oops("write");

		close( sock_fd );              /* release connection   */
	}
}
