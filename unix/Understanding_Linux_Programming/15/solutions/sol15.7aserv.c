/** sol15.7aserv.c
 ** ------------------------------------------------------------
	This problem asks for two of the four combinations
	in the datagram/stream, Internet/Unix socket
	space.

	(a)
	A client/server pair using datagram sockets in
	the AF_INET are 
	sol15.7aclnt.c 
	and
	sol15.7aserv.c.

	(b)
	A client/server pair using stream socket in
	the AF_UNIX are 
	sol15.7bclnt.c 
	and
	sol15.7bserv.c.

 ** ------------------------------------------------------------
 **
 *    A time/date server that uses
 *    a datagram, internet socket
 *
 *    Note: since dgram sockets are not connected, the only way
 *    the server can be notified that a client wants the time is
 *    for the client to send any message to the server.
 *
 *    usage: sol15.7aserv portnum 
 *
 *    build: cc sol15.7aserv.c dgram.c -o sol15.7aserv
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<time.h>
#include	"dgram.h"

#define oops(s,x)  { perror(s); exit(x); }
#define	BUFLEN	 128


int main(int ac, char *av[])
{
	int	    	sock;	/* receive requests here	*/
	struct sockaddr	caller;	/* get caller id here		*/
	int		len;	/* with this length address	*/
	time_t		now;
	char		buf[BUFLEN];
	int		msglen;

	if ( ac != 2 ){
		fprintf(stderr,"usage: %s portnum\n", *av);
		exit(1);
	}

	/* set up a socket */
	sock = make_dgram_server_socket(atoi(av[1]));
	if ( sock == -1 )
		oops("make_dgram_server_socket",2);

	while(1){

		/* wait for any request */
		len = sizeof(caller);
		msglen =  recvfrom(sock, buf, BUFLEN, 0, &caller, &len);
		if ( msglen == -1 )
			perror("recvfrom");
		else
		{
			/* send back a reply */
			time(&now);
			strcpy(buf,ctime(&now));
			if( sendto(sock,buf,strlen(buf),0,&caller,len)==-1 )
				perror("sendto");
		}
	}
	return 0;
}
