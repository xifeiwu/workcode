/** sol03.17.c
 ** ------------------------------------------------------------
	sol03.17.c is a
	solution to the question.  This solution does not give
	the user 20 seconds.  Instead it runs a shell so the user
	can set the mode without having to open a second terminal window.

	The result is that the file is still readable via the file 
	descriptor.  Read permission, therefore, really means the
	permission to open a file for reading.  Once a file is open,
	the permission bits are not checked.

 ** ------------------------------------------------------------
 **
 **
 *
 * Shows that read permission affects open, not read
 * opens a file, reads a few bytes, then runs a shell so
 * the user can change the mode of the file, then reads some more.  
 *
 *	build: cc sol03.17.c -o sol03.17
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

main(int ac, char *av[])
{
	int	fd;
	char	buf[100];
	int	n;

	if ( ac == 1 ){
		fprintf(stderr,"usage: sol3.17 filename\n");
		exit(1);
	}
	/*
	 * open the file
	 */
	fd = open(av[1], O_RDONLY);
	if ( fd == -1 ){
		perror(av[1]);
		exit(1);
	}
	/*
	 * read a few bytes
	 */

	n = read(fd, buf, 10);
	buf[n] = '\0';
	printf("Read %d bytes from %s: \"%s\"\n", n, av[1], buf);
	
	printf("Now, type  chmod 000 %s ; exit \n", av[1]);
	system("sh");

	/*
	 * then try to read again
	 */
	n = read(fd, buf, 10);
	buf[n] = '\0';
	printf("Read %d more bytes from %s: \"%s\"\n", n, av[1], buf);
	close(fd);
}
