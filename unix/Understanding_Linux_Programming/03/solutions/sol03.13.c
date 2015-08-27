/** sol03.13.c 
 ** ------------------------------------------------------------
	A modified version of cp1.c is:
	sol03.13.c.

 ** ------------------------------------------------------------
 **
 ** 
 *	modified version of cp1.c
 *	This version handles the case in which the second argument is
 *	the name of a directory.  If the source file has a leading path
 *	then only the last component is used to make the name of the
 *	output file.
 */
/*
 *     usage: cp1 src dest
 *     if dest names a directory, then copy src to dest/src
 *     Note: if src has a leading path, then only use last component
 *
 *	build: cc sol03.13.c -o sol03.13
 */
#include        <stdio.h>
#include	<stdlib.h>
#include        <unistd.h>
#include        <fcntl.h>
#include	<sys/stat.h>
#include	<string.h>

#define BUFFERSIZE      4096
/*
 * note: the real copy takes the mode of the copy from
 *       the mode of the source.  
 */
#define COPYMODE        0644

void oops(char *, char *);

int
main(int ac, char *av[])
{
        if ( ac != 3 ){
                fprintf( stderr, "usage: %s source destination\n", *av);
                exit(1);
        }
	do_copy( av[1], av[2] );
	return 0;
}

/*
 * copies a file from src to dest
 * If dest is a directory, then do_copy() copies to
 * a file in dest with the name taken from the filename for
 * src
 */
do_copy(char *src, char *dest)
{
        int     in_fd, out_fd, n_chars;
        char    buf[BUFFERSIZE];
	char	*destfilename;
	char	*make_destfilename(char*,char*);

	destfilename = make_destfilename(src, dest);

	/*
	 * open files
	 */

        if ( (in_fd=open(src, O_RDONLY)) == -1 )
                oops("Cannot open ", src);

        if ( (out_fd=creat( destfilename, COPYMODE)) == -1 )
                oops( "Cannot creat", destfilename);
	
	/*
	 * copy files
	 */

        while ( (n_chars = read(in_fd , buf, BUFFERSIZE)) > 0 )
                if ( write( out_fd, buf, n_chars ) != n_chars )
                        oops("Write error to ", destfilename);
	if ( n_chars == -1 )
			oops("Read error from ", src);

	/*
	 * close files
	 */

        if ( close(in_fd) == -1 || close(out_fd) == -1 )
                oops("Error closing files","");
}

void oops(char *s1, char *s2)
{
        fprintf(stderr,"Error: %s ", s1);
        perror(s2);
        exit(1);
}

/*
 * if dest is a directory, then combine src and dest
 * (see header to this program)
 */

char *
make_destfilename(char *src, char *dest)
{
	struct stat info;
	char   *srcfilename;
	char   *rv;

	if ( stat(dest, &info) == -1 )	/* let someone else handle this */
		return dest;

	if ( ! S_ISDIR(info.st_mode) )	/* ok to copy to other types	*/
		return dest;

	/* find last component of source name */
	if ( (srcfilename = strrchr(src, '/')) != NULL )
		srcfilename++;
	else
		srcfilename = src;

	/* use that to construct target name */
	rv = malloc(strlen(srcfilename) + strlen(dest) + 2);
	if ( rv == NULL )
		oops("Out of memory","");
	sprintf(rv, "%s/%s", dest, srcfilename);

	return rv;
}
