/** sol11.13.c
 ** ------------------------------------------------------------
	(a)
	There are many solutions to this problem.  I do not believe
	I can say with certainty exactly which screening algorithms
	are secure, and I do not want to devise one that will turn
	out to be a security hole.  The purpose of this exercise is
	to show how tricky this problem can be.  The safest method
	is not to try to screen out patterns but instead to run the
	command directly.  That way, you can focus on preventing
	buffer overflows and illegal directory paths.

	(b)
	A version of rlsd.c that runs ls directly is
	sol11.13.c.
	This program uses fork, exec, dup2, and wait to pass a string
	as an argument to ls.  There is no shell to interpret special
	characters.  The program calls wait to prevent the child from
	becoming a zombie.  There are other ways to prevent a child from
	becoming a zombie, so you do not need to call wait if you use
	one of the other techniques.

 ** ------------------------------------------------------------
 **
 **
 *   A version of rlsd.c that uses fork and exec to run ls
 *   directly rather than using popen and risk passing a
 *   string to the shell.
 *

 *	build: cc sol11.13.c -o sol11.13
 */

#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <unistd.h>
#include  <sys/types.h>
#include  <sys/wait.h>
#include  <sys/socket.h>
#include  <netinet/in.h>
#include  <netdb.h>
#include  <time.h>
#include  <strings.h>

#define   PORTNUM  15000   /* our remote ls server port */
#define   HOSTLEN  256
#define   oops(msg)      { perror(msg) ; exit(1) ; }

void trimcrnl(char *);
void run_ls(char *, int);

#define	LS_CMD	"/bin/ls"	/* safer than using $PATH */

int main(int ac, char *av[])
{
	struct  sockaddr_in   saddr;   /* build our address here */
	struct	hostent		*hp;   /* this is part of our    */
	char	hostname[HOSTLEN];     /* address 	         */
	int	sock_id,sock_fd;       /* line id, file desc     */
	FILE	*sock_fpi;	       /* streams for in and out */
	char    dirname[BUFSIZ];       /* from client            */

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

	       /* open reading direction as buffered stream */
	       if( (sock_fpi = fdopen(sock_fd,"r")) == NULL )
		       oops("fdopen reading");

	       if ( fgets(dirname, BUFSIZ-5, sock_fpi) == NULL )
		       oops("reading dirname");
	       trimcrnl(dirname);

	       /* now just run ls with stdout attached to sock_fd */

	       run_ls(dirname, sock_fd);
	       fclose(sock_fpi);	/* this closes sock_fd */
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
/*
 * run ls as a child with stdout attached to sock_fd */
void run_ls(char *dirname, int sock_fd)
{
	pid_t	pid;

	pid = fork();
	if ( pid == -1 )
		oops("fork");
	/*
	 * child runs ls
	 */
	if ( pid == 0 ){
		printf("Request for '%s'\n", dirname);
		if ( dup2(sock_fd, 1) == -1 || dup2(sock_fd, 2) == -1 )
			oops("dup2");
		execl(LS_CMD, "ls", "-l", dirname, NULL);
		oops(LS_CMD);
	}
	wait(NULL);
}
