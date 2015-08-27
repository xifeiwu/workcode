/** sol13.11.c
 ** ------------------------------------------------------------
	A revised version of dgrecv.c is
	sol13.11.c.

 ** ------------------------------------------------------------
 **
 **
 *  A version of dgrecv.c that prints time of the message and
 *  also assigns to each message serialized message numbers
 *  and prints the message number.
 */
/************************************************************************
 * dgrecv.c  - datagram receiver
 * 	         usage: dgrecv portnum 
 * 	        action: listens at the specfied port and reports messages
 *
 * build: cc sol13.11.c dgram.c -o sol13.11
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<time.h>

#define oops(m,x)  { perror(m);exit(x);}

int  make_dgram_server_socket(int);
int  get_internet_address(char *, int,  int *, struct sockaddr_in *);
void say_who_called(struct sockaddr_in *);
void say_when(void);
void show_msgnum(int);

int main(int ac, char *av[])
{
	int	port;			/* use this port		*/
	int	sock;			/* for this socket		*/
	char	buf[BUFSIZ];		/* to receive data here		*/
	size_t	msglen;			/* store its length here	*/
	struct  sockaddr_in   saddr;	/* put sender's address here	*/
	socklen_t saddrlen;		/* and its length here		*/

	int	msgnum = 0;
	
	if ( ac == 1 || (port = atoi(av[1])) <= 0 ){
		fprintf(stderr,"usage: dgrecv portnumber\n");
		exit(1);
	}

    /*  get a socket and assign it a port number */

	if( (sock = make_dgram_server_socket(port)) == -1 )
		oops("cannot make socket",2);

    /* receive messaages on that socket */

	saddrlen = sizeof(saddr);
	while( (msglen = recvfrom(sock,buf,BUFSIZ,0,
				(struct sockaddr*)&saddr,&saddrlen))>0 ) {
		buf[msglen] = '\0';
		printf("dgrecv: got a message: %s\n", buf);
		say_who_called(&saddr);
		say_when();
		show_msgnum(++msgnum);
	}
	return 0;
}
void say_who_called(struct sockaddr_in *addrp)
{
	char	host[BUFSIZ];
	int	port;

	get_internet_address(host,BUFSIZ,&port,addrp);
	printf("  from: %s:%d\n", host, port);
}
void say_when(void)
{
	time_t	now;

	time(&now);
	printf("    at: %s", ctime(&now));
}
void show_msgnum(int n)
{
	printf("  msg#: %d\n",n);
}

