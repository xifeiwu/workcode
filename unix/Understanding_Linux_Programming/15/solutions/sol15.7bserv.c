/** sol15.7bserv.c
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
 *    A time/date server that uses a STREAM socket with a UNIX address
 *
 *    usage: sol15.7bserv sockname
 *
 *    build: cc sol15.7bserv.c -o sol15.7bserv
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<sys/un.h>
#include	<time.h>
#include	<errno.h>

#define oops(s,x)  { perror(s); exit(x); }
#define	BUFLEN	 128


int main(int ac, char *av[])
{
	int	    		sock;	/* receive requests here	*/
	int			fd;
	struct sockaddr_un	myaddr;	/* get caller id here		*/
	socklen_t 		addrlen;
	time_t			now;
	char			buf[BUFLEN];

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

	/* ----------------*/
	/* make an address */
	/* ----------------*/
	unlink(av[1]);				/* remove it if there	*/
	myaddr.sun_family = AF_UNIX;		/* note AF_UNIX		*/
	strcpy(myaddr.sun_path, av[1]);		/* filename is address	*/
	addrlen = strlen(av[1]) + sizeof(myaddr.sun_family);

	/* ----------------*/
	/* bind the addres */
	/* ----------------*/
	if ( bind(sock, (struct sockaddr *) &myaddr, addrlen) == -1 )
		oops("bind", 3);

	/* ----------------*/
	/* enable call-ins */
	/* ----------------*/
	if ( listen(sock,1) == -1 )
		oops("listen",4);

	/* ----------------*/
	/* main loop       */
	/* ----------------*/
	while(1)
	{
		printf("SRV waiting on sock %d\n", sock);
		fd = accept(sock,NULL,NULL);
		printf("SRV Got a call %d\n", fd);
		if ( fd != -1 ){
			time(&now);
			strcpy(buf,ctime(&now));
			if (write(fd, buf, strlen(buf))==-1 )
				oops("write",9);
			close(fd);
		}
		else if ( errno != EINTR )
			oops("accept",5);
	}
	return 0;
}
