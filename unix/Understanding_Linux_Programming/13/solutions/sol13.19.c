/** sol13.19.c
 ** ------------------------------------------------------------
	This solution adds the name of the user submitting the
	message to the message.  This is different from the
	problem of adding the IP address of the caller to 
	network records because the user id is not part of
	the connection information.  Therefore, the client has
	to determine the user and add that name to the data.
	A solution is
	sol13.19.c.

 ** ------------------------------------------------------------
 **
 *   Version of the logfile client that automatically puts the
 *   name of the user at the start of the message
 */
/**********************************************************************
 * logfilec.c - logfile client - send messages to the logfile server
 *              usage: logfilec "a message here"
 *
 * build: cc sol13.19.c -o sol13.19
 */

#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<sys/un.h>
#include	<unistd.h>
#include	<pwd.h>
#include	<stdlib.h>

#define	SOCKET	"/tmp/logfilesock"
#define	oops(m,x) { perror(m); exit(x); }
char *make_message(char *);

int main(int ac, char *av[])
{
	int	           sock;
	struct sockaddr_un addr;
	socklen_t          addrlen;
	char	           sockname[] = SOCKET ;
	char               *msg = make_message(av[1]);

	if ( ac != 2 ){
		fprintf(stderr,"usage: logfilec 'message'\n");
		exit(1);
	}
	sock = socket(PF_UNIX, SOCK_DGRAM, 0);
	if ( sock == -1 )
		oops("socket",2);

	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, sockname);
	addrlen = strlen(sockname) + sizeof(addr.sun_family) ;

	if ( sendto(sock,msg, strlen(msg), 0, 
				(struct sockaddr *)&addr, addrlen) == -1 )
		oops("sendto",3);
	return 0;
}

/*
 * Make a message with the name of the current user prepended
 * returns a dynamically allocated string
 */

char *
make_message(char *s)
{
	struct passwd	*p = getpwuid(getuid());
	char   		*user;
	char		*result;

	if ( p )
		user = p->pw_name;
	else
		user = "(unknown)";

	result = (char *)malloc(strlen(user)+strlen(s)+4);
	if ( result == NULL ){
		perror("no memory");
		exit(1);
	}
	sprintf(result,"%s: %s",user,s);
	return result;
}
