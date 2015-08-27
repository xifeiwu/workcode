/** sol15.6.c
 ** ------------------------------------------------------------
	A version of selectdemo.c recoded using poll()
	instead of select() is
	sol15.6.c.
	The program does the same thing the select-based
	version did.  This exercise allows the reader
	to learn about and compare the two techniques.

 ** ------------------------------------------------------------
 **
 *   A recoding of 'selectdemo.c' using poll.
 */

/* selectdemo.c : watch for input on two devices AND timeout
 *         usage: selectdemo dev1 dev2 timeout
 *        action: reports on input from each file, and 
 *                reports timeouts
 *
 * build: cc sol15.6.c -o sol15.6
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/poll.h>

#define oops(m,x) { perror(m); exit(x); }

void showdata(char *fname, int fd);

int main(int ac, char *av[])
{
	int		fd1, fd2;	/* the fds to watch	*/
	int		timeout;	/* how long to wait	*/
	struct pollfd	items[2];	/* items to watch	*/
	int    retval;		   	/* return from poll	*/

	if ( ac != 4 ){
		fprintf(stderr,"usage: %s file file timeout\n", *av);
		exit(1);
	}

	/** open files **/
	if ( (fd1 = open(av[1],O_RDONLY)) == -1 )
		oops(av[1], 2);
	if ( (fd2 = open(av[2],O_RDONLY)) == -1 )
		oops(av[2], 3);

	/** set up one struct per open file **/
	items[0].fd = fd1;
	items[0].events = POLLIN;
	items[1].fd = fd2;
	items[1].events = POLLIN;
	timeout = atoi(av[3]) * 1000;	/* Linux says millisecs */

	while(1) {

		/** wait for input **/
		retval = poll(items, 2, timeout);
		if( retval == -1 )
			oops("poll",4);

		if ( retval > 0 ){
			/** check bits for each fd **/
			if ( items[0].revents & POLLIN )
				showdata(av[1], fd1);
			if ( items[1].revents & POLLIN )
				showdata(av[2], fd2);
		}
		else
			printf("no input after %d seconds\n", atoi(av[3]));
	}
	return 0;
}
void showdata(char *fname, int fd)
{
	char buf[BUFSIZ];
	int  n;

	printf("%s (%d): ", fname, fd);
	fflush(stdout);
	n = read(fd, buf, BUFSIZ);
	if ( n == -1 )
		oops(fname,5);
	write(1, buf, n);
	write(1, "\n", 1);
}
