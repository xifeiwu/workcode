/** sol13.17.c
 ** ------------------------------------------------------------
	A version of lserv_funcs2.c that stores the IP number of
	the client in the ticket is
	sol13.17.c.
	The main function for the server requires no changes,
	and the client requires no changes.  For them, a ticket
	is a magic cookie.

 ** ------------------------------------------------------------
 **
 **
 *   Version of lserv_funcs2.c that includes IP address in ticket
 */
/****************************************************************************
 * lsrv_funcs2.c
 * functions for the license server - version 2
 *    includes ticket_reclaim(), do_validate(), and expanded handle_request
 *
 * build: cc lserv2.c sol13.17.c dgram.c -o sol13.17
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <string.h>
#include "dgram.h"

#define SERVER_PORTNUM	2020		/* Our server's port number */
#define MSGLEN		128		/* Size of our datagrams */
#define TICKET_AVAIL	0		/* Slot is available for use */
#define MAXUSERS	3		/* Only 3 users for us */
#define	oops(x)	{ perror(x); exit(-1); }

#define RECLAIM_INTERVAL 5		/* Expire every 60 seconds */

int	setup();
void	free_all_tickets();
void	shut_down();
void	handle_request(char *,struct sockaddr *, socklen_t );
static	char *do_hello(char *, struct sockaddr *);
static	char *do_goodbye(char *, struct sockaddr *);
void	narrate(char *, char *, struct sockaddr *);
static	char *do_validate(char *,struct sockaddr *);
void	ticket_reclaim();

/****************************************************************************
 * Important variables
 */
struct ticket { pid_t           pid;
	        struct in_addr  host;
	      };

struct ticket ticket_array[MAXUSERS];	/* Our ticket array */
int sd = -1;				/* Our socket */
int num_tickets_out = 0;		/* Number of tickets outstanding */

static char *do_hello();
static char *do_goodbye();
static char *do_validate();

/****************************************************************************
 * setup() - initialize license server
 */
int setup()
{
	sd = make_dgram_server_socket(SERVER_PORTNUM);
	if ( sd == -1 )
		oops("make socket");
	free_all_tickets();
	return sd;
}
void free_all_tickets()
{
	int	i;

	for(i=0; i<MAXUSERS; i++)
		ticket_array[i].pid = TICKET_AVAIL;
}

/****************************************************************************
 * shut_down() - close down license server
 */
void shut_down()
{
	close(sd);
}

/****************************************************************************
 * handle_request(request, clientaddr, addrlen)
 *   branch on code in request
 */
void handle_request(char *req,struct sockaddr *client, socklen_t addlen)
{
	char	*response;
	int	ret;

	/* act and compose a response */
	if ( strncmp(req, "HELO", 4) == 0 )
		response = do_hello(req,client);
	else if ( strncmp(req, "GBYE", 4) == 0 )
		response = do_goodbye(req,client);
	else if ( strncmp(req, "VALD", 4) == 0 )
		response = do_validate(req,client);
	else
		response = "FAIL invalid request";

	/* send the response to the client */
	narrate("SAID:", response, client);
	ret = sendto(sd, response, strlen(response),0,client,addlen);
	if ( ret == -1 )
		perror("SERVER sendto failed");
}

/****************************************************************************
 * do_hello
 * Give out a ticket if any are available
 * IN  msg_p			message received from client
 * IN  clientaddr               ptr to sockaddr of client
 * Results: ptr to response
 *    NOTE: return is in static buffer overwritten by each call
 */
static char *do_hello(char *msg_p, struct sockaddr *client)
{
	int x;
	static char replybuf[MSGLEN];
	struct in_addr ip_address;

	if(num_tickets_out >= MAXUSERS) 
		return("FAIL no tickets available");

	/* else find a free ticket and give it to client */

	for(x = 0; x<MAXUSERS && ticket_array[x].pid != TICKET_AVAIL; x++) 
		;

	/* A sanity check - should never happen */
	if(x == MAXUSERS) {
		narrate("database corrupt","",NULL);
		return("FAIL database corrupt");
	}

	/* Found a free ticket.  Record "name" of user (pid) in array.
	 * 	generate ticket of form: pid.slot.ipaddr
	 */
	ip_address = ((struct sockaddr_in *)client)->sin_addr;
	ticket_array[x].pid  = atoi(msg_p + 5);   /* get pid in msg */
	ticket_array[x].host = ip_address;	  /* set the hostid */
	sprintf(replybuf, "TICK %d.%d.%s", ticket_array[x].pid, x,
				   inet_ntoa(ip_address));
	num_tickets_out++;
	return(replybuf);
} /* do_hello */

/****************************************************************************
 * do_goodbye
 * Take back ticket client is returning
 * IN  msg_p			message received from client
 * IN  clientaddr               ptr to sockaddr of client
 * Results: ptr to response
 */
static char *do_goodbye(char *msg_p, struct sockaddr *client)
{
	int pid, slot;		/* components of ticket	*/
	char addr[MSGLEN];	

	/* The user's giving us back a ticket.  First we need to get
 	 * the ticket out of the message, which looks like:
	 *
	 * 	GBYE pid.slot.ipaddress
	 */
	if((sscanf((msg_p + 5), "%d.%d.%s", &pid, &slot, addr) != 3) ||
	   (ticket_array[slot].pid != pid) ||
	   (strcmp(addr, inet_ntoa(ticket_array[slot].host))!=0)
	   ) {
		narrate("Bogus ticket", msg_p+5, NULL);
		return("FAIL invalid ticket");
	}

	/* The ticket is valid.  Release it. */
	ticket_array[slot].pid = TICKET_AVAIL;
	num_tickets_out--;

	/* Return response */
	return("THNX See ya!");
} /* do_goodbye */

/****************************************************************************
 * narrate() - chatty news for debugging and logging purposes
 */
void narrate(char *msg1, char *msg2, struct sockaddr *clientp)
{
	struct sockaddr_in *p = (struct sockaddr_in *) clientp;

	fprintf(stderr,"\t\tSERVER: %s %s ", msg1, msg2);
	if ( clientp )
		fprintf(stderr,"(%s:%d)", inet_ntoa(p->sin_addr),
					  ntohs(p->sin_port) );
	putc('\n', stderr);
}

/* ------------------------ VERSION 2 ADDITIONS HERE --------------------- */


/****************************************************************************
 * do_validate
 * Validate client's ticket
 * IN  msg_p			message received from client
 * IN  clientaddr               ptr to sockaddr of client
 * Results: ptr to response
 */
static char *do_validate(char *msg,struct sockaddr *client)
{
	int  pid, slot;          /* components of ticket */
	char addr[MSGLEN];	 /* another component    */

	struct in_addr caller_ip = ((struct sockaddr_in*)client)->sin_addr;
	int            addrlen   = sizeof(struct in_addr);

	/* msg looks like VALD pid.slot.ipaddr - parse it and validate */
	 
	if ( sscanf(msg+5,"%d.%d.%s",&pid,&slot,addr)==3 
             && ticket_array[slot].pid == pid
 	     && strcmp(addr, inet_ntoa(ticket_array[slot].host)) == 0
	     && memcmp(&caller_ip,&(ticket_array[slot].host),addrlen) == 0 
	   )
		return("GOOD Valid ticket");

	/* bad ticket */
	narrate("Bogus ticket", msg+5, NULL);
	return("FAIL invalid ticket");
} 


/****************************************************************************
 * ticket_reclaim
 * go through all tickets and reclaim ones belonging to dead processes
 * Results: none
 */
void ticket_reclaim()
{
	int	i;
	char	tick[BUFSIZ];

	for(i = 0; i < MAXUSERS; i++) {
		if((ticket_array[i].pid != TICKET_AVAIL) &&
		   (kill(ticket_array[i].pid, 0) == -1) && (errno == ESRCH)) {
			/* Process is gone - free up slot */
			sprintf(tick, "%d.%d.%s", ticket_array[i].pid,i,
					inet_ntoa(ticket_array[i].host));
			narrate("freeing", tick, NULL);
			ticket_array[i].pid = TICKET_AVAIL;
			num_tickets_out--;
		}
	}
	alarm(RECLAIM_INTERVAL);	/* reset alarm clock */
} 

