/** sol14.18disp.c
 * ----------------------------------------------------------
 * display subsystem here
 *
 *   The display functions are encapsulated in this part
 *   of the program.  The interface is simple:
 *
 *      display_init(hostname,port)
 *      display_draw(row,col,message)
 *      display_clear()
 *      char *display_info("var",char val[], int vallen)
 *
 *   In this program, the display subsystem uses a dgram socket
 *   to send requests to a separate server that controls the
 *   screen.
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<netdb.h>
#include	<string.h>
#include	"dgram.h"

#include	"sol14.18disp.h"

#define oops(s,x) { perror(s); exit(x); }
#define MSGLEN  128

static int sock = -1;
static struct sockaddr_in server_addr;
static socklen_t addrlen;

/*
 * set up system to talk to server
 *   - make address
 *   - get socket
 * args:
 *   hostname: if NULL use DFL_HOST
 *   port    : if <= 0 use DFL_PORT
 */

void display_init(char *hostname, int portnum)
{
	char *host;
	int  port;

	sock = make_dgram_client_socket();
	if ( sock == -1 )
		oops("socket",1);

	host = ( hostname == NULL ? DFL_HOST : hostname );
	port = ( portnum  <= 0    ? DFL_PORT : portnum  );

	if (make_internet_address(host, port, &server_addr) == -1 )
		oops("making address",2);
}

/*
 * send a messge to the server
 */

int tell_server(char *msg)
{
	addrlen = sizeof(server_addr);
	if ( sendto(sock,msg,strlen(msg),0,
				(struct sockaddr *)&server_addr,addrlen) == -1 )
		oops("sendto",3);
	return 0;
}

/*
 * get a message from the server
 */
int from_server(char buf[], int l)
{
	struct sockaddr_in caller;
	socklen_t          callerlen = sizeof(caller);
	int		   n;

	n = recvfrom(sock,buf,l,0,(struct sockaddr *)&caller,&callerlen);
	if ( n == -1 )
		oops("recvfrom",4);
	buf[n] = '\0';
	return 0;
}


void display_clear()
{
	tell_server("CLEAR");
}

int display_draw(int r, int c, char *str)
{
	char	*msg;

	msg = (char *)malloc(strlen(str)+1+128);	
	if ( msg == NULL )
		oops("malloc",5);
	sprintf(msg,"DRAW %d %d %s",r,c,str);
	tell_server(msg);
	free(msg);
	return 0;
}

/*
 * Ask server for an info string and then
 * extract the value we need
 */
char *
display_info(char *var, char val[], int vallen)
{
	char	msg[MSGLEN];
	int	lines, cols;

	tell_server("SIZE");
	from_server(msg, MSGLEN);
	*val = '\0';
	if ( sscanf(msg,"%d%d", &lines, &cols) == 2 ){
		if ( strcmp(var,"lines") == 0 )
			sprintf(val,"%d",lines);
		if ( strcmp(var,"cols") ==  0 )
			sprintf(val,"%d",cols);
	}
	return val;
}

