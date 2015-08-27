/** sol15.13.c
 ** ------------------------------------------------------------
	A version of cp that puts a write lock on the output file.
	If the output file is a device, that lock ensures exclusive
	access.  The program is
	sol15.13.c.

 ** ------------------------------------------------------------
 **
 *   A version of cp that uses file locking to lock the output file.
 *
 *   usage: sol15.13 source dest
 *
 *   build: cc sol15.13.c -o sol15.13
 *
 *   use #define SLOW to slow down the copy to see how the locking
 *   works when you try to run multiple instances
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	<sys/file.h>
#include	<fcntl.h>

#define oops(s,x)  { perror(s); exit(x); }

#define	SLOW	

void lock_operation(int fd, int op);

int main(int ac, char *av[])
{
	int	input = 0, output;
	int	n;
	char	buf[BUFSIZ];

	if ( ac < 2 || ac > 3 ){
		fprintf(stderr,"usage: %s [input] outfile\n", *av);
		exit(2);
	}

	if ( ac == 3 ){
		if( (input = open(av[1], O_RDONLY)) == -1 )
			oops(av[1],1);
	}

	if ( (output = open(av[ac-1],O_WRONLY|O_CREAT,0644)) == -1 )
		oops(av[ac-1],1);

	printf("waiting for lock..\n");
	lock_operation( output, F_WRLCK );
	printf("Got lock!..Copying\n");

	while( (n = read(input, buf, BUFSIZ)) > 0 ){
#ifdef SLOW
		putchar('.'); fflush(stdout);
		sleep(1);
#endif
		if ( write(output, buf, n) == -1 )
			oops(av[ac-1],3);
	}

	printf("releasing lock\n");
	lock_operation(output, F_UNLCK);
	close(input);
	close(output);
	return 0;
}

void lock_operation(int fd, int op)
{
	struct flock lock;

	lock.l_whence = SEEK_SET;
	lock.l_start = lock.l_len = 0;
	lock.l_pid = getpid();
	lock.l_type = op;

	if ( fcntl(fd, F_SETLKW, &lock) == -1 )
		oops("lock operation", 6);
}
