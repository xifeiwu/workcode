/** sol03.21.c
 ** ------------------------------------------------------------
	sol03.21.c is a version
	of cp that does two things: (1) It resets the access and
	modification times of the original file, and (2) It sets
	the access and modification time of the copy to those of
	the original.  Suitable for doing backup copies.

 ** ------------------------------------------------------------
 *
 *     version of cp that copies st_mtime and st_atime from the 
 *     original to the copy.
 *
 *     usage: cp1 src dest
 *
 *	build: cc sol03.21.c -o sol03.21
 */
#include        <stdio.h>
#include        <unistd.h>
#include        <fcntl.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<utime.h>

#define BUFFERSIZE      4096
/*
 * We really ought to copy the permission bits, too.
 * That it easy to add.
 */
#define COPYMODE        0644

void oops(char *, char *);

main(int ac, char *av[])
{
        int     in_fd, out_fd, n_chars;
	struct  stat  fileinfo;
        char    buf[BUFFERSIZE];
						/* check args 	*/
        if ( ac != 3 ){
                fprintf( stderr, "usage: %s source destination\n", *av);
                exit(1);
        }

	/*
	 * first, get modtime and access time info *before*
	 * we read the file.
	 */

	if ( stat(av[1], &fileinfo) == -1 )
		oops("Cannot stat", av[1]);
	/*
	 * open files
	 */
        if ( (in_fd=open(av[1], O_RDONLY)) == -1 )
                oops("Cannot open ", av[1]);

        if ( (out_fd=creat( av[2], COPYMODE)) == -1 )
                oops( "Cannot creat", av[2]);

	/*
	 * copy data
	 */
        while ( (n_chars = read(in_fd , buf, BUFFERSIZE)) > 0 )
                if ( write( out_fd, buf, n_chars ) != n_chars )
                        oops("Write error to ", av[2]);
	if ( n_chars == -1 )
			oops("Read error from ", av[1]);

	/*
	 * close files
	 */
        if ( close(in_fd) == -1 || close(out_fd) == -1 )
                oops("Error closing files","");

	/*
	 * reset atime and mtime on both files
	 */
	set_times(&fileinfo, av[1], av[2]);
	return 0;
}

void oops(char *s1, char *s2)
{
        fprintf(stderr,"Error: %s ", s1);
        perror(s2);
        exit(1);
}

/*
 * copy the mtime and atime from file f1 to file f2
 * method: fill in the two members in a utimbuf
 * and then call utime() for each file
 */
set_times(struct stat *finfop, char *f1, char *f2)
{
	struct utimbuf timeinfo;
	char *ctime(time_t);

	timeinfo.actime   = finfop->st_atime;
	timeinfo.modtime  = finfop->st_mtime;
	if ( utime(f1, &timeinfo) == -1 )
		oops("Cannot change times", f1);
	if ( utime(f2, &timeinfo) == -1 )
		oops("Cannot change times", f2);
}
