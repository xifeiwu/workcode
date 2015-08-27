/** sol15.11serv.c
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
 *    Server for SHARED MEMORY version of time/date service.
 *    MULTIPLE-SERVER-SAFE
 *    Uses FILE LOCKS for locks
 *
 *   A version of shm_ts2.c that allows multiple servers to write
 *   to shared memory safely.  The locking mechanism has been
 *   replaced by file locks.  A lock is needed; the name is a
 *   required command line argument.
 *
 *    build: cc sol15.11serv.c -o sol15.11serv
 */
/*
 * program uses shared memory with key 99
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	<sys/shm.h>
#include	<time.h>
#include	<sys/types.h>
#include	<sys/file.h>
#include	<fcntl.h>
#include	<signal.h>
#include	<errno.h>

#define	TIME_MEM_KEY	99			/* like a filename      */
#define	SEG_SIZE	((size_t)100)		/* size of segment	*/
#define oops(m,x)  { perror(m); exit(x); }

int	seg_id;					/* global for cleanup()	*/

int mypid;

void narrate(char *s);
void cleanup(int n);
void lock_operation(int fd, int op);

int main(int ac, char *av[])
{
	char	*mem_ptr, *ctime();
	int	fd;

	mypid = getpid();

	/**  make sure the file is there so we can lock it  **/

	if ( ac == 1 ){
		fprintf(stderr,"usage: %s filename\n", *av);
		exit(1);
	}
	if ( (fd=open(av[1],O_WRONLY|O_CREAT,0644)) == -1 )
		perror(av[1]);

	/** create a shared memory segment **/

	seg_id = shmget( TIME_MEM_KEY, SEG_SIZE, IPC_CREAT|0777 );
	if ( seg_id == -1 )
		oops("shmget", 1);

	/** attach to it and get a pointer to where it attaches **/

	mem_ptr = shmat( seg_id, NULL, 0 );
	if ( mem_ptr == ( void *) -1 )
		oops("shmat", 2);

	/** now run the main loop **/

	while(1)
	{
		time_t	now;
				narrate("waiting for memory");
		lock_operation(fd, F_WRLCK);

				narrate("HAVE (updating) memory");
		time( &now );			/* get the time	*/
		strcpy(mem_ptr, ctime(&now));	/* write to mem */
		sleep(5);

				narrate("FREEING memory");
		lock_operation(fd, F_UNLCK);
		sleep(1);			/* wait a sec   */
	}
	return 0;
}

void narrate(char *s)
{
	printf("\tshm_ts2 [%d] %s\n", mypid, s);
}

void cleanup(int n)
{
	shmctl( seg_id, IPC_RMID, NULL );	/* rm shrd mem	*/
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
