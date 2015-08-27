/** sol05.25.c
 ** ------------------------------------------------------------

	A version of lock_passwd that claims stale locks is
	sol05.25.c
	This solution uses a lock on the lockfile and has to
	touch the file before releasing the lock.

	This problem asks students to think about race conditions
	and resource locking carefully.  The solution sounds sensible
	but cannot be coded without introducing a second lock.  The
	use of that second lock is not trivial; the problem requires
	thinking about the extent of a critical section of code.

	This problem also raises the question of how a program that
	holds a lock can keep the lock `warm.'  Later chapters discuss
	techniques for scheduling periodic actions, and the chapter on
	the license server presents a method for recovering abandoned
	resources.  Here, this problem can introduce ideas for future
	study.

 ** ------------------------------------------------------------
 **
 **
 *  lock_passwd() that considers checks the modification time
 *                for the lockfile.   If the lock is more than
 *                a certain number of seconds old, then unlink
 *                the lock and try to make it again.
 *
 *                Note: It is not safe to unlink the lock directy
 *                because another process might try to do the
 *                same thing.  To prevent that race condition,
 *                the lock has to be locked so only one process
 *                can examine, unlink, and claim it.
 *
 *		  returns 0 on success, -1 on failure

	build: cc -c sol05.25.c 
 */

#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <utime.h>

lock_passwd(int stale)
{
	while( link("/etc/passwd", "/etc/passwd.LCK") == -1 )
	{
		if ( errno != EEXIST ){
			perror("Cannot make link");
			return -1;
		}
		if ( claim_lock_if_stale(stale) == 0 )
			break;
		sleep(1);
	}
	return 0;
}

/*
 * claim_lock_if_stale(int duration)
 *
 * See if the lock has been abandoned.  If so then delete it,
 * then create it, then change mod time so it is now `warm'.
 * 
 * To prevent two processes from doing this at the same time,
 * the lock has to be locked.
 *
 * returns 0 if deleted the lock
 * returns -1 otherwise
 *
 * Note: We do not really have to create the lock again, simply
 *       warming it up is ok.
 */

claim_lock_if_stale(int duration)
{
	int	rv = -1;

	/* 
	 * first, try to lock the lock
	 */
	if( link("passwd.LCK", "passwd.LCK.LCK") == -1 )
		return -1;

	/*
	 *  If the lock has gone stale,
	 *	delete and recreate the lock and set the 
	 *	mod and access time to now.
	 */
	if ( gone_stale("passwd.LCK", duration) )
	{
		/* try to delete the lock */
		unlink("passwd.LCK");
		if ( link("passwd", "passwd.LCK") == 0 )
			if ( utime("passwd.LCK", NULL ) == 0 )
				rv = 0;

	}
	/* 
	 * finally, unlock the lock
	 */
	unlink("passwd.LCK.LCK");
	return rv;
}
 
/*
 * boolean: true if file does not exist or has not been touched for duration
 */

gone_stale(char *filename, int duration )
{
	struct stat info;
	time_t      now;

	time(&now);

	return (stat(filename,&info) == -1 || (now-info.st_mtime)>duration);
}

