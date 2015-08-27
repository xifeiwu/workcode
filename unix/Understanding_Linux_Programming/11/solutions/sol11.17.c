/** sol11.17.c
 ** ------------------------------------------------------------
	A cache for user info requests makes sense because the database
	of users does not change often (the way the time does), and also
	because searching for user information by looking through the
	passwd file requires file access, which can take time.  If a 
	server gets a lot of requests for user information, it can
	eliminate the need to search the disk file and speed access 
	by keeping the data in a hash table, tree, or sorted list.

	A finger server that reads the entire passwd database into an array
	of structs is
	sol11.17.c.
	The data stored in memory is good until the user database is
	changed.  Users are not usually added or changed on a fixed
	schedule, so it does not make sense to set a timeout period.

	A better system is to have some method for forcing the server
	to reload its table.  A simple method is to kill the server and
	start it again.  The newly started server will read the current
	table into memory.  Another system is to program the server
	to reload the table when it receives a particular signal.  For
	example, one could add a handler for SIGUSR1 that would clear
	the old table and reload the table.  Several standard Unix
	servers use this technique.

       	If you use the technique of having a signal handler invoke a reload
	function, you have to be careful of race conditions.  Consider
	what would happen if a signal arrives when if the server 
	is in the middle of searching the table.  Control jumps to
	the signal handler, the table is rebuilt, but the lookup function
	still holds a pointer to some item in the table.  That pointer may
	point to deallocated space.

	The table, therefore, is an object that must be protected from
	being changed by one function while another function is using it.
	Ignoring or blocking signals are bad ideas for two reasons.  The question
	of how to share resources has come up a few times.  In later chapters we
	shall see some techniques for avoiding interference.

 ** ------------------------------------------------------------
 **
 **
 *   A caching version of fingerd.  
 *   This program is based on sol11.14, but this one reads the entire list from
 *   getpwent() into an array then searches that array to handle each request.
 *
 *   This program accepts a single name from the
 *   client and uses getpwent to search the userlist for a matching entry.
 *   This server does not use fork; the server does the work itself.
 *
 *   You can test this one by using telnet to connect to port 7979 and
 *   then typing any part of any username on the system.
 *

 *	build: cc sol11.17.c -o sol11.17
 */

#include  <stdio.h>
#include  <stdlib.h>
#include  <malloc.h>
#include  <unistd.h>
#include  <pwd.h>
#include  <sys/types.h>
#include  <sys/socket.h>
#include  <netinet/in.h>
#include  <netdb.h>
#include  <time.h>
#include  <strings.h>
#include  <string.h>
#include  <signal.h>

#define   PORTNUM  7979   /* regular finger is 79 */
#define   HOSTLEN  256
#define   oops(msg)      { perror(msg) ; exit(1) ; }

void trimcrnl(char *);
void get_userinfo(char *, FILE *);

int main(int ac, char *av[])
{
	struct  sockaddr_in   saddr;   /* build our address here */
	struct	hostent		*hp;   /* this is part of our    */
	char	hostname[HOSTLEN];     /* address 	         */
	int	sock_id,sock_fd;       /* line id, file desc     */
	FILE	*sock_fpi, *sock_fpo;  /* streams for in and out */
	char    request[BUFSIZ];       /* from client            */


      /** Step 1: ask kernel for a socket **/

	sock_id = socket( PF_INET, SOCK_STREAM, 0 );    /* get a socket */
	if ( sock_id == -1 ) 
		oops( "socket" );

      /** Step 2: bind address to socket.  Address is host,port **/

	bzero( (void *)&saddr, sizeof(saddr) ); /* clear out struct     */
	gethostname( hostname, HOSTLEN );       /* where am I ?         */
	hp = gethostbyname( hostname );         /* get info about host  */
	bcopy( (void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(PORTNUM);        /* fill in socket port  */
	saddr.sin_family = AF_INET ;            /* fill in addr family  */
	if ( bind(sock_id, (struct sockaddr *)&saddr, sizeof(saddr)) != 0 )
	       oops( "bind" );

      /** Step 3: allow incoming calls with Qsize=1 on socket **/

	if ( listen(sock_id, 1) != 0 ) 
		oops( "listen" );

      /*
       * main loop: accept(), write(), close()
       */

	while ( 1 ){
	       sock_fd = accept(sock_id, NULL, NULL); /* wait for call */
	       if ( sock_fd == -1 )
		       oops("accept");       

	       /* convert fd into two buffered streams */
	        if( (sock_fpi = fdopen(sock_fd,"r")) == NULL )
		       oops("fdopen reading");
		if ( (sock_fpo = fdopen(sock_fd,"w")) == NULL )
			oops("fdopen writing");

	       if ( fgets(request, BUFSIZ-5, sock_fpi) == NULL )
		       oops("reading request");
	       trimcrnl(request);

		get_userinfo(request, sock_fpo);
	       
	       fclose(sock_fpo);	/* flush, free(), and close */
	       fclose(sock_fpi);	/* this only has to free()  */
       }
       return 0;
}

/*
 * This is important.  The request might arrive with trailing newlines
 * and carriage returns.  This function trims them all off
 */
void trimcrnl(char *str)
{
	int	c;

	while ( ( c = str[strlen(str)-1]) == '\n' || c == '\r' )
		str[strlen(str)-1] = '\0';
}

/* -----------------------------------------------------------------------
 * userlist functions are all down here
 * The struct used is
 */
struct userinfo{ char	*ul_name, *ul_gecos;  };

/*
 * a list of them
 */
struct userinfo *userlist = NULL ;

/*
 * get_userinfo(char *lookfor, FILE *output)
 *
 * Get user by searching through the table stored in userlist[]
 * Write results to *fp
 */
void get_userinfo(char *rq, FILE *fp)
{
	struct userinfo *ulp;
	struct userinfo *get_userlist();

	if  ( fprintf(fp, "%-10s %s\n", "LOGNAME", "REALNAME") == EOF )
		perror("fprintf");

	/* if list is empty, fill it up */
	if ( userlist == NULL )
		userlist = get_userlist();

	/* search */
	for( ulp = userlist ; ulp->ul_name != NULL ; ulp++ )
		if ( strstr(ulp->ul_name, rq) || strstr(ulp->ul_gecos, rq)  )
			fprintf(fp, "%-10s %s\n", ulp->ul_name, ulp->ul_gecos);
}

/*
 * list filling function here
 */

void *emalloc(size_t);
char *newstr(char *s);

/*
 * Make an array of userinfo structs from getpwent().  Return that array.
 * First, figure out how many
 * structs we need for the array, allocate the space and then traverse again to fill
 * the table.
 */
struct userinfo *
get_userlist()
{
	struct passwd   *pwp;
	int	        numentries = 0, i;
	struct userinfo *list;

	/* count them first */
	while( (pwp = getpwent()) != NULL )
		numentries++;
	setpwent();

	/* allocate space */
	list = (struct userinfo *) emalloc((1+numentries) * sizeof(struct userinfo));
	
	/* fill the array */
	for( i = 0 ; i<numentries ; i++ )
		if ( (pwp = getpwent()) != NULL ){
			list[i].ul_name  = newstr(pwp->pw_name);
			list[i].ul_gecos = newstr(pwp->pw_gecos);
		}
	/* add sentinel  and close passwd db */
	list[i].ul_name = NULL;
	endpwent();

	return list;
}

void *emalloc(size_t n)
{
	char *rv;
	if ( (rv = (char*)malloc(n)) != NULL )
		return rv;
	oops("malloc");
}

char *newstr(char *s)
{
	char *rv;

	rv = emalloc(1+strlen(s));
	strcpy(rv, s);
	return rv;
}
