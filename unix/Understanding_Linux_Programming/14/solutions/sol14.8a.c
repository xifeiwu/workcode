/** sol14.8a.c
 ** ------------------------------------------------------------
	(a)
	A multi-process version of wordcount is
	sol14.8a.c.
	This program uses popen() to run "wc -w" for
	each file in a separate process and to create
	a pipe to each of those processes.

	(b)
	A single process, single threaded version of wordcount is
	sol14.8b.c.
	This program could have just built a new argument list
	and used execvp to run "wc" "-l" av[1] .. .  I did that
	and it ran faster than the version presented here.  

	(c)
	Ease of design and ease of coding depends on the programmer.
	Posix threads is generally portable to all versions of Unix,
	and popen is absolutely portable to all versions of Unix.

 ** ------------------------------------------------------------
 **
 **
 *   A multi-process version of word count program.  This 
 *   version uses popen() to create processes that run wc -w
 *
 *   The results are then read from the pipes and added to the
 *   total.
 *
 *   build: cc sol14.8a.c -o sol14.8a
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#define	oops(s,x) { perror(s); exit(x); }

int main(int ac, char *av[])
{
	FILE	**pipes;	/* an array of pipes	*/
	int	words;		/* file count		*/
	int	total;		/* total count		*/
	char	cmd[BUFSIZ+9];	/* wc -l < filename	*/
	int	i;

	if ( ac == 1 ){
		fprintf(stderr,"usage: %s file ..\n", *av);
		exit(1);
	}

	/** make an array of FILE *'s to hold streams */
	pipes = (FILE **) malloc( ac * sizeof(FILE *) );
	if ( pipes == NULL )
		oops("malloc",2);

	/** launch those processes */
	for(i=1; i<ac; i++){
		if ( strlen(av[i])>BUFSIZ ){
			fprintf(stderr,"pathname %s too long\n", av[i]);
			exit(3);
		}
		sprintf(cmd, "wc -w<%s", av[i]);
		if ( (pipes[i]=popen(cmd, "r")) == NULL )
			oops("popen",4);
	}

	/** round up all the processes */
	total = 0;
	for(i=1; i<ac; i++){
		if ( fscanf(pipes[i],"%d",&words) != 1 )
			oops("fscanf",5);
		printf("%7d %s\n", words, av[i]);
		total += words;
		pclose(pipes[i]);
	}
	printf("%7d TOTAL words\n", total);
	return 0;
}
