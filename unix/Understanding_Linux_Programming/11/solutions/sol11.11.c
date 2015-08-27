/** sol11.11.c
 ** ------------------------------------------------------------
	A version of tinybc.c using a single socketpair is
	sol11.11.c.
	This program is much simpler than the two-pipe version.
	The important new idea here is that each end of the connection
	uses a different end of the socketpair.  One process uses
	pipe[0] for reading and writing, and the other process
	uses pipe[1] for reading and writing.

 ** ------------------------------------------------------------
 **
 *   a version of tinybc using a socketpair
 *
 *   It could be done with bidirectional pipes, too.  Not all versions of
 *   Unix support bidirectional pipes.  socketpairs are more widely 
 *   supported.
 *
 *   Each element in the array is a socket and may be used for reading
 *   and for writing.  So one process uses one socket and the other
 *   process uses the other socket, and the kernel connects the two
 *   sockets.  If a process wrote into one element in the array and
 *   read from the other one, that process would just talk to itself.

 *	build: cc sol11.11.c -o sol11.11
 */

/**	tinybc.c	* a tiny calculator that uses dc to do its work
 **			* demonstrates bidirectional pipes
 **			* input looks like number op number which
 **			  tinybc converts into number \n number \n op \n p
 **			  and passes result back to stdout
 **
 **		+-----------+                +----------+
 **	stdin  >0           |                |          |
 **	        |  tinybc   :================:   dc -   |
 **	stdout <1           |  pair of con-  |          |
 **		+-----------+  necte sockets +----------+
 **                                      
 **
 **			* program outline
 **				a. get a socketpair
 **				b. fork (get another process)
 **				c. in the dc-to-be process,
 **					 connect stdin and out one socket
 **					 then execl dc
 **				d. in the tinybc-process, no plumbing to do
 **					 just talk to human via normal i/o
 **					 and send stuff via the socket
 **				e. then close pipe and dc dies
 **			* note: this version DOES handle long lines
 **/
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include	<sys/socket.h>

#define	oops(m,x)	{ perror(m); exit(x); }

void be_dc(int);
void be_bc(int);
int bidirpipe(int[]);
void fatal(char *);

int main()
{
	int	p[2];
	int	pid;

	/* make one bidir pipe */

	if ( bidirpipe(p) == -1 )
		oops("pipe failed", 1);

	/* get a process for user interface */

	if ( (pid = fork()) == -1 )
		oops("cannot fork", 2);
	if ( pid == 0 ){			/* child is dc 	   */
		close(p[1]);			/* won't use this  */
		be_dc(p[0]);			/* it uses p[0]    */
	} else {
		close(p[0]);			/* don't need this  */
		be_bc(p[1]);			/* parent uses p[1] */
		wait(NULL);			/* wait for child   */
	}
	return 0;
}

/*
 * Make a bidirectional pipe. Use regular pipe or socketpair
 * depending on what works on your system.
 */
int bidirpipe(int p[2])
{
	return socketpair(AF_UNIX, SOCK_STREAM, PF_UNSPEC, p);
}

void be_dc(int fd)
/*
 *	set up stdin and stdout, then execl dc
 */
{
    /* setup stdin from pipein	*/
	if ( dup2(fd,0) == -1 )	/* copy read end to 0   */
		oops("dc: cannot redirect stdin",3);

    /* setup stdout to pipeout	*/
	if ( dup2(fd, 1) == -1 )	/* dupe write end to 1	*/
		oops("dc: cannot redirect stdout",4);

	close(fd);
    /* now execl dc with the - option */
	execlp("dc", "dc", "-", NULL );
	oops("Cannot run dc", 5);
}

void be_bc(int fd)
/*
 *	read from stdin and convert into to RPN, send down pipe
 *	then read from other pipe and print to user
 *	Uses fdopen() to convert a file descriptor to a stream
 */
{
	int	num1, num2;
	char	operation[BUFSIZ], message[BUFSIZ], *fgets();
	char	dc_cmd[BUFSIZ], dc_reply[BUFSIZ];
	FILE	*fpin;

	fpin = fdopen(fd, "r");
	if ( fpin == NULL )
		fatal("Cannot fdopen pipe");
	/* main loop */
	while ( printf("tinybc: "), fgets(message,BUFSIZ,stdin) != NULL ){

		/* parse input */
		if ( sscanf(message,"%d%[-+*/^]%d",&num1,operation,&num2)!=3){
			printf("syntax error\n");
			continue;
		}
		sprintf(dc_cmd,"%d\n%d\n%c\np\n", num1, num2, *operation);
		if ( write( fd, dc_cmd, strlen(dc_cmd)) == -1 )
			fatal("Error writing");

		printf("%d %c %d = ", num1, *operation, num2 );
		while ( fgets(dc_reply, BUFSIZ, fpin) != NULL ){
			fputs(dc_reply, stdout);
			if ( !strstr(dc_reply,"\\\n") )
				break;
		}
	}
	close(fd);
}

void fatal( char *mess )
{
	fprintf(stderr, "Error: %s\n", mess);
	exit(1);
}
