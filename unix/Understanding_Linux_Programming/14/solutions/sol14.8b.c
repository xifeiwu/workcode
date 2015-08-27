/** sol14.8b.c
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
 *   A single threaded version of word count program.  This 
 *   version uses execvp to run wc -w.  
 *
 *   build: cc sol14.8b.c -o sol14.8b
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<ctype.h>

#define	oops(s,x) { perror(s); exit(x); }

int count_words(char *);

int main(int ac, char *av[])
{
	int	total=0, words;
	int	i;

	if ( ac == 1 ){
		fprintf(stderr,"usage: %s file ..\n", *av);
		exit(1);
	}

	for(i=1; i<ac; i++){
		words = count_words(av[i]);
		printf("%7d %s\n", words, av[i]);
		total += words;
	}

	printf("%7d TOTAL words\n", total);
	return 0;
}

/*
 * count words
 */
int count_words(char *filename)
{
	FILE *fp;
	int  c, prevc = '\0';
	int  num=0;
	
	if ( (fp = fopen(filename, "r")) != NULL ){
		while( ( c = getc(fp)) != EOF ){
			if ( !isalnum(c) && isalnum(prevc) )
				num++;
			prevc = c;
		}
		fclose(fp);
	} else 
		perror(filename);
	return num;
}
