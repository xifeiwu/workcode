/** sol11.6.c
 ** ------------------------------------------------------------
	A revised version of tinybc.c that supports the -c
	option is
	sol11.6.c.

 ** ------------------------------------------------------------
 **
 **
 ** a version of tinybc.c that supports the -c option
 **
 ** The -c option just reads expressions and translates to RPN.
 ** There is no need to set up complex pipes.  Therefore, it is
 ** easier to split off the read and translate function and just
 ** call it from two places.
 **
 **	tinybc.c	* a tiny calculator that uses dc to do its work
 **			* demonstrates bidirectional pipes
 **			* input looks like number op number which
 **			  tinybc converts into number \n number \n op \n p
 **			  and passes result back to stdout
 **
 **		+-----------+                +----------+
 **	stdin  >0           >== pipetodc ====>          |
 **	        |  tinybc   |                |   dc -   |
 **	stdout <1           <== pipefromdc ==<          |
 **		+-----------+                +----------+
 **
 **			* program outline
 **				a. get two pipes
 **				b. fork (get another process)
 **				c. in the dc-to-be process,
 **					 connect stdin and out to pipes
 **					 then execl dc
 **				d. in the tinybc-process, no plumbing to do
 **					 just talk to human via normal i/o
 **					 and send stuff via pipe
 **				e. then close pipe and dc dies
 **			* note: this does not handle multiline results
 **/
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/wait.h>

#define	oops(m,x)	{ perror(m); exit(x); }

void compile_only();
void be_dc(int [], int []);
void be_bc(int [], int []);
int read_and_translate(FILE *, FILE *);
void fatal( char *);

int main(int ac, char *av[])
{
	int	pid, todc[2], fromdc[2];	/* equipment	*/

	/*
	 * special case for -c option
	 */
	if ( ac > 1 && strcmp(av[1], "-c") == 0 ){
		compile_only();
		return 0;
	}

	/* make two pipes */

	if ( pipe(todc) == -1 || pipe(fromdc) == -1 )
		oops("pipe failed", 1);

	/* get a process for user interface */

	if ( (pid = fork()) == -1 )
		oops("cannot fork", 2);
	if ( pid == 0 )				/* child is dc	*/
		be_dc(todc, fromdc);
	else {
		be_bc(todc, fromdc);		/* parent is ui	*/
		wait(NULL);			/* wait for child */
	}
	return 0;
}

/*
 * just read from stdin and write out compiled code

 *	build: cc sol11.6.c -o sol11.6
 */

void compile_only()
{
	while( read_and_translate(stdin,stdout) > 0 )
		;
}

void be_dc(int in[2], int out[2])
/*
 *	set up stdin and stdout, then execl dc
 */
{
    /* setup stdin from pipein	*/
	if ( dup2(in[0],0) == -1 )	/* copy read end to 0   */
		oops("dc: cannot redirect stdin",3);
	close(in[0]);			/* moved to fd 0	*/
	close(in[1]);			/* won't write here	*/

    /* setup stdout to pipeout	*/
	if ( dup2(out[1], 1) == -1 )	/* dupe write end to 1	*/
		oops("dc: cannot redirect stdout",4);
	close(out[1]);			/* moved to fd 1	*/
	close(out[0]);			/* won't read from here	*/

    /* now execl dc with the - option */
	execlp("dc", "dc", "-", NULL );
	oops("Cannot run dc", 5);
}

void be_bc(int todc[2], int fromdc[2])
/*
 *	read from stdin and convert into to RPN, send down pipe
 *	then read from other pipe and print to user
 *	Uses fdopen() to convert a file descriptor to a stream
 */
{
	FILE	*fpout, *fpin, *fdopen();
	char	buf[BUFSIZ];

	/* setup */
	close(todc[0]);			/* won't read from pipe to dc  */
	close(fromdc[1]);		/* won't write to pipe from dc */

	fpout = fdopen( todc[1],   "w" );	/* convert file desc-  */
	fpin  = fdopen( fromdc[0], "r" );	/* riptors to streams  */
	if ( fpout == NULL || fpin == NULL )
		fatal("Error convering pipes to streams");

	/* main loop */
	while( read_and_translate(stdin, fpout) > 0 ){
		if ( fgets( buf, BUFSIZ, fpin ) == NULL )
			break;
		fputs(buf,stdout);
	}
	fclose(fpout);		/* close pipe		*/
	fclose(fpin);		/* dc will see EOF	*/
}
/*
 * read_and_translate()
 *
 * Read a line of input from 'src' and write the
 * translated version to 'dest'.  
 * Returns 1 for ok, 0 for eof and calls fatal on errors
 */
int read_and_translate(FILE *src, FILE *dest)
{
	int	num1, num2;
	char	buf[BUFSIZ];
	char	op[BUFSIZ];

	while(1){
		if ( isatty(0) )
			printf("tinybc: ");
		if ( fgets(buf,BUFSIZ,stdin) == NULL )
			return 0;
		if ( sscanf(buf,"%d%[-+*/^]%d",&num1,op,&num2) == 3 )
			break;
		printf("syntax error: %s", buf);
	}
	if ( fprintf(dest,"%d\n%d\n%c\np\n",num1,num2,*op) == EOF )
		fatal("Error writing");
	fflush( dest );
	return 1;
}

void fatal( char *mess )
{
	fprintf(stderr, "Error: %s\n", mess);
	exit(1);
}
