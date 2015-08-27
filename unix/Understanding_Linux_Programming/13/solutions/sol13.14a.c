/** sol13.14a.c
 ** ------------------------------------------------------------
	A version of the server that only reclaims tickets when
	a client is denied a ticket consists of the two files:
	  sol13.14a.c (main) and
	  sol13.14b.c (functions).
	
	If the server only reclaims tickets on a fixed schedule,
	it uses more CPU time overall, but provides quicker
	response time to clients, and may deny a client a
	ticket even if there are reclaimable ones in the table.
	If the server only reclaims tickets when a client is
	denied a ticket, the server will use less CPU time
	overall, may cause a delay when a client requests a
	ticket, but will never deny a client when it could
	have been granted a ticket.

 ** ------------------------------------------------------------
 **
 **
 *   A version of the lserv that reclaims tickets only when the table
 *   fills up.  This one does not call alarm() to schedule ticket
 *   reclamation
 */
/****************************************************************************
 * lsrv2.c  
 * License server server program version 2 - features ticket recycling
 *
 * build:  cc sol13.14a.c sol13.14b.c dgram.c -o sol13.14
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/errno.h>

#define	MSGLEN		128

int	setup();
void	handle_request(char *,struct sockaddr *, socklen_t );
void	narrate(char *, char *, struct sockaddr *);

int main(int ac, char *av[])
{
	struct sockaddr client_addr;
	socklen_t addrlen;
	char      buf[MSGLEN];
	int	  ret, sock;

	sock = setup();

	while(1) {
		addrlen = sizeof(client_addr);
		ret = recvfrom(sock,buf,MSGLEN,0,&client_addr,&addrlen);
		if ( ret != -1 ){
			buf[ret] = '\0';
			narrate("GOT:", buf, &client_addr);
			handle_request(buf,&client_addr,addrlen);
		}
		else if ( errno != EINTR )
			perror("recvfrom");
	}
}
