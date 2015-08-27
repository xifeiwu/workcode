/** sol15.9clnt.c
 ** ------------------------------------------------------------
	(a)
	You cannot run two versions of shm_ts2.c at once
	because the call to semget() uses the O_EXCL flag
	to prevent another process from creating the
	semaphore set once one exists.  I tried it.

	(b)
	On the other hand, if the O_EXCL flag were not set,
	a second, third or more server could be started up,
	and all would be programmed to write data to the
	memory segment.  The problem with the design in the
	original version is that the server does not check
	to see if other servers are writing to the segment.

	The servers want exclusive access to the memory
	segment.  Therefore, they need a mutex object.
	A semaphore can be used to creat a mutex; when 
	the value is 1, the mutex is unlocked and when
	the value is 0, the mutex is locked.
	A server locks the mutex, the server has to
	tell readers to wait.  We keep the semaphore
	for the number of writers in addition to the
	number of readers and the mutex semaphore.

	A solution to the many-server version is 
	sol15.9serv.c,
	and the matching client is
	sol15.9clnt.c.

 ** ------------------------------------------------------------
 **
 *    Client for the shared memory time/date service.
 *    MULTIPLE-SERVER-SAFE version.
 */

/*
 * program uses shared memory with key 99
 * program uses semaphore set with key 9900
 * 
 * build: cc sol15.9clnt.c -o sol15.9clnt
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/shm.h>
#include	<time.h>
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/sem.h>

#define	TIME_MEM_KEY	99		/* kind of like a port number */
#define	TIME_SEM_KEY	9900		/* like a filename	      */
#define	SEG_SIZE	((size_t)100)		/* size of segment	*/
#define oops(m,x)  { perror(m); exit(x); }
union semun { int val ; struct semid_ds *buf ; ushort *array; };

void release_lock( int semset_id );
void wait_and_lock( int semset_id );

int main()
{
	int	seg_id;
	char	*mem_ptr, *ctime();

	int	semset_id;		/* id for semaphore set	*/

	/* get the shared memory segment */

	seg_id = shmget( TIME_MEM_KEY, SEG_SIZE, 0777 );
	if ( seg_id == -1 )
		oops("shmget",1);

	/* attach to it and get a pointer to where it attaches */

	mem_ptr = shmat( seg_id, NULL, 0 );
	if ( mem_ptr == ( void *) -1 )
		oops("shmat",2);

	/* get a connection to semaphore set 9900 */
	/* use that semaphore set to wait until memory is OK to read */

	semset_id = semget( TIME_SEM_KEY, 0, 0);
	wait_and_lock( semset_id );

	printf("The time, direct from memory: ..%s", mem_ptr);

	release_lock( semset_id );
	shmdt( mem_ptr );		/* detach, but not needed here */
	return 0;
}

/* 
 * build and execute a 2-element action set: 
 *    wait for 0 on n_writers AND increment n_readers
 */
void wait_and_lock( int semset_id )
{
	struct sembuf actions[2];	/* action set		*/

	actions[0].sem_num = 1;		/* sem[1] is n_writers	*/
	actions[0].sem_flg = SEM_UNDO;	/* auto cleanup		*/
	actions[0].sem_op  = 0 ;	/* wait for 0		*/

	actions[1].sem_num = 0;		/* sem[0] is n_readers	*/
	actions[1].sem_flg = SEM_UNDO;	/* auto cleanup		*/
	actions[1].sem_op  = +1 ;	/* incr n_readers	*/

	if ( semop( semset_id, actions, 2) == -1 )
		oops("semop: locking", 10);
}

/*
 * build and execute a 1-element action set:
 *    decrement num_readers
 */
void release_lock( int semset_id )
{
	struct sembuf actions[1];	/* action set		*/

	actions[0].sem_num = 0;		/* sem[0] is n_readers	*/
	actions[0].sem_flg = SEM_UNDO;	/* auto cleanup		*/
	actions[0].sem_op  = -1 ;	/* decr reader count	*/

	if ( semop( semset_id, actions, 1) == -1 )
		oops("semop: unlocking", 10);
}
