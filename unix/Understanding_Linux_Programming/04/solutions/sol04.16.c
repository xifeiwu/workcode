/** sol04.16.c
 ** ------------------------------------------------------------
	This version of mv accepts a directory name as the 
	second argument.  In that case the source item is
	moved into that directory.  This code is similar to
	the solution to Exercise 3.13.
	sol04.16.c.

 ** ------------------------------------------------------------
 **
 *  A version of mv that accepts a directory name as second argument
 *
 *     usage: mv src dest
 *     if dest names a directory, then rename src to dest/src
 *     Note: if src has a leading path, then only use last component
 *     Also: if dest is on a different file system then copy the file
 */

#include        <stdio.h>
#include	<stdlib.h>
#include        <unistd.h>
#include	<sys/stat.h>
#include	<string.h>
#include	<errno.h>


void oops(char *, char *);

int
main(int ac, char *av[])
{
        if ( ac != 3 ){
                fprintf( stderr, "usage: %s source destination\n", *av);
                exit(1);
        }
	return do_mv(av[1], av[2]);
}

/*
 * Renames a file from src to dest
 *
 * If dest is a directory, then do_mv() moves to an item
 * in dest with the name taken from the filename for src
 */
int
do_mv(char *src, char *dest)
{
	char	*destfilename;
	char	*make_destfilename(char*,char*);
	int	rv;

	destfilename = make_destfilename(src, dest);
	rv = rename(src,destfilename);
	/*
	 * If target is different file system, try copy and remove
	 */
	if ( rv == -1 && errno == EXDEV )
		rv = copy_and_unlink(src,destfilename);
	if ( rv == -1 ){
		fprintf(stderr,"mv: cannot mv %s -> %s: ", src, dest);
		perror("");
	}
	return rv;

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

/*
 * copy_and_unlink(old,new)
 *   purpose: copy a file and unlink the original
 *    reason: links cannot be moved to different file systems
 *    action: makes a copy using standard i/o lib
 *   returns: 0 for OK or -1 for file errors
 *   details: tedious.  If unlink fails, then unlink target
 *     NEEDS: needs to copy permission bits to new file
 */

copy_and_unlink(char *old, char *new)
{
	FILE	*fpin, *fpout;
	int	c;
	int	rv = 0;

	/** open files **/
	if ( (fpin = fopen(old,"r")) == NULL )
		return -1;
	if ( (fpout = fopen(new, "w")) == NULL ){
		fclose(fpin);
		return -1;
	}

	/** do copy **/
	while( rv == 0 && (c = getc(fpin)) != EOF )
		if ( putc(c, fpout) == EOF )
			rv = -1;

	/** close files: if error, abandon project **/
	if ( fclose(fpin) == EOF || fclose(fpout) == EOF ){
		rv = -1;
		unlink(new);
	}
	/** otherwise try to unlink original **/
	else if ( unlink(old) == -1 ){
		rv = -1;
		unlink(new);
	}

	/** whew! **/
	return rv;
}
