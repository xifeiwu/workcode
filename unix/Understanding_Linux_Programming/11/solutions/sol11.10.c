/** sol11.10.c
 ** ------------------------------------------------------------
	A program that runs sort as a subroutine is
	sol11.10.c.
	This program is similar to tinybc, but it has to 
	write all the data and then close the pipe before 
	reading any data back.  The assignent says the 
	program should read all the data into an array, then write
	the array into sort, then read from sort back into the array, then
	print the array.  That design seems to waste memory; the data could
	just be streamed into sort then back from sort.  The 
	value of this design is that the logic could be used to 
	process data internally as part of a larger program.

 ** ------------------------------------------------------------
 **
 **
 *   A program that uses sort as a server subroutine.
 *   This program reads up to MAXLINES lines into an array
 *   of strings.  Then opens two pipes to sort, writes the
 *   array into sort, closes the pipe into sort then reads
 *   the result from the other pipe.
 *
 *   The results are put back into the array then printed.
 *   The purpose of the program is to show how to use ano-
 *   ther program as a server, how to use two pipes, and
 *   more practice with fork and exec.
 *   
 *   usage: sol11.10 [-v] < input > output
 *

 *	build: cc sol11.10.c -o sol11.10
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include	<unistd.h>

#define	MAXLINES	1000
#define	LINELEN		1000
#define	WR_END		1
#define	RD_END		0

#define	SORTPROG	"/bin/sort"
#define	SORTPROG2	"/usr/bin/sort"

char *newstr(char *s);
char *emalloc(size_t);

void readlist(char **, int, int);
void sortlist(char **);
void printlist(char **);
void besort(int [], int []);
void callsort(char *[], int [], int []);

#define	oops(s) { perror(s); exit(1); }

int	verbose = 0;

int
main(int ac, char *av[])
{
	char	*list[MAXLINES+1];	/* will be NULL terminated */

	if ( ac > 1 && strcmp(av[1],"-v") == 0 )
		verbose = 1;

	readlist(list,MAXLINES,LINELEN);
	sortlist(list);
	printlist(list);
	return 0;
}

/*
 * read up to maxitems, each up to linelen long into list
 * put NULL at the end
 */
void readlist(char *list[],int maxitems,int linelen)
{
	int	pos = 0;
	char	*buf;

	buf = emalloc(1+linelen);

	while( pos < maxitems && fgets(buf,linelen,stdin) != NULL )
		list[pos++] = newstr(buf);
	list[pos] = NULL;
	if ( verbose )
		fprintf(stderr,"Read %d lines into the list\n", pos);

	free(buf);
}

/*
 * sortlist(list)
 *   uses "sort" and two pipes and fork and exec and wait
 *   list is terminated with a null byte
 */

void sortlist(char *list[])
{
	int	tosort[2];		/* one pipe	*/
	int	fromsort[2];		/* the other	*/
	pid_t	pid;

	if ( pipe(tosort) == -1 )
		oops("pipe");
	if ( pipe(fromsort) == -1 )
		oops("pipe");

	if ( (pid=fork()) == -1 )
		oops("fork");

	/* child does sort with pipes attached to stdin and stdout */
	if ( pid == 0 )
		besort(tosort,fromsort);

	/* otherwise we are the parent */
	callsort(list, tosort, fromsort);
	wait(NULL);
}

/*
 * redirect stdio and execlp sort
 */
void besort(int in[], int out[])
{
	/* handle data coming in */
	if ( close(in[WR_END]) == -1 )
		oops("close");
	if ( dup2(in[RD_END],0) != 0 )
		oops("dup2");
	if ( close(in[RD_END]) == -1 )
		oops("close");

	/* handle pipe for output */
	if ( close(out[RD_END]) == -1 )
		oops("close");
	if ( dup2(out[WR_END],1) != 1 )
		oops("dup2");
	if ( close(out[WR_END]) == -1 )
		oops("close");

	/*
	 * notice this technique for searching for a program
	 * in two places. No 'if's needed!
	 */
	execl(SORTPROG,"sort",NULL);
	execl(SORTPROG2,"sort",NULL);
	oops("execl sort");
}

/*
 * set up pipes to call sort, then pass the list then get the answers
 */
void callsort(char *list[], int out[], int in[])
{
	FILE	*to_client, *from_client;
	char	*buf;
	int	maxlen = 0;
	int	i, numout;

	/*
	 * set up outbound pipe
	 */
	if ( close(out[RD_END]) == -1 )
		oops("close");
	if ( (to_client = fdopen(out[WR_END], "w")) == NULL )
		oops("fdopen to client");
	
	/*
	 * set up inbound pipe
	 */
	if ( close(in[WR_END]) == -1 )
		oops("close");
	if ( (from_client=fdopen(in[RD_END],"r")) == NULL )
		oops("fdopen from client");

	/*
	 * write the data, and free the memory
	 * And figure out largest line needed on return read
	 */

	for(i=0; list[i] != NULL ; i++ ){
		fputs(list[i], to_client);
		if ( strlen(list[i]) > maxlen )
			maxlen = strlen(list[i]);
		free(list[i]);
	}
	numout = i;
	fclose(to_client);
	if ( verbose )
		fprintf(stderr,"Wrote %d lines into sort\n", numout);

	/*
	 * read the results, make sure we don't use more space than
	 * we had on input
	 */

	buf = emalloc(1+maxlen);
	i = 0;
	while( i < numout && fgets(buf,1+maxlen,from_client) != NULL )
		list[i++] = newstr(buf);
	list[i] = NULL;
	free(buf);
	fclose(from_client);
	if ( verbose )
		fprintf(stderr,"Read %d lines from sort\n", i);
}

/*
 * print the list until a NULL is found
 */
void printlist(char *list[])
{
	int	i;

	for (i=0; list[i] ; i++)
		fputs(list[i], stdout);
}


char *emalloc(size_t n)
{
	char *rv;
	if ( (rv = (char*)malloc(n)) == NULL )
		oops("malloc");
	return rv;
}

char *newstr(char *s)
{
	char *rv;

	rv = emalloc(1+strlen(s));
	strcpy(rv, s);
	return rv;
}
