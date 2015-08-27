/** sol13.12.c
 ** ------------------------------------------------------------
	A version of dgsend.c that works with dgrecv2.c is
	sol13.12.c.

	This program sends a message to a server and then
	waits for an acknowledgement message back.  The
	reply contains the length of the message.  The
	client can use that to make sure the entire message
	arrived.  
	
	This program needs more work.  If the server does not
	reply, the client waits forever.  One solution is for
	the client to set an alarm and break out of the 
	recvfrom upon receipt of the signal.
 */
/*********************************************************************
 * dgsend2.c  - datagram sender
 * 	         usage: dgsend hostname portnum "message"
 * 	        action: sends message to hostname:portnum
 *
 *  build: cc sol13.12.c dgram.c -o sol13.12
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	"dgram.h"

#define oops(m,x)  { perror(m);exit(x);}

int main(int ac, char *av[])
{
	int	sock;			/* use this socket to send	*/
	char	*msg;			/* send this messag		*/
	struct  sockaddr_in   saddr;	/* put sender's address here	*/
	socklen_t saddrlen;		/* and its length here		*/
	int	msglen;
	char	buf[BUFSIZ];

	if ( ac != 4 ){
		fprintf(stderr,"usage: dgsend host port 'message'\n");
		exit(1);
	}
	msg = av[3];

    /* get a datagram socket */

	if( (sock = make_dgram_client_socket()) == -1 )
		oops("cannot make socket",2);

    /* combine hostname and portnumber of destination into an address */

	make_internet_address(av[1], atoi(av[2]), &saddr);

    /* send a string through the socket to that address */

	if ( sendto(sock, msg, strlen(msg), 0, 
			(struct sockaddr*)&saddr, sizeof(saddr)) == -1 )
		oops("sendto failed", 3);

    /* wait for response confirmation that message was received */

	saddrlen = sizeof(saddr);
	msglen = recvfrom(sock, buf, BUFSIZ,0,
		(struct sockaddr *) &saddr,&saddrlen);
	if ( msglen == -1 )
		oops("recvfrom", 4);
	buf[msglen] = '\0';
	printf("Server said: %s\n", buf);
	return 0;
}
