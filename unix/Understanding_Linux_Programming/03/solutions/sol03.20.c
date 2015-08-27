/** sol03.20.c
 ** ------------------------------------------------------------
	sol03.20.c is a version
	of chown.  This program shows how to use getpwnam() to
	convert a username to a user id number.  It also shows how
	to use the chown() system call.

 ** ------------------------------------------------------------
 *
 * chown username file1 file2 ...
 *
 * Uses getpwnam() to convert the name into a uid and then
 * simply loops through file list calling chown(2) to do the
 * work.
 *
 * Returns 0 if ok, 1, and 2 on usage errors, 3 on chown failure
 *
 *	build: cc sol03.20.c -o sol03.20
 */

#include <pwd.h>
#include <sys/types.h>
#include <stdio.h>

main(int ac, char *av[])
{
	uid_t	       userid;
	struct passwd *infop;
	int	       i;
	int	       rv = 0;

	if ( ac < 3 ){
		fprintf(stderr,"chown: too few arguments\n");
		exit(1);
	}

	/*
	 * look up user id number for name
	 */
	infop = getpwnam(av[1]);
	if ( infop == NULL ){
		fprintf(stderr,"chown: %s: invalid user\n", av[1]);
		exit(2);
	}
	userid = infop->pw_uid;
	
	/*
	 * then just process all the arguments
	 */

	for( i = 2 ; i<ac ; i++ )
		if ( chown( av[i], userid, -1) == -1 ){
			fprintf(stderr,"chown: ");
			perror(av[i]);
			rv = 3;
		}
	return rv;
}
