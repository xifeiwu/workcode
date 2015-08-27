/** sol14.5.c
 ** ------------------------------------------------------------
	A revised version of twordcount1.c is
	sol14.5.c.

	This version is not any faster.  In the original version, 
	the original thread blocked until the worker threads finished, 
	so it used no time.  This version saves the time of creating 
	the extra thread, but that is minor.  This design is not
	better than the original one; the other one was clearer.

 ** ------------------------------------------------------------
 **
 **
 *  Version of twordcount1.c that uses only two threads: the original
 *  one to count the first file and a second one to count the second
 *  file.
 *
 *  build: cc sol14.5.c -lpthread -o sol14.5
 */

/* twordcount1.c - threaded word counter for two files. Version 1 */

#include  <stdio.h>
#include  <stdlib.h>
#include  <pthread.h>
#include  <ctype.h>

int	  total_words ;

int main(int ac, char *av[])
{
	pthread_t t1;			/* one thread */
	void	  *count_words(void *);

	if ( ac != 3 ){
		printf("usage: %s file1 file2\n", av[0]);
		exit(1);
	}
	total_words = 0;
	pthread_create(&t1, NULL, count_words, (void *) av[1]);
	count_words((void*) av[2]);
	pthread_join(t1, NULL);
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
			if ( !isalnum(c) && isalnum(prevc) )
				total_words++;
			prevc = c;
		}
		fclose(fp);
	} else 
		perror(filename);
	return NULL;
}
