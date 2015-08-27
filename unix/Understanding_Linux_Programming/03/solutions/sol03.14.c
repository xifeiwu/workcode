/** sol03.14.c
 ** ------------------------------------------------------------
	sol03.14.c is a
	revised version of cp1.c that can copy an entire directory
	file by file to a second directory.  It also supports some
	of the features required by earlier exercises.

 ** ------------------------------------------------------------
 **
 ** 
 *     A version of cp1.c that works if src or dest name directories
 *     (but not if src is a directory and dest is not)
 *
 *     usage: cp1 src dest
 *     If dest names a directory, then copy src to dest/src
 *     If src  names a directory, then copy all files in src to dest
 *        If src is a directory and dest is NOT a directory, quit
 *     Note: if src has a leading path, then only use last component
 *     
 *	build: cc sol03.14.c -o sol03.14
 */
#include        <stdio.h>
#include	<stdlib.h>
#include        <unistd.h>
#include        <fcntl.h>
#include	<sys/stat.h>
#include	<string.h>
#include	<dirent.h>

#define BUFFERSIZE      4096
/*
 * note: the real copy takes the mode of the copy from
 *       the mode of the source.  
 */
#define COPYMODE        0644

void oops(char *, char *);
void *emalloc(size_t);

int
main(int ac, char *av[])
{
        if ( ac != 3 ){
                fprintf( stderr, "usage: %s source destination\n", *av);
                exit(1);
        }

	/*
	 * if source is a dir, then the dest has to be, too
	 */

	if ( isadir(av[1]) ){
		if ( isadir(av[2]) )
			copydir(av[1], av[2]);
		else {
			fprintf(stderr,"cp1: %s is not a directory\n", av[2]);
			exit(1);
		}
	}
	/*
	 * if source is not a dir, then the dest can be anything
	 */
	else
		do_copy( av[1], av[2] );
	return 0;
}

/*
 * copydir()
 *   loops through all files in srcdir, copying each to destdir
 *   uses do_copy but builds the paths here
 *   Note: this function skips subdirectories of srcdir
 */
copydir(char *srcdir, char *destdir)
{
	char	       *srcpath, *destpath;
	DIR	       *dir_ptr;
	struct dirent  *direntp;

	srcpath  = (char *) emalloc(strlen(srcdir)+1+MAXNAMLEN+1);
	destpath = (char *) emalloc(strlen(destdir)+1+MAXNAMLEN+1);
	if ( (dir_ptr = opendir(srcdir)) == NULL )
		oops("Cannot open directory", srcdir);

	/*
	 * loop through all items in src dir
	 *  Do not copy directories, and report that so user
	 *  realizes not all the items are copied.
	 */
	while( ( direntp = readdir(dir_ptr)) != NULL )
	{
		sprintf(srcpath,"%s/%s", srcdir, direntp->d_name);
		if ( isadir(srcpath) ){
			if ( strcmp(direntp->d_name,".") != 0 &&
			     strcmp(direntp->d_name,"..") != 0 )
				printf("skipping directory %s\n", srcpath);
			continue;
		}
		sprintf(destpath, "%s/%s", destdir, direntp->d_name);
		do_copy( srcpath, destpath );
	}
	closedir(dir_ptr);
	free(srcpath);
	free(destpath);
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
	rv = emalloc(strlen(srcfilename) + strlen(dest) + 2);
	sprintf(rv, "%s/%s", dest, srcfilename);

	return rv;
}

void *
emalloc(size_t n)
{
	void *rv = malloc(n);
	if ( rv == NULL )
		oops("Out of memory","");
	return rv;
}
/*
 * boolean: tells if arg names a directory
 */
isadir(char *str)
{
	struct stat info;

	return ( stat(str,&info) != -1 && S_ISDIR(info.st_mode) );
}
