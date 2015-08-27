/** sol05.26.c 
 ** ------------------------------------------------------------
	A solution to this problem is a slightly modified version of 
	the cp1.c program from Chapter 2.  This program is
	sol05.26.c.
	It takes a third argument that tells it to turn O_SYNC on or
	off.  The program accepts a fourth argument that specifies the
	buffersize.  You need to test this on your system to see 
	what difference buffering makes.  On my system with a 2meg file
	and a 16byte buffer, the difference in execution time 
	between sync and nosync was large.

 ** ------------------------------------------------------------
 **
 **
 *     Buffering test version of cp1.
 *
 *     usage: cp1 src dest {sync|nosync} [buffersize]
 *
	build: cc sol05.26.c -o sol05.26
 */

#include        <stdio.h>
#include        <unistd.h>
#include        <fcntl.h>

#define COPYMODE        0644

void oops(char *, char *);

main(int ac, char *av[])
{
        int     in_fd, out_fd, n_chars;
        char    *buf;
	int	bufsize = 4096;
	int	openmode = O_CREAT|O_TRUNC|O_WRONLY;

	/*
	 * check and handle args
	 */

        if ( ac < 4 ){
                fprintf(stderr,
			"usage: %s source dest {sync|nosync} [bufsiz]\n",
		       	*av);
                exit(1);
        }

	/*
	 * set up buffer
	 */

	if ( ac > 5 )
		bufsize = atoi(av[4]);

	if ( (buf = (char *)malloc(bufsize)) == NULL ){
		perror("out of memory");
		exit(2);
	}

	/*
	 * set up buffering mode for open
	 */

	if ( strcmp(av[3], "sync") == 0 )
		openmode |= O_SYNC;

        if ( (in_fd=open(av[1], O_RDONLY)) == -1 )
                oops("Cannot open ", av[1]);

        if ( (out_fd=open( av[2], openmode, COPYMODE)) == -1 )
                oops( "Cannot create", av[2]);
	

	/*
	 * do the copy
	 */

        while ( (n_chars = read(in_fd , buf, bufsize)) > 0 )
                if ( write( out_fd, buf, n_chars ) != n_chars )
                        oops("Write error to ", av[2]);
	if ( n_chars == -1 )
			oops("Read error from ", av[1]);

						/* close files	*/

        if ( close(in_fd) == -1 || close(out_fd) == -1 )
                oops("Error closing files","");
}

void oops(char *s1, char *s2)
{
        fprintf(stderr,"Error: %s ", s1);
        perror(s2);
        exit(1);
}

