/** sol10.16.c
 ** ------------------------------------------------------------
	A short program that shows how to determine if a file
	descriptor leads to a regular file (as opposed to a pipe,
	a device file, fifo, socket, etc) is
	sol10.16.c.

 ** ------------------------------------------------------------
 **
 **
 *   isaregfile.c
 *
 *   purpose: determine if a fd leads to a regular file
 *    method: use fstat and check type encoded in mode
 *    reason: useful to know how to tell ttys from regfiles
 *            by looking at the fd
 *
 *      test: try isaregfile < /dev/tty
 *                isaregfile 2>/dev/null
 *                isaregfile >foo
 *                who | isaregfile
 *
 *     notes: not an exciting program, but it can be useful

 *	build: cc sol10.16.c -o sol10.16
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<sys/stat.h>

int isaregfile(int);

int main()
{
	if ( isaregfile(1) )
		printf("stdout is a regular file\n");
	else
		printf("stdout is not a regular file\n");

	if ( isaregfile(0) )
		printf("stdin is a regular file\n");
	else
		printf("stdin is not a regular file\n");

	if ( isaregfile(2) )
		printf("stderr is a regular file\n");
	else
		printf("stderr is not a regular file\n");
	return 0;
}

int isaregfile(int fd)
{
	struct stat info;

	if ( fstat(fd, &info) == -1 ){
		fprintf(stderr,"cannot stat fd %d\n", fd);
		exit(1);
	}

	return (S_ISREG(info.st_mode));
}
