/** sol14.7a.c
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
 *  A version of twordcount2.c that accepts many filenames on the 
 *  command line.
 */
/* twordcount2.c - threaded word counter for two files.	   */
/*                 version 2: uses mutex to lock counter   */
/*
 * build: cc sol14.7a.c -lpthread -o sol14.7a
 */

#include  <stdio.h>
#include  <pthread.h>
#include  <ctype.h>
#include  <stdlib.h>

int	        total_words ;    /* the counter and its lock */
pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER;

#define oops(s,x) { perror(s); exit(x);}

int main(int ac, char *av[])
{
	pthread_t *thread_list;
	void	  *count_words(void *);
	int	  i;

	if ( ac == 1 ){
		printf("usage: %s file ..\n", av[0]);
		exit(1);
	}
	/*
	 * make one for each arg (but we'll not use index 0)
	 */
	thread_list = (pthread_t *) malloc(ac*sizeof(pthread_t));
	if ( thread_list == NULL )
		oops("malloc",1);

	total_words = 0;
	for(i=1; i<ac; i++)
		if ( pthread_create(&thread_list[i], NULL, 
					count_words, (void *) av[i]) )
			oops("pthread_create",2);

	/*
	 * then wait for them all to finish
	 */
	for(i=1; i<ac; i++)
		pthread_join(thread_list[i], NULL);

	printf("%5d: total words\n", total_words);
	return 0;
}
void *count_words(void *f)
{
	char *filename = (char *) f;
	FILE *fp;
	int  c, prevc = '\0';
	
	if ( (fp = fopen(filename, "r")) != NULL ){
		while( ( c = getc(fp)) != EOF ){
			if ( !isalnum(c) && isalnum(prevc) ){
				pthread_mutex_lock(&counter_lock);
				total_words++;
				pthread_mutex_unlock(&counter_lock);
			}
			prevc = c;
		}
		fclose(fp);
	} else 
		perror(filename);
	return NULL;
}
