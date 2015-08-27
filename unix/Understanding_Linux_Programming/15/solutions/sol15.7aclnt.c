/** sol15.7aclnt.c
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
 *    A time/date client that uses
 *    a datagram, internet socket
 *
 *    Note: since dgram sockets are not connected, the only way
 *    the server can be notified that a client wants the time is
 *    for the client to send any message to the server.
 *
 *    usage: sol15.7aclnt hostname portnum 
 *
 *    build: cc sol15.7aclnt.c dgram.c -o sol15.7aclnt
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	"dgram.h"

#define oops(s,x)  { perror(s); exit(x); }
#define	BUFLEN	 128
#define	HOSTLEN	 256

#define oops(s,x) { perror(s); exit(x); }

int main(int ac, char *av[])
{
	int	      	sock;		/* receive requests here	*/
	struct sockaddr server;		/* make server id here		*/
	char		buf[BUFLEN];
	int		msglen;

	if ( ac != 3 ){
		fprintf(stderr,"usage: %s hostname portnum\n", *av);
		exit(1);
	}

	/* ----------------*/
	/* set up a socket */
	/* ----------------*/
	sock = make_dgram_client_socket();
	if ( sock == -1 )
		oops("make_dgram_server_socket",2);

	make_internet_address(av[1], atoi(av[2]), (struct sockaddr_in*)&server);

	if ( sendto(sock,"",0,0,&server,sizeof(server)) == -1 )
		oops("sendto",3);

	if ( (msglen = read(sock,buf,BUFLEN)) == -1 )
		oops("read",4);

	if ( write(1, buf, msglen) == -1 )
		oops("write",5);

	close(sock);
	return 0;
}
