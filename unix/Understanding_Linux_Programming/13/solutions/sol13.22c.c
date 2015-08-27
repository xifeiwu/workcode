/***********************************************************************
 * sol13.22c.c - a datagram based client for the card dealer server
 *
 * 	         usage: cardc get #
 * 	        action: asks dealer for # cards
 * 	        output: a string of two-char card names separated
 * 	                by spaces.  For example
 * 	                4D AH 4C TD KC
 *
	A server for the card dealer is
	sol13.22s.c

	A client for the card dealer is
	sol13.22c.c

	The server accepts a port number as an argument and an optional
	argument specifying the number of decks it holds.  The server
	simply deals cards until it runs out of cards.

	The client places a "GET" request to obtain a number of cards.
	The server returns a string consisting of space-separated
	card identifiers.  Each card identifier is a two-character
	string of the form VS where V is the value and S is the suit.

	The protocol needs to be expanded to allow for returning cards,
	setting up a new deck, asking the server for the number of
	cards left in the stack, or other operations.  Which operations
	you add depend on the application.  

	Requests are handled sequentially, so the server does
	not have to worry about race conditions and locks.

	If the server has fewer cards than the client asks for, the
	server dispenses what it has.  This might not be the correct
	response for some applications.  By changing the protocol
	so the server does not give out any cards if it cannot
	satisfy the entire request, the card server can be used as
	a way to explain semaphores.

 *  build: cc sol13.22c.c dgram.c -o sol13.22c
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>

#define oops(m,x)  { perror(m);exit(x);}

int make_dgram_client_socket();
int make_internet_address(char *,int, struct sockaddr_in *);

#define	CARDD_PORT	3322

int main(int ac, char *av[])
{
	int	sock;			/* use this socket to send	*/
	struct  sockaddr_in   saddr;	/* put sender's address here	*/
	socklen_t saddrlen;		/* and its length here		*/
	int	msglen;
	char	buf[BUFSIZ];
	int	rv = 0;

	if ( ac != 3 || strcasecmp(av[1],"get")!=0 || atoi(av[2]) <= 0 ){
		fprintf(stderr,"usage: cardc get number\n");
		exit(1);
	}

    /* get a datagram socket */

	if( (sock = make_dgram_client_socket()) == -1 )
		oops("cannot make socket",2);

    /* combine hostname and portnumber of destination into an address */

	make_internet_address("localhost", CARDD_PORT, &saddr);

    /* send a string through the socket to that address */

	sprintf(buf, "GET %s", av[2]);
	if ( sendto(sock, buf, strlen(buf), 0, 
			(struct sockaddr*)&saddr, sizeof(saddr)) == -1 )
		oops("sendto failed", 3);

    /* wait for response.  Start will be ERR or OK */

	saddrlen = sizeof(saddr);
	msglen = recvfrom(sock, buf, BUFSIZ,0,
		(struct sockaddr *) &saddr,&saddrlen);
	if ( msglen == -1 )
		oops("recvfrom", 4);
	buf[msglen] = '\0';
	if ( strncmp(buf, "OK ", 3) == 0 )
		printf("%s\n", buf+3);
	else {
		printf("%s\n", buf);
		rv = 1;
	}
	return rv;
}
