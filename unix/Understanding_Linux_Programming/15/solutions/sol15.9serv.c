/** sol15.9serv.c
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
 *   server - SHARED MEMORY time/date server
 *   MULTIPLE-SERVER-SAFE
 *   SEMAPHORES for locks
 *
 *   A version of shm_ts2.c that allows multiple servers to write
 *   to shared memory safely.  The system uses semaphores to control
 *   access to the shared memory segment.  The system supports
 *   multiple readers and multile writers.  The multiple writers
 *   control access by means of a mutex implemented as a semaphore
 *   starting with a value of 1.
 *   
 */
/*
 * program uses shared memory with key 99
 * program uses semaphore set with key 9900
 *
 * build: cc sol15.9serv.c -o sol15.9serv
 */

#include	<stdio.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/shm.h>
#include	<time.h>
#include	<sys/types.h>
#include	<sys/file.h>
#include	<fcntl.h>
#include	<sys/sem.h>
#include	<signal.h>
#include	<errno.h>

#define	TIME_MEM_KEY	99			/* like a filename      */
#define	TIME_SEM_KEY	9900
#define	SEG_SIZE	((size_t)100)		/* size of segment	*/
#define oops(m,x)  { perror(m); exit(x); }

union semun { int val ; struct semid_ds *buf ; ushort *array; };

int	seg_id, semset_id;			/* global for cleanup()	*/
void	cleanup(int);

int mypid;

void release_lock( int semset_id );
void wait_and_lock( int semset_id );
void set_sem_value(int semset_id, int semnum, int val);
void cleanup(int n);
void narrate(char *s);

int main()
{
	char	*mem_ptr, *ctime();
	time_t	now;
	int	n;

	mypid = getpid();

	/* create a shared memory segment */

	seg_id = shmget( TIME_MEM_KEY, SEG_SIZE, IPC_CREAT|0777 );
	if ( seg_id == -1 )
		oops("shmget", 1);

	/* attach to it and get a pointer to where it attaches */

	mem_ptr = shmat( seg_id, NULL, 0 );
	if ( mem_ptr == ( void *) -1 )
		oops("shmat", 2);


	/* create a semset: key 9900, 3 semaphores, and mode rw-rw-rw */
	/* if already there, just get the id.  If not initialize it */

	semset_id = semget( TIME_SEM_KEY, 3, (0666|IPC_CREAT|IPC_EXCL) );
	if ( semset_id == -1 )
	{
		if ( errno != EEXIST )
			oops("semget", 3);
		printf("[%d] using existing semset\n", mypid);
		semset_id = semget( TIME_SEM_KEY, 3, 0 );
	}
	else
	{
		set_sem_value( semset_id, 0, 0);	/* set counters	*/
		set_sem_value( semset_id, 1, 0);	/* both to zero */
		set_sem_value( semset_id, 2, 1);	/* set mutex=1  */
		signal(SIGINT, cleanup);
		printf("[%d] created semset\n", mypid);
	}

	/* run for a minute */
	for(n=0; n<60; n++ ){
		time( &now );			/* get the time	*/
				narrate("waiting for memory");
		wait_and_lock(semset_id);	/* lock memory	*/
				narrate("HAVE (updating) memory");
		strcpy(mem_ptr, ctime(&now));	/* write to mem */
				sleep(5);
				narrate("FREEING memory");
		release_lock(semset_id);	/* unlock	*/
		sleep(1);			/* wait a sec   */
	}
		
	cleanup(0);
	return 0;
}

void narrate(char *s)
{
	printf("\tshm_ts2 [%d] %s\n", mypid, s);
}

void cleanup(int n)
{
	shmctl( seg_id, IPC_RMID, NULL );	/* rm shrd mem	*/
	semctl( semset_id, 0, IPC_RMID, NULL);	/* rm sem set	*/
}

/*
 * initialize a semaphore
 */
void set_sem_value(int semset_id, int semnum, int val)
{
	union semun  initval;

	initval.val = val;
	if ( semctl(semset_id, semnum, SETVAL, initval) == -1 )
		oops("semctl", 4);
}

/* 
 * build and execute a 3-element action set: 
 *    wait for 0 on n_readers AND decrement by 1 the mutex
 *    AND increment n_writers
 */
void wait_and_lock( int semset_id )
{
	struct sembuf actions[3];	/* 3 ACTIONS in the set	*/

	actions[0].sem_num = 0;		/* sem[0] is n_readers	*/
	actions[0].sem_flg = SEM_UNDO;	/* auto cleanup		*/
	actions[0].sem_op  = 0 ;	/* wait til no readers	*/

	actions[1].sem_num = 2;		/* sem[2] is mutex    	*/
	actions[1].sem_flg = SEM_UNDO;	/* auto cleanup		*/
	actions[1].sem_op  = -1 ;	/* wait until free 	*/

	actions[2].sem_num = 1;		/* sem[1] is n_writers	*/
	actions[2].sem_flg = SEM_UNDO;	/* auto cleanup		*/
	actions[2].sem_op  = +1 ;	/* incr num writers	*/

	if ( semop( semset_id, actions, 3) == -1 )
		oops("semop: locking", 10);
}

/*
 * build and execute a 2-element action set:
 *    increment mutex
 *    decrement num_writers
 */
void release_lock( int semset_id )
{
	struct sembuf actions[2];	/* 2 ACTIONS in the set	*/

	actions[0].sem_num = 2;		/* sem[2] is mutex    	*/
	actions[0].sem_flg = SEM_UNDO;	/* auto cleanup		*/
	actions[0].sem_op  = +1 ;	/* release mutex    	*/

	actions[1].sem_num = 1;		/* sem[0] is n_writerS	*/
	actions[1].sem_flg = SEM_UNDO;	/* auto cleanup		*/
	actions[1].sem_op  = -1 ;	/* decr writer count	*/

	if ( semop( semset_id, actions, 2) == -1 )
		oops("semop: unlocking", 10);
}
