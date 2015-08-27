/** sol15.10serv.c
 ** ------------------------------------------------------------
	This solution shows how to use semaphores 
	to implement read and write locks on a file.  
	This solution is the answer to exercise 15.9
	with the shared memory segment replaced by
	a shared disk file.  
	 
	The server is
	sol15.10serv.c,
	and the matching client is
	sol15.10clnt.c.

 ** ------------------------------------------------------------
 **
 *    Server for FILE version of time/date service.
 *    MULTIPLE-SERVER-SAFE
 *    Uses SEMAPHORES for locks
 *
 *   A version of file_ts.c that allows multiple servers to write
 *   to a shared file safely.  Only one server is allowed access
 *   to the file at a time, so we need to add a mutex the
 *   servers use to limit access among themselves.  The mutex is
 *   implemented as a semaphore that starts at 1 and can be decremented 
 *   by one server at a time.
 *
 *   Based on sol15.9serv.c which is based on shm_ts2.c
 *
 *   build: cc sol15.10serv.c -o sol15.10serv
 */
/*
 * program uses semaphore set with key 9900
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	<time.h>
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/sem.h>
#include	<signal.h>
#include	<errno.h>
#include	<fcntl.h>

#define	TIME_SEM_KEY	9900

#define oops(m,x)  { perror(m); exit(x); }

union semun { int val ; struct semid_ds *buf ; ushort *array; };
int	semset_id;			/* global for cleanup()	*/

void release_lock( int semset_id );
void wait_and_lock( int semset_id );
void set_sem_value(int semset_id, int semnum, int val);
void cleanup(int n);
void narrate(char *s);

int mypid;

int main(int ac, char *av[])
{
	time_t	now;
	int	n;
	int	fd;

	if ( ac == 1 ){
		fprintf(stderr,"usage: %s filename\n", *av);
		exit(1);
	}
	mypid = getpid();

	/* create a shared file */

	if ( (fd = open(av[1],O_CREAT|O_TRUNC|O_WRONLY,0644)) == -1 )
		oops(av[1],2);

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
	for(n=0; n<60; n++ )
	{
		char	*cp;
				narrate("waiting for access");
		wait_and_lock(semset_id);	/* lock memory	*/
				narrate("HAVE (updating) memory");
		lseek(fd, 0L, SEEK_SET);
		time( &now );			/* get the time	*/
		cp = ctime(&now);
		if ( write(fd, cp, strlen(cp)) == -1 )
			oops("write",8);
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
	printf("\tfile_sem [%d] %s\n", mypid, s);
}

void cleanup(int n)
{
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
