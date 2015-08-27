/** sol15.7bclnt.c
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
 *    A time/date client that uses a STREAM socket with a UNIX address
 *
 *    usage: sol15.7bclnt sockname
 *
 *    build: cc sol15.7bclnt.c -o sol15.7bclnt
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<sys/un.h>
#include	<errno.h>

#define oops(s,x)  { perror(s); exit(x); }
#define	BUFLEN	 128


int main(int ac, char *av[])
{
	int	    		sock;	/* receive requests here	*/
	int			fd;
	struct sockaddr_un	myaddr;	/* get caller id here		*/
	socklen_t 		addrlen;
	char			buf[BUFLEN];
	int			msglen;

	if ( ac != 2 ){
		fprintf(stderr,"usage: %s sockname\n", *av);
		exit(1);
	}

	/* ----------------*/
	/*  get a socket   */
	/* ----------------*/
	sock = socket(PF_UNIX, SOCK_STREAM, 0);	/* note PF_UNIX  */
	if ( sock == -1 )
		oops("socket",2);
	printf("have a socket: %d\n", sock);

	/* ------------------------------------*/
	/* make an address for server          */
	/* ------------------------------------*/
	myaddr.sun_family = AF_UNIX;		/* note AF_UNIX		*/
	strcpy(myaddr.sun_path, av[1]);		/* filename is address	*/
	addrlen = strlen(av[1]) + sizeof(myaddr.sun_family);


	/* ----------------- * 
	 * connect to server *
	 * ----------------- */

	fd = connect(sock,(struct sockaddr*)&myaddr,addrlen);

	if ( fd != -1 ){
		printf("Connectd at sock %d\n", sock);
		if( (msglen=read(sock, buf, BUFLEN)) == -1 )
			oops("read",8);
		if (write(1, buf, msglen)==-1 )
			oops("write",9);
		close(fd);
	}
	else if ( errno != EINTR )
		oops("connect",5);
	return 0;
}
