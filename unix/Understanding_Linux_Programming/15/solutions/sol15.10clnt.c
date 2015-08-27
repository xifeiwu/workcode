/** sol15.10clnt.c
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
 *    Client for FILE version of time/date service.
 *    MULTIPLE-SERVER-SAFE
 *    Uses SEMAPHORES for locks
 *
 *    This client uses the semaphore code from sol15.9clnt.c 
 *    and takes the file access code from file_tc.c.  You
 *    know, reading a file, is really reading kernel buffers,
 *    and kernel buffers are part of computer memory, so
 *    reading a shared file is really reading a shared memory
 *    segment, just one that happens to be in kernel space and
 *    not in process space.
 *
 *    build: cc sol15.10clnt.c -o sol15.10clnt
 */

/*
 * program uses semaphore set with key 9900
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<time.h>
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/sem.h>
#include	<fcntl.h>

#define	TIME_SEM_KEY	9900		/* like a filename	      */

#define oops(m,x)  { perror(m); exit(x); }
union semun { int val ; struct semid_ds *buf ; ushort *array; };

void wait_and_lock( int semset_id );
void release_lock( int semset_id );

int main(int ac, char *av[])
{
	int	fd;
	int	semset_id;		/* id for semaphore set	*/
	char	buf[BUFSIZ];
	int	nread;

	/* open the file if we have one */
	if ( ac == 1 ){
		fprintf(stderr,"usage: %s filename\n", *av);
		exit(1);
	}
	if ( (fd=open(av[1],O_RDONLY)) == -1 )
		oops(av[1],2);


	/* connect to semaphore set 9900 */
	semset_id = semget( TIME_SEM_KEY, 0, 0);

	wait_and_lock( semset_id );
	if ( (nread = read(fd, buf, BUFSIZ)) > 0 ){
		buf[nread] = '\0';
		printf("The time, direct from disk: ..%s", buf);
	}
	release_lock( semset_id );
	close(fd);
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
