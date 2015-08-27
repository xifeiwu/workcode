/** sol13.20.c
 ** ------------------------------------------------------------
	A program that uses, by means of #ifdefs', 
	read(), recv(), or recvfrom() to get a message from
	a datagram socket is
	sol13.20.c.
	The basic differences is that recv and recvfrom receive
	messages at a socket, and read receives messages at
	any input object.  The read system call uses the type
	of the thing referred to by the file descriptor to
	determine how to receive the message.  The advantage of
	recvfrom is it records the address of the sending socket.
	recv does not provide the address of the sending socket.

 ** ------------------------------------------------------------
 **
 ** 
 *  Versions of logfiled.c using recvfrom and recv.
 *  Just to see how they differ and how to use them.
 *
 *  Set the following symbols to get various versions
 */

// #define	USE_READ	// for read()
// #define	USE_RECV	// for recv()
// #define	USE_RECVFROM	// for recvfrom()

/*************************************************************************
 * logfiled.c  - a simple logfile server using Unix Domain Datagram Sockets
 * 	         usage: logfiled >>logfilename
 *
 * build: cc -DUSE_READ sol13.20.c -o sol13.20.read
 * build: cc -DUSE_RECV sol13.20.c -o sol13.20.recv
 * build: cc -DUSE_RECVFROM sol13.20.c -o sol13.20.recvfrom
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<sys/un.h>
#include	<time.h>

#define	MSGLEN	512
#define oops(m,x) { perror(m); exit(x); }
#define	SOCKNAME "/tmp/logfilesock"

int main(int ac, char *av[])
{
	int	sock;			/* read messages here	*/
	struct sockaddr_un addr;	/* this is its address	*/
	socklen_t addrlen;
	char	msg[MSGLEN];
	int	l;
	char	sockname[] = SOCKNAME ;
	time_t	now;
	int	msgnum = 0;
	char	*timestr;

	/* build an address */
	addr.sun_family = AF_UNIX;		/* note AF_UNIX */
	strcpy(addr.sun_path, sockname);	/* filename is address */
	addrlen = strlen(sockname) + sizeof(addr.sun_family);

	sock = socket(PF_UNIX, SOCK_DGRAM, 0);	/* note PF_UNIX  */
	if ( sock == -1 )
		oops("socket",2);

	/* bind the address */
	if ( bind(sock, (struct sockaddr *) &addr, addrlen) == -1 )
		oops("bind", 3);

	/* read and write */
	while(1)
	{
#ifdef USE_READ
		l = read(sock, msg, MSGLEN);	/* read works for DGRAM	*/
#endif
#ifdef	USE_RECV
		l = recv(sock, msg, MSGLEN, 0);	/* recv works too	*/
#endif
#ifdef	USE_RECVFROM
		l = recvfrom(sock, msg, MSGLEN, 0, NULL, NULL);
#endif
		msg[l] = '\0';			/* make it a string 	*/
		time(&now);
		timestr = ctime(&now);
		timestr[strlen(timestr)-1] = '\0';	/* chop newline */

		printf("[%5d] %s %s\n", msgnum++, timestr, msg);
		fflush(stdout);
	}
}

