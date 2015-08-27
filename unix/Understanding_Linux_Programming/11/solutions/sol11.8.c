/** sol11.8.c
 ** ------------------------------------------------------------
	A version of pclose() is 
	sol11.8.c.
	pclose() has to call fclose() to release the memory
	set aside for buffers and to close the file descriptor.
	In this case, the file descriptor leads to a pipe,
	so calling fclose() will shut the parent's end of the
	pipe.  The other thing pclose() has to do is to
	wait() for the child and obtain its exit status.

	The simple, naive way to write pclose() is to call
	wait().  If another child exits between the call
	to popen() and pclose(), and that other process is
	not waited for, the call to pclose() will catch it.
	The purpose of pclose() is to wrap up all the resources
	used by the child and the connection.

	A better solution, therefore, is for popen() to 
	record the pid of the child so that pclose() knows
	which pid to wait for.  The code in this solution
	keeps an array of pids, indexed by file descriptors.
	popen() records the pid in the array and pclose()
	reads the pid from that array. 
	  
 ** ------------------------------------------------------------
 **
 **
 *  A version of the popen and pclose functions.  This
 *  version of popen() is slightly extended from the version
 *  in the text.  popen() has to record the pid of the
 *  process associated with the stream it returns so that pclose
 *  can wait for that process to finish.
 *
 *  For maximum flexibility we have an array of pids indexed
 *  by fd allocated dynamically.
 *
 *  popen.c -  a version of the Unix popen() library function
 *	FILE *popen( char *command, char *mode )
 *		command is a regular shell command
 *		mode is "r" or "w" 
 *		returns a stream attached to the command, or NULL
 *		execls "sh" "-c" command
 *    todo: what about signal handling for child process?

 */
#include	<stdio.h>
#include	<signal.h>
#include	<sys/types.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<sys/wait.h>
#include	<errno.h>

#define	READ	0
#define	WRITE	1

static  pid_t	*children = NULL;
static	int	listlen = 0;
#define	CHUNKSIZE	20		/* for allocating chunks	*/

static	void	record_pid(int, pid_t);	/* local function		*/

FILE *popen(const char *command, const char *mode)
{
	pid_t	pid;			/* the new process		*/
	int	pfp[2]; 		/* the pipe 			*/
	FILE	*fdopen();		/* fdopen makes a fd a stream	*/
	int	parent_end, child_end;	/* of pipe 			*/

	if ( *mode == 'r' ){		/* figure out direction		*/
		parent_end = READ;
		child_end = WRITE ;
	} else if ( *mode == 'w' ){
		parent_end = WRITE;
	       	child_end = READ ;
	} else return NULL ;

	if ( pipe(pfp) == -1 ) 			/* get a pipe		*/
		return NULL;
	if ( (pid = fork()) == -1 ){		/* and a process	*/
		close(pfp[0]);			/* or dispose of pipe	*/
		close(pfp[1]);
		return NULL;
	}

	/* --------------- parent code here ------------------- */
	/*   need to close one end and fdopen other end		*/

	if ( pid > 0 ){	
		if (close( pfp[child_end] ) == -1 )
			return NULL;
		record_pid( pfp[parent_end], pid );
		return fdopen( pfp[parent_end] , mode );	/* same mode */
	}

	/* --------------- child code here --------------------- */
	/*   need to redirect stdin or stdout then exec the cmd	 */

	if ( close(pfp[parent_end]) == -1 )	/* close the other end	*/
		exit(1);			/* do NOT return	*/

	if ( dup2(pfp[child_end], child_end) == -1 )
		exit(1);

	if ( close(pfp[child_end]) == -1 )	/* done with this one	*/
		exit(1);
						/* all set to run cmd	*/
	execl( "/bin/sh", "sh", "-c", command, NULL );
	exit(1);
}

/*
 * Record the pid for this fd in an array.
 * Expand the array if needed.
 */
static void 
record_pid(int fd, pid_t pid)
{
	size_t	newsize;

	/* if not a slot for this fd make one */
	if ( fd >= listlen ){
		newsize  = CHUNKSIZE * ( 1 + fd/CHUNKSIZE );
		children = (pid_t *) realloc(children,newsize*sizeof(pid_t));
		if ( children == NULL ){
			perror("out of memory");
			exit(1);
		}
	}
	/* now record it */
	children[fd] = pid;
}

/*
 * pclose fcloses the connection then waits for the child
 * pclose returns the child status or -1 if it cannot get status
 */
int
pclose(FILE *fp)
{
	pid_t	child = children[fileno(fp)];
	int	rv;
	int	status;

	fclose(fp);		/* free buffers and close pipe	*/

	/* ignore returns caused by jumps to signal handlers */
	while( (rv = waitpid(child,&status,0)) ==-1 && errno== EINTR )
		;
	return ( rv == -1 ? -1 : status );
}
