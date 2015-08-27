/** sol11.16.c
 ** ------------------------------------------------------------
	A caching proxy time server is
	sol11.16.c.
	This program demonstrates two important ideas: a proxy and 
	caching.  This one program includes the code from timeserv.c
	and timeclnt.c.  As a server, it receives requests at port 13000.
	It gets the actual time by being a client and sending a request
	to a time server on another machine.  That's the proxy part.

	It then stores the time it gets in a buffer.  If another request
	comes in soon, the server does not have to go back to the real
	time server; it can use the time is has in its cache.  On the
	other hand, if it has been a few seconds since the last request,
	the time in the cache is old news.

	Disk caching was discussed in Chapter 2.  How does this sort of
	caching compare to caching in Chapter 2?

 ** ------------------------------------------------------------
 ** 
 *   A time server proxy.  This program accepts connections on port 
 *   13000 and then acts a client to get the time from another server 
 *   at another port.  Those values are #define'd in, but could be 
 *   added as command line args.
 *
 *   This program caches the data.  It records the time at which it 
 *   read the time from the `real' server.   If a new request arrives 
 *   and the time is less than a second old, this server sends back 
 *   the information it has in its cache.
 *
 *   If the date is more than a second old, the server gets new, 
 *   fresh data, updates the cache, and then dispenses the data.
 *
 *   This program combines timeserv and timeclnt
 *

 *	build: cc sol11.16.c -o sol11.16
 *
 *   usage: sol11.16 timehost timeport
 */

#include  <stdio.h>
#include  <stdlib.h>
#include  <unistd.h>
#include  <sys/types.h>
#include  <sys/socket.h>
#include  <netinet/in.h>
#include  <netdb.h>
#include  <time.h>
#include  <sys/time.h>
#include  <strings.h>

#define   PORTNUM  13000   /* our time service phone number */
#define   HOSTLEN  256
#define   oops(msg)      { perror(msg) ; exit(1) ; }

#define	  SERVERHOST "mit.edu"
#define   SERVERPORT 13
#define	  DATELEN    50

#define   STALE	     1	  /* more than this is old news */

int	timediff(struct timeval *, struct timeval *);
char    *thetime(char *host, int port); 	/* get time here */

int main(int ac, char *av[])
{
	struct  sockaddr_in   saddr;   /* build our address here */
	struct	hostent		*hp;   /* this is part of our    */
	char	hostname[HOSTLEN];     /* address 	         */
	int	sock_id,sock_fd;       /* line id, file desc     */
	FILE	*sock_fp;              /* use socket as stream   */
	char	*tshost;	       /* server host            */
	int	tsport;                /*    and port            */

      /*
       * Step 0: check args
       */

	if ( ac != 3 ){
		fprintf(stderr,"usage: sol11.16 timehost timeport\n");
		exit(0);
	}
	tshost = av[1];
	tsport = atoi(av[2]);

      /*
       * Step 1: ask kernel for a socket
       */

	sock_id = socket( PF_INET, SOCK_STREAM, 0 );    /* get a socket */
	if ( sock_id == -1 ) 
		oops( "socket" );

      /*
       * Step 2: bind address to socket.  Address is host,port
       */

	bzero( (void *)&saddr, sizeof(saddr) ); /* clear out struct     */

	gethostname( hostname, HOSTLEN );       /* where am I ?         */
	hp = gethostbyname( hostname );         /* get info about host  */
	                                        /* fill in host part    */
	bcopy( (void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(PORTNUM);        /* fill in socket port  */
	saddr.sin_family = AF_INET ;            /* fill in addr family  */

	if ( bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0 )
	       oops( "bind" );

      /*
       * Step 3: allow incoming calls with Qsize=1 on socket
       */

	if ( listen(sock_id, 1) != 0 ) 
		oops( "listen" );

      /*
       * main loop: accept(), write(), close()
       */

	while ( 1 ){
	       sock_fd = accept(sock_id, NULL, NULL); /* wait for call */
		printf("Wow! got a call!\n");
	       if ( sock_fd == -1 )
		       oops( "accept" );       /* error getting calls  */

	       sock_fp = fdopen(sock_fd,"w");  /* we'll write to the   */
	       if ( sock_fp == NULL )          /* socket as a stream   */
		       oops( "fdopen" );       /* unless we can't      */

	       fprintf( sock_fp, "The time here is .." );
	       fprintf( sock_fp, "%s", thetime(tshost, tsport) );
	       fclose( sock_fp );              /* release connection   */
	}
	return 0;
}

/*
 * cache part is here
 *
 * The function  thetime()  returns a pointer to the time/date stored in
 * a cache.  Before returning that pointer, the function checks if the
 * date in the cache is stale.  If so, the cache is refreshed and then
 * the date is returned.
 */

char *
thetime(char *host, int port)
{
	void get_time_from_server(char *, char *, int);
	static char	        now[DATELEN] = "" ;	/* cache	*/
	static struct timeval   lastmod = { 0, 0} ;	/* timestamp	*/
	struct timeval		rightnow;
	struct timezone		tz;
	int    age;

	if ( gettimeofday(&rightnow, &tz) == -1 )
		return "Error";
	printf("   Cache is %d seconds old", 
			(age = timediff(&rightnow, &lastmod)) );
	if ( !*now || age > STALE ){
		printf(" [refilling]");
		get_time_from_server(now, host, port);
		gettimeofday(&lastmod, &tz);
	}
	putchar('\n');
	return now;
}

/*
 * returns number of seconds between two times
 */
int
timediff(struct timeval *nowp, struct timeval *thenp)
{
	return ( (1000000*(nowp->tv_sec - thenp->tv_sec) + 
				(nowp->tv_usec - thenp->tv_usec))
					/1000000);
}

/*
 * client part is here
 *
 * The function get_time_from_server(char *buf)
 * gets the time from a time server and puts the string into buf
 */

#define	fail(s,b)	{ sprintf(b,"error: %s", s); return; }

void get_time_from_server(char *buf, char *host, int port)
{
	struct sockaddr_in  servadd;        /* the number to call */
	struct hostent      *hp;            /* used to get number */
	int    sock;                        /* the socket         */
	int    messlen;

     /*
      * Step 1: Get a socket
      */

	sock = socket( AF_INET, SOCK_STREAM, 0 );    /* get a line   */
	if ( sock == -1 ) 
		fail("socket", buf);

     /*
      * Step 2: connect to server
      *         need to build address (host,port) of server  first
      */

	bzero( &servadd, sizeof( servadd ) );   /* zero the address     */

	hp = gethostbyname( host );             /* lookup host's ip #   */
	if (hp == NULL) 
		fail(host, buf);
	bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);

	servadd.sin_port = htons(port);         /* fill in port number  */

	servadd.sin_family = AF_INET ;          /* fill in socket type  */

						       /* now dial     */
	if ( connect(sock,(struct sockaddr *)&servadd, sizeof(servadd)) !=0)
		fail("connect", buf);

     /*
      * Step 3: transfer data from server, then hangup
      */

	messlen = read(sock, buf, DATELEN);     /* read stuff   */
	if ( messlen == - 1 )
	       fail("read", buf);
	buf[messlen] = '\0';
	close( sock );    
}
