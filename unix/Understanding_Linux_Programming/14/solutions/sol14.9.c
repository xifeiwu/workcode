/** sol14.9.c
 ** ------------------------------------------------------------
	A version of twordcount4.c that handles more than two
	files is
	sol14.9.c.

	This is an important exercise.  Notice how the solution
	uses two condition variables and one lock.  The two-file
	version in the text only needs only one condition variable,
	but that method does not work with three or more
	threads producing data to be passed through a single
	shared variable to a consumer thread.  The consumer has
	to block until the variable has data to read, and the
	producers have to block until the variable is empty.

 ** ------------------------------------------------------------
 **
 *   A multi-file version of twordcount4.c
 *
 *   This program uses two condition variables so that the main thread
 *   can wait for the box to be full, and the other threads can wait
 *   for the box to be empty.  
 *
 *   The two-file version of this program did not need two condition
 *   variables.  If the box was not empty, the other counting thread
 *   had finished and would not need to wait for the box to become
 *   empty.  
 */  
/* twordcount4.c - threaded word counter for two files.	
 *		 - Version 4: condition variable allows counter
 *                            functions to report results early 
 *
 *  build: cc sol14.9.c -lpthread -o sol14.9
 */

#include  <stdio.h>
#include  <pthread.h>
#include  <ctype.h>
#include  <stdlib.h>

struct arg_set {		/* two values in one arg*/
		char *fname;	/* file to examine	*/
		int  count;	/* number of words	*/
		int  setid;	/* used for joining	*/
};

struct arg_set  *mailbox;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  flag_box_full  = PTHREAD_COND_INITIALIZER;
pthread_cond_t  flag_box_empty = PTHREAD_COND_INITIALIZER;

#define	oops(s,x) { perror(s); exit(x); }

int main(int ac, char *av[])
{
	pthread_t      *threads;	/* an array of threads */
	struct arg_set *argsets;	/* an array of argsets */
	void	       *count_words(void *);
	int            reports_in = 0;
	int	       total_words = 0;
	void           *emalloc(unsigned int);
	int            i;

	if ( ac == 1 ){
		printf("usage: %s file ...\n", av[0]);
		exit(1);
	}

	/*
	 * allocate space for the threads and their argsets
	 */
	threads = (pthread_t *) emalloc(ac * sizeof(pthread_t));
	argsets = (struct arg_set*) emalloc(ac * sizeof(struct arg_set));

	pthread_mutex_lock(&lock);	/* lock the report box now */

	/** launch the threads */

	for(i = 1; i<ac ; i++ ){
		argsets[i].fname = av[i];
		argsets[i].count = 0;
		argsets[i].setid = i;
		if ( pthread_create(threads+i,NULL,count_words,argsets+i) )
			oops("pthread_create",3);
	}

	/** wait for the reports */

	while( reports_in < ac-1 ){
		printf("%2d Reports to go..\n", (ac-1)-reports_in);
		printf("MAIN: Waiting for flag to be signalled\n");
		/* wait for the box to be full */
		if ( pthread_cond_wait(&flag_box_full, &lock) )
			oops("pthread_cond_wait",4);
		printf("MAIN: thread %d signalled, I have the lock\n",
				mailbox->setid);
		printf("%7d: [%d] %s\n", mailbox->count, 
					mailbox->setid, mailbox->fname);
		total_words += mailbox->count;
		if ( pthread_join(threads[mailbox->setid],NULL) )
			oops("pthread_cond_wait",5);
		reports_in++;
		/* then empty the box and tell anyone who wants to know */
		mailbox = NULL;
		pthread_cond_signal(&flag_box_empty);
	}
	printf("%7d: total words\n", total_words);
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

	printf("COUNT: %d waiting to get lock\n", args->setid );
	pthread_mutex_lock(&lock);	/* get the mailbox */
	printf("COUNT: %d have lock\n", args->setid);

	while( mailbox != NULL ){
		printf("COUNT: %d box is full, wait for empty\n",args->setid);
		pthread_cond_wait(&flag_box_empty, &lock);
	}
	printf("COUNT: %d now writing to mailbox\n", args->setid);
	mailbox = args;			/* put ptr to our args there */

	printf("COUNT: %d signaling box is full\n", args->setid);
	if ( pthread_cond_signal(&flag_box_full) )/* notify reader */
		oops("pthread_cond_signal",6);

	printf("COUNT: %d unlocking box\n", args->setid);
	if ( pthread_mutex_unlock(&lock) ) 	/* release the mailbox */
		oops("pthread_mutex_unlock",7);

	return NULL;
}

void *
emalloc(unsigned int n)
{
	void *p;

	if ( (p = malloc(n)) != NULL )
		return p;
	oops("malloc", 2);
}
