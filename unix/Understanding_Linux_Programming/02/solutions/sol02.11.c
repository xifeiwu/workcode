/** sol02.11.c 
 ** ------------------------------------------------------------
	Several versions of cp report 
	cp: `file1' and `file1' are the same file
	when asked to copy a file onto itself.  A modified version
	is: sol02.11.c.
	Note that a much better way to do this is to check the inodes
	to see if the two files actually are the same file.  Inodes
	are described in a later chapter.

 ** ------------------------------------------------------------
 *
 *     some versions of cp say  "cp: `file1' and `file1' are the same file."
 *     If the two files are the same.
 *     This modification to cp1.c adds that feature by comparing filenames
 *     only.  
 *
 *	build: cc sol02.11.c -o sol02.11
 */
/*
 *     version 1 of cp - uses read and write with tunable buffer size
 *
 *     usage: cp1 src dest
 *
 */

#include        <stdio.h>
#include        <unistd.h>
#include        <fcntl.h>

#define BUFFERSIZE      4096
#define COPYMODE        0644

void oops(char *, char *);

main(int ac, char *av[])
{
        int     in_fd, out_fd, n_chars;
        char    buf[BUFFERSIZE];
						/* check args 	*/
        if ( ac != 3 ){
                fprintf( stderr, "usage: %s source destination\n", *av);
                exit(1);
        }

	/*
	 * prevent user from clobbering a file with itself
	 * Note: this code should really check if av[2] is a link
	 *       to file1 (or vice versa for symlinks).  Those topics
	 *       are not covered until later, so we omit those checks
	 */

	if ( strcmp(av[1], av[2]) == 0 ){
		fprintf(stderr,"cp: `%s' and `%s' are the same file.\n",
			av[1], av[2]);
		exit(1);
	}
						/* open files	*/

        if ( (in_fd=open(av[1], O_RDONLY)) == -1 )
                oops("Cannot open ", av[1]);

        if ( (out_fd=creat( av[2], COPYMODE)) == -1 )
                oops( "Cannot creat", av[2]);
	
						/* copy files	*/

        while ( (n_chars = read(in_fd , buf, BUFFERSIZE)) > 0 )
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
