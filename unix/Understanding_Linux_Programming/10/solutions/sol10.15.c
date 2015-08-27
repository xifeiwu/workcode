/** sol10.15.c
 ** ------------------------------------------------------------
	A "progtee" program that sends stdin to stdout 
	and also into a command is
	sol10.15.c.
	The program is a good exercise in fork, exec, and pipe.  It
	also introduces the "tee" program, and in doing so, 
	reviews the ideas of writing to a file and writing to a process.

 ** ------------------------------------------------------------
 **
 **
 *   progtee.c
 *
 *   purpose: pipe-fitting to direct data to stdout and also to a pipe
 *     usage: progtee cmd arg1 arg2 ..
 *    effect: standard input is passed to cmd arg1 arg2 .. 
 *            and also to stdout

 *	build: cc sol10.15.c -o sol10.15
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include	<unistd.h>

#define	oops(s,x) { perror(s); exit(x); }

int main(int ac, char *av[])
{
	int	apipe[2];
	pid_t	pid;
	void	run_command(int, char **, int []);
	int	split_data(pid_t, int[]);

	if ( ac == 1 ){
		fprintf(stderr,"usage: progtee cmd [arg..]\n");
		exit(1);
	}
	/*
	 * make a pipe and then a process, take it from there
	 */

	if ( pipe(apipe) == -1 )
		oops("pipe",1);

	if ( ( pid = fork() ) == -1 )
		oops("fork",2);

	if ( pid == 0 )
		run_command(ac,av,apipe);
	else
		split_data(pid, apipe);
	return 0;
}

/*
 * run_command
 *   child runs a command with its input from the reading end of pipe
 *   Note the kludgy trick of shifting arg list up one so there is
 *   space to put the NULL.  (actually the null is already at the
 *   end, but that is not promised).
 */
void
run_command(int ac, char *av[], int apipe[])
{
	int	i;

	/* set up plumbing */

	if ( close(apipe[1]) == -1 )
		oops("closing writing end in child",8);
	if ( dup2(apipe[0], 0) == -1 )
		oops("dup2",9);
	if ( close(apipe[0]) == -1 )
		oops("closing reading end in child",8);

	/* build arglist */

	for(i=1;i<ac;i++)
		av[i-1] = av[i];
	av[ac-1] = NULL;

	/* run command */

	execvp(av[0], av);
	oops(av[0],10);
}

/*
 * split_data(pid,apipe)
 *   parent executes this code to copy stdin to stdout and into pipe
 *   then wait for child to exit
 */
int
split_data(pid_t pid, int apipe[])
{
	void	put_data(int, char *, int );
	char	buf[BUFSIZ];
	int	n;

	if ( close(apipe[0]) == -1 )
		oops("closing reading end in parent",3);

	/* transfer all data */
	while( (n = read(0, buf, BUFSIZ)) > 0 ){
		put_data(1, buf, n);
		put_data(apipe[1], buf, n);
	}

	/* close writing end and wait for child */
	if ( close(apipe[1]) == -1 )
		oops("close writing end in parent",4);
	while( wait(NULL) != pid )
		;
	return 0;
}

/*
 * write data to fd in as many parts at it takes
 */
void
put_data(int fd, char *buf, int amt)
{
	int	amt_out;
	int	total_put = 0;

	while( total_put < amt )
	{
		amt_out = write(fd,buf+total_put,(amt-total_put));
		if ( amt_out == -1 )
			oops("writing out data",5);
		total_put += amt_out;
	}
}
