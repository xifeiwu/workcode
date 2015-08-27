/** sol14.7b.c
 ** ------------------------------------------------------------
	Versions of twordcount2.c and twordcount3.c that
	accept an arbitrary number of files are
	sol14.7a.c
	and sol14.7b.c
	respectively.

	Extending twordcount3.c is slightly more complicated
	because each thread has to have its own argset structure.

	The extension of twordcount3.c runs faster because the
	separate threads, and there may be many of them, do
	not need to contend for the mutex.

 ** ------------------------------------------------------------
 **
 **
 *   A version of twordcount3.c that handles many files by making
 *   an array of threads.
 */

/* twordcount3.c - threaded word counter for two files.	
 *		 - Version 3: one counter per file
 *
 * build: cc sol14.7b.c -lpthread -o sol14.7b
 */

#include  <stdio.h>
#include  <pthread.h>
#include  <ctype.h>
#include  <stdlib.h>

#define oops(s,x)  { perror(s); exit(x); }

struct arg_set {		/* two values in one arg */
		char *fname;	/* file to examine	 */
		int  count;	/* number of words	 */
};

int main(int ac, char *av[])
{
	pthread_t      *threads;	/* a list of threads */
	struct arg_set *argsets;	/* a list of argsets */
	void	       *count_words(void *);
	int            i, total = 0;

	if ( ac == 1 ){
		printf("usage: %s file ..\n", av[0]);
		exit(1);
	}

	/*
	 * make the arrays (make an extra one to make indexing easier)
	 */
	threads = (pthread_t *)malloc(ac*sizeof(pthread_t));
	if ( threads == NULL )
		oops("malloc",1);
	argsets = (struct arg_set *)malloc(ac*sizeof(struct arg_set));
	if ( argsets == NULL )
		oops("malloc",2);

	/*
	 * make and start the threads
 	 */
	for(i=1; i<ac; i++){
		argsets[i].fname = av[i];
		argsets[i].count = 0;
		if ( pthread_create(&threads[i], NULL, count_words,
						(void *) &argsets[i]) )
			oops("pthread_create",3);
	}
	/*
	 * wait for them all to finish
	 * and print and accumulate their totals
	 */
	for(i=1; i<ac; i++){
		if ( pthread_join(threads[i], NULL) )
			oops("pthread_join",4);

		printf("%7d: %s\n", argsets[i].count, av[i]);
		total += argsets[i].count;
	}
	printf("%7d: total words\n", total);
	return 0;
}

void *count_words(void *a)
{
	struct arg_set *args = a;	/* cast arg back to correct type */
	FILE *fp;
	int  c, prevc = '\0';
	
	if ( (fp = fopen(args->fname, "r")) != NULL ){
		while( ( c = getc(fp)) != EOF ){
			if ( !isalnum(c) && isalnum(prevc) )
				args->count++;
			prevc = c;
		}
		fclose(fp);
	} else 
		perror(args->fname);
	return NULL;
}
