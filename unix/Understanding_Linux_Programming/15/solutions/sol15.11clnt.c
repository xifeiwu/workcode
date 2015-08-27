/** sol15.11clnt.c
 ** ------------------------------------------------------------
	This solution shows how to use file locks
	to implement read and write locks on a shared
	memory segment.
	This solution is the answer to exercise 15.9
	with the semaphores replaced by read and write
	locks on a disk file.  The file locking code
	came from file_tc.c and file_ts.c.
	 
	The server is
	sol15.11serv.c,
	and the matching client is
	sol15.11clnt.c.

 ** ------------------------------------------------------------
 **
 *    Client for SHARED MEMORY  version of time/date service.
 *    MULTIPLE-SERVER-SAFE
 *    Uses FILE LOCKS for locks
 *
 *    Client for the shared memory time/date service that allows
 *    multiple servers to write into memory safely.  This sytem
 *    uses file locks rather than semaphores.  Just to see how
 *    one can mix and match locking mechanisms with transfer 
 *    mechanisms.
 *
 *    build: cc sol15.11clnt.c -o sol15.11clnt
 *
 *    This client is a blend of shm_tc2.c and file_tc.c
 */

#include	<stdio.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<sys/shm.h>
#include	<time.h>
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/file.h>
#include	<fcntl.h>

#define	TIME_MEM_KEY	99		/* kind of like a port number */
#define	SEG_SIZE	((size_t)100)		/* size of segment	*/
#define oops(m,x)  { perror(m); exit(x); }

void lock_operation(int fd, int op);

int main(int ac, char *av[])
{
	int	seg_id;
	char	*mem_ptr, *ctime();
	int	fd;

	/** check arglist and open file **/

	if ( ac == 1 ){
		fprintf(stderr,"usage: %s filename\n", *av);
		exit(1);
	}
	if ( (fd=open(av[1],O_RDONLY)) == -1 )
		oops(av[1],2);

	/* create a shared memory segment */

	seg_id = shmget( TIME_MEM_KEY, SEG_SIZE, 0777 );
	if ( seg_id == -1 )
		oops("shmget",1);

	/* attach to it and get a pointer to where it attaches */

	mem_ptr = shmat( seg_id, NULL, 0 );
	if ( mem_ptr == ( void *) -1 )
		oops("shmat",2);

	lock_operation(fd, F_RDLCK);
	printf("The time, direct from memory: ..%s", mem_ptr);
	lock_operation(fd, F_UNLCK);

	shmdt( mem_ptr );		/* detach, but not needed here */
	return 0;
}

void lock_operation(int fd, int op)
{
	struct flock lock;

	lock.l_whence = SEEK_SET;
	lock.l_start = lock.l_len = 0;
	lock.l_pid = getpid();
	lock.l_type = op;

	if ( fcntl(fd, F_SETLKW, &lock) == -1 )
		oops("lock operation", 6);
}
