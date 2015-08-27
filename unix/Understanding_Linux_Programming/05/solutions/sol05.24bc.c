/** sol05.24bc.c
 ** ------------------------------------------------------------

	(b), (c)
	A program that uses the link system call for locking is:
	sol05.24bc.c.
	This program appends a line of data to a file.  
	The solution locks the file, waits 10 seconds, then
	appends a line of data to the file.  The delay is included
	so users can see that attempts to run multiple instances of
	the program produce the correct blocking behavior.

 ** ------------------------------------------------------------
 **
 **
 *  Demonstration of using link() to lock a file.
 *  
 *   Usage: appendline filename "line to add"
 *  Action: makes a link to filename called filename.LCK
 *          adds the line then unlinks the lock
 *          If the link fails, waits a second then tries again
 *          After 30 seconds, gives up with exit code 2

	build: cc sol05.24bc.c -o sol05.24bc
 */

#include	<stdio.h>
#include	<unistd.h>
#include	<errno.h>

#define	TOOLONG		30
#define	LOCKSUFFIX	".LCK"

char *makelockname(char *);		/* make name of lockfile */

main(int ac, char *av[])
{
	int	rv ;
	FILE	*fp;
	char	*lockname;

	if ( ac != 3 ){
		fprintf(stderr, "usage: appendline filename \"data\"\n");
		exit(1);
	}

	/* lock the file */
	lockname = makelockname(av[1]);
	if ( (rv = lockfile(av[1], lockname)) != 0 )
		return rv;

	/* open the file so we can add to it */
	fp = fopen(av[1], "a");
	if ( fp == NULL ){
		perror(av[1]);
		unlockfile(av[1]);
		exit(4);
	}
	/* just for fun, hold the lock for a while */
	/* try running other instances now	   */
	sleep(10);

	/* add data to it */
	fprintf(fp, "%s\n", av[2]);
	fclose(fp);

	rv = unlockfile(lockname);

	return rv;
}

/*
 * creates a lock called f.LCK
 * returns 0 for OK, 2 for cannot get lock, 3 for other error
 */
lockfile(char *f, char *l)
{
	int	tries = 0;

	while( tries < TOOLONG ){
		if ( link(f, l) == 0 )		/* worked? all done	*/
			return 0;
		if ( errno != EEXIST ){		/* other error: get out	*/
			perror(l);
			return 3 ;
		}
		tries++;			/* count tries		*/
		sleep(1);			/* and wait a bit	*/
	}
	fprintf(stderr, "Cannot set lock\n");
	return 2;
}

/*
 * removes the lock
 * returns 0 for OK, 5 for error
 */
unlockfile(char *l)
{
	int	rv = 0 ;

	if ( unlink(l) == -1 ){
		perror("Cannot unlock file");
		rv = 5;
	}
	return rv;
}

/*
 * make a name for the lock by adding ".LCK"
 */
char *
makelockname(char *f)
{
	char	*rv = (char *)malloc(strlen(f)+strlen(LOCKSUFFIX)+1);

	if ( rv == NULL ){
		perror("out of memory");
		exit(9);
	}
	sprintf(rv, "%s%s", f, LOCKSUFFIX);
	return rv;
}
