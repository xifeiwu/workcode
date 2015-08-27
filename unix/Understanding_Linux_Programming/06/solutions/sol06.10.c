/** sol06.10.c
 ** ------------------------------------------------------------
	A program that uses a signal handler to generate a status
	report is
	sol06.10.c.
	This program computes prime numbers and keeps track of how
	many prime numbers it has found and the largest prime number it
	has found.  By sending the program SIGINT, the user can invoke
	a function that prints out the status of the search.

	Using signal handlers to invoke internal functions is a
	powerful tool in designing programs.  The signal handler
	can be used to dump internal data for diagnostic purposes
	and also for tracking purposes.

 ** ------------------------------------------------------------
 **
 **
 *	slowprimes - a program that looks for prime numbers slowly.
 *
 *	The real purpose of this program is to show how a signal
 *	handler can be used to report status of a running program.
 *
 *	Details:  This program looks for prime numbers by trying each
 *		  integer and testing by trying to divide by all integers
 *		  less than that integer.  I know it's dumb, but the goal
 *		  is to have a very slow program that can be asked, by
 *		  sending it SIGINT, how far it has gotten.
	build: cc sol06.10.c -o sol06.10
 */

#include	<stdio.h>
#include	<signal.h>

/*
 * status information here so the handler can get at them
 */

long	current;
long	largest_prime_so_far;
long	primes_found;

main()
{
	void	status(int);
	long	divisors, test;

	current = 2;
	largest_prime_so_far = 2;

	signal(SIGINT, status);
	
	while(1)
	{
		divisors = 0;

		/* try all numbers from 2 to num-1 */
		for( test = 2; test < current ; test++ )
			if ( (current % test) == 0 )
				divisors++;

		/* if nothing goes in evenly, num is prime */
		if ( divisors == 0 ){
			largest_prime_so_far = current;
			primes_found++;
		}

		current++;
	}
}

void status(int n)
/*
 * print out status of prime seeker
 */
{
	printf("Status of prime-number-finder:\n");
	printf("  Currently trying:     %8d\n", current);
	printf("  Largest prime found:  %8d\n", largest_prime_so_far);
	printf("  Primes found:         %8d\n", primes_found);
	printf("  Average density:      %8.0f\n",(100.0*primes_found)/current);
	putchar('\n');
}
