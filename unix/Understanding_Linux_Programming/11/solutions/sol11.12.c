/** sol11.12.c
 ** ------------------------------------------------------------
	A version of the timeserver that only tells the time to
	callers from IP numbers listed in a file of authorized
	hosts is
	sol11.12.c.
	This solution is an extension of the ideas in exercise
	11.9.

 ** ------------------------------------------------------------
 **
 **
 *  A version of timeserv.c that only talks to clients connecting from
 *  a set of IP numbers listed in a file provided on the command line.
 *  of the caller.  Shows how to use inet_ntoa() to check the caller.
 *
 *  usage: timesrv [listofipnums]
 *
 *  If no file given on the command line, tell the time to anyone
 *
 * timeserv.c - a socket-based time of day server

 *	build: cc sol11.12.c -o sol11.12
 */

#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <unistd.h>
#include  <sys/types.h>
#include  <sys/socket.h>
#include  <netinet/in.h>
#include  <netdb.h>
#include  <time.h>
#include  <strings.h>
#include  <arpa/inet.h>

#define   PORTNUM  13000   /* our time service phone number */
#define   HOSTLEN  256
#define   oops(msg)      { perror(msg) ; exit(1) ; }

#define	VERBOSE	

int authorized(char **, struct in_addr);
char **get_authorized(char *);

int main(int ac, char *av[])
{
	struct  sockaddr_in   saddr;   /* build our address here */
	struct	hostent		*hp;   /* this is part of our    */
	char	hostname[HOSTLEN];     /* address 	         */
	int	sock_id,sock_fd;       /* line id, file desc     */
	FILE	*sock_fp;              /* use socket as stream   */
	char    *ctime();              /* convert secs to string */
	time_t  thetime;               /* the time we report     */
	struct  sockaddr_in clnt_addr; /* who is calling us      */
	int     sin_len;               /* and the length         */

	char	**okcallers = NULL ;   /* used for screening     */

	if ( ac > 1 )
		okcallers = get_authorized(av[1]);
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

	while ( 1 )
	{
		/* 
		 * remember to set the length variable 
		 * before calling accept
		 */
	       sin_len = sizeof(struct sockaddr);
	       sock_fd = accept(sock_id, 
			       	(struct sockaddr *)&clnt_addr, &sin_len);

	       if ( sock_fd == -1 )	       /* check for errors     */
		       oops( "accept" );

	       /*
		* if screening and not ok, close and take next call
		*/
	       if ( okcallers && !authorized(okcallers,clnt_addr.sin_addr))
	       {
		       close(sock_fd);
		       continue;
	       }

	       sock_fp = fdopen(sock_fd,"w");  /* we'll write to the   */
	       if ( sock_fp == NULL )          /* socket as a stream   */
		       oops( "fdopen" );       /* unless we can't      */

	       thetime = time(NULL);           /* get time             */
					       /* and convert to strng */
	       fprintf( sock_fp, "The time here is .." );
	       fprintf( sock_fp, "%s", ctime(&thetime) ); 
	       fclose( sock_fp );              /* release connection   */
	}
	return 0;
}

/*
 *  looks up ipnumber on list, returns 1 if found, 0 if not
 */
int authorized(char **list, struct in_addr addr)
{
	char	*ip_number ;
	int	i;

	/* inet_ntoa converts an address to #.#.#.# format */
	ip_number = inet_ntoa(addr);

#ifdef VERBOSE
	printf("Call from %s\n", ip_number);
#endif
	for(i=0; list[i] && strcmp(ip_number,list[i])!=0; i++ )
		printf("comparing '%s' to '%s'\n", ip_number, list[i]);
	return ( list[i] != NULL );
}

/*
 * Read a file of IP numbers and build a list.
 * scan once to count addresses, then scan again to build
 * the list.  Put a null at end of list. Return ptr to list
 */
char **
get_authorized(char *file)
{
	char	**list;
	FILE	*fp;
	int	numwords = 0;
	int	i;
	char	*newstr;
	char	buf[BUFSIZ];

	if ( (fp = fopen(file, "r")) == NULL)
		oops(file);

	/*
	 * Read the file once to count the number of words
	 * then allocate space for the list
	 */
	while( fscanf(fp, "%100s", buf) == 1 )
		numwords++;
	list = (char **) malloc((1+numwords) * sizeof(char *));
	if ( list==NULL )
		oops("malloc");

	/*
	 * Then rewind the file and read the words into
	 * the array.
	 */
	rewind(fp);
	i = 0;
	while( i < numwords && fscanf(fp, "%100s", buf) == 1 )
	{
		newstr = (char *)malloc(1+strlen(buf));
		if ( newstr == NULL )
			oops("malloc");
		list[i++] = strcpy(newstr, buf);
	}
	list[i] = NULL;
	return list;
}
