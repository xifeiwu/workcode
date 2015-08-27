/** sol02.15.c 
 ** ------------------------------------------------------------
	A version of cp1.c that supports the -i option is:
	sol02.15.c.

 ** ------------------------------------------------------------
 *
 * cp1.c modified to support the -i option
 *
 *     usage: cp1 src dest
 *
 *	build: cc sol02.15.c -o sol02.15
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
	char	*src = NULL, *dest = NULL;
	int	i_option = 0;
						/* check args 	*/
	while( --ac ){
		if ( strcmp("-i", *++av) == 0 )
			i_option = 1;
		else if ( !src )
			src = *av;
		else if ( !dest )
			dest = *av;
		else 
			usage();
        }
	if ( !src || !dest )
		usage();
						/* open files	*/

        if ( (in_fd=open(src, O_RDONLY)) == -1 )
                oops("Cannot open ", src );

	if ( !i_option || !exists(dest) || ok_to_replace(dest) )
		if ( (out_fd=creat( dest, COPYMODE)) == -1 )
			oops("Cannot creat", dest);
	
						/* copy files	*/

        while ( (n_chars = read(in_fd , buf, BUFFERSIZE)) > 0 )
                if ( write( out_fd, buf, n_chars ) != n_chars )
                        oops("Write error to ", dest);
	if ( n_chars == -1 )
			oops("Read error from ", src);

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

usage()
{
	fprintf(stderr,"usage: cp [-i] source dest\n");
	exit(1);
}

/*
 * new functions to support the "-i" option
 *
 *   exists(char *filename)  - uses open() to see if file is there
 *                             A better choice is stat() - see Chapter 3
 *   ok_to_replace(char *fn) - ask the user 
 */

int exists(char *filename)
{
	int	fd;

	if ( fd = open(filename, O_RDONLY) )
		close(fd);
	return (fd != -1) ;
}

/*
 * ok_to_replace - prompts to stderr and reads from stdin
 */
int ok_to_replace(char *filename)
{
	char	ans[10];		/* ought to be enough for y/n */
	char	c;
	int	retval = 0;

	fprintf(stderr,"cp: Ok to replace `%s'? ", filename);
	if ( scanf("%9s", ans) == 1 ){
		if ( *ans == 'y' || *ans == 'Y' )
			retval = 1;
	}
	while( ( c = getchar() ) != EOF && c != '\n' )
		;
	return retval;
}
