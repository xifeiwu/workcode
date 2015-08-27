/** sol14.4.c
 ** ------------------------------------------------------------
	A revised version of hello_multi.c is
	sol14.4.c.

	In this program the "hello" thread waits for the "world" thread.  
	The only reason that was done was to avoid using global variables 
	or having to pass references.  The original thread could have 
	waited for the "world" thread if it had access to the variable.

 ** ------------------------------------------------------------
 **
 *  A version of hello_multi.c in which one thread creates another thread.
 *
 *  The example is a bit odd, because the same function is run in
 *  both threads, so it needs a way to tell if it is to create the
 *  second thread or not.
 *
 *  build: cc sol14.4.c -lpthread -o sol14.4
 */

/* hello_multi.c - a multi-threaded hello world program */

#include  <stdio.h>
#include  <unistd.h>
#include  <string.h>
#include  <pthread.h>

#define	NUM	5

int main()
{
	pthread_t t;

	void	*print_msg(void *);

	pthread_create(&t, NULL, print_msg, (void *)"hello");
	pthread_join(t, NULL);
	return 0;
}
void *print_msg(void *m)
{
	pthread_t t;
	char *cp = (char *) m;
	int i;

	if ( strcmp(cp, "hello") == 0 )
		pthread_create(&t, NULL, print_msg, (void *)"world\n");

	for(i=0 ; i<NUM ; i++){
		printf("%s", cp);
		fflush(stdout);
		sleep(1);
	}

	if ( strcmp(cp, "hello") == 0 )
		pthread_join(t, NULL);
	return NULL;
}
