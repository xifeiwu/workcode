/** sol15.8serv.c
 ** ------------------------------------------------------------
	A C version of the FIFO-base server is
	sol15.8serv.c,
	and a C version of the FIFO-based client is
	sol15.8clnt.c.

 ** ------------------------------------------------------------
 **
 *    A time/date server that uses a FIFO
 *
 *    usage: sol15.8serv fifoname
 *
 *    build: cc sol15.8serv.c -o sol15.8serv
 *
 *    Note: The only safe way to write a fifo-based server is
 *    to close the fifo, then remove the fifo, then create it
 *    again.  The reason you have to go to all this trouble is
 *    that the client may be slow to close the reading end.
 *    If the client still has the reading end open when the
 *    server loops back to try to open the fifo for writing,
 *    open will succeed at once!  The server will then talk to
 *    the same client again, who may be surprised to see more
 *    data showing up.
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<time.h>
#include	<fcntl.h>
#include	<errno.h>
#include	<signal.h>

#define oops(s,x)  { perror(s); exit(x); }
#define	BUFLEN		128
#define	FIFOMODE 	0644

char	*fifoname;

int main(int ac, char *av[])
{
	int	fd;		/* the fifo 	*/
	time_t	now;		/* the time	*/
	char	buf[BUFLEN];	/* the string	*/
	void	rm_fifo(int);	/* handler	*/

	if ( ac != 2 ){
		fprintf(stderr,"usage: %s fifoname\n", *av);
		exit(1);
	}

	fifoname = av[1];
	signal(SIGINT, rm_fifo);
	signal(SIGQUIT, rm_fifo);
	signal(SIGHUP, rm_fifo);

	/*
	 * the main loop consists of waiting for open() 
	 * to succeed, writing stuff into it, then
	 * closing it and looping back.
	 */
	while(1)
	{
		/* remove any existing instance */
		if ( unlink(fifoname) == -1 && errno != ENOENT )
			oops(fifoname,1);

		/* make a new instance of the fifo */
		if ( mkfifo(fifoname, FIFOMODE) == -1 )
			oops(fifoname,2);
		fifoname = fifoname;

		if ( ( fd = open(fifoname,O_WRONLY) ) == -1 )
			perror("Cannot open fifo");
		else
		{
			printf("Someone opened the fifo for reading\n");
			time(&now);
			strcpy(buf,ctime(&now));
			if ( write(fd, buf, strlen(buf) ) == -1 )
				perror("writing to fifo");
			close(fd);
		}
	}
	return 0;
}

/*
 * on signals, remove the fifo and report the signal using exit()
 */
void rm_fifo(int s)
{
	unlink(fifoname);
	exit(s);
}
