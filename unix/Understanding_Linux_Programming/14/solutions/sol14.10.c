/** sol14.10.c
 ** ------------------------------------------------------------
	A version of twordcount4.c that has no global variables is
	sol14.10.c.
	The solution is to pass pointers to local variables in 
	main.  Because a thread can only receive one argument,
	those pointers have to be passed as members in a struct.

 ** ------------------------------------------------------------
 **
 **
 *  A version of twordcount4.c without global variables.
 *  Pointers in main are passed to threads.  Because a
 *  thread can only receive one argument, the pointers have
 *  to be passed as members in a struct.  
 * 
 *  Also, if you #define SLOWREAD then the main thread will
 *  be slow about trying to read from the mailbox.  That
 *  will allow both writers to finish first and need to
 *  serialize access to the mailbox.
 */

/* twordcount4.c - threaded word counter for two files.	
 *		 - Version 4: condition variable allows counter
 *                            functions to report results early 
 *
 * build: cc sol14.10.c -lpthread -o sol14.10
 */

#include  <stdio.h>
#include  <stdlib.h>
#include  <unistd.h>
#include  <pthread.h>
#include  <ctype.h>

struct arg_set {		/* two values in one arg*/
		char *fname;	/* file to examine	*/
		int  count;	/* number of words	*/
		struct arg_set **mailboxp;
		pthread_mutex_t *lockp;
		pthread_cond_t  *flagp;
};

static void	*count_words(void *a);
static int	do_count(char *filename);
static void	mnarrate(char *str);
static void	cnarrate(char *str);

int main(int ac, char *av[])
{
	pthread_t       t1, t2;		/* two threads */
	struct arg_set  args1, args2;	/* two argsets */
	void	        *count_words(void *);
	int             reports_in = 0;
	int	        total_words = 0;
	struct arg_set  *mailbox = NULL;
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t  flag = PTHREAD_COND_INITIALIZER;

	if ( ac != 3 ){
		printf("usage: %s file1 file2\n", av[0]);
		exit(1);
	}
	pthread_mutex_lock(&lock);	/* lock the report box now */

	/* set the values common to both structs */
	args1.mailboxp = &mailbox;
	args1.lockp    = &lock;
	args1.flagp    = &flag;
	args1.count    = 0;

	args1.fname = av[1];
	pthread_create(&t1, NULL, count_words, (void *) &args1);

	args2 = args1;
	args2.fname = av[2];
	pthread_create(&t2, NULL, count_words, (void *) &args2);

	while( reports_in < 2 ){
		mnarrate("waiting for signal on flag");
#ifdef SLOWREAD
		sleep(5);
#endif
		pthread_cond_wait(&flag, &lock); 

		mnarrate("flag signaled, MAIN has the lock");
		printf("%7d: %s\n", mailbox->count, mailbox->fname);
		total_words += mailbox->count;
		if ( mailbox == &args1) 
			pthread_join(t1,NULL);
		if ( mailbox == &args2) 
			pthread_join(t2,NULL);

		/* clear out the mailbox and signal the flag */
		mnarrate("clearing box and signaling flag");
		mailbox = NULL;
		pthread_cond_signal(&flag);
		reports_in++;
	}
	printf("%7d: total words\n", total_words);
	return 0;
}
static void *count_words(void *a)
{
	struct arg_set *args = a;	/* cast arg back to correct type */
	
	args->count = do_count(args->fname);

	cnarrate("count done, waiting to get lock");
	pthread_mutex_lock(args->lockp);

	cnarrate("have lock");
	if ( *(args->mailboxp) != NULL ){
		cnarrate("oops..box is not empty, will wait..");
		pthread_cond_wait(args->flagp,args->lockp);
	}

	cnarrate("storing pointer to args in mailbox");
	*(args->mailboxp) = args;

	cnarrate("signaling flag to wake main");
	pthread_cond_signal(args->flagp);	/* raise the flag */

	cnarrate("unlocking box");
	pthread_mutex_unlock(args->lockp);	/* release the mailbox */

	return NULL;
}
/*
 * count the number of words in a file
 */
static int do_count(char *filename)
{
	FILE	*fp;
	int	c, prevc = '\0';
	int	rv = 0;

	if ( (fp = fopen(filename, "r")) != NULL )
	{
		while( ( c = getc(fp)) != EOF ){
			if ( !isalnum(c) && isalnum(prevc) )
				rv++;
			prevc = c;
		}
		fclose(fp);
	} else 
		perror(filename);

	return rv;
}

/*
 * used to clean up code and centralize output
 */
static void mnarrate(char *str)
{
	printf("MAIN: %s\n", str);
}
static void cnarrate(char *str)
{
	printf("COUNT: %s\n", str);
}
