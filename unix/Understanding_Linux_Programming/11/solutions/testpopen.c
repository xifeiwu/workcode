#include	<stdio.h>

/*
 * test program for new popen/pclose functions
 *
 * build: cc testpopen.c sol11.8.c -o testpopen
 */

main()
{
	FILE	*in, *out;
	int	c;

	in = popen("ls -l /bin", "r");
	out = popen("sort -r > junk","w");

	while( ( c = getc(in) ) != EOF )
		putc(c,out);
	system("echo '';ps x");
	pclose(out);
	system("echo '';ps x");
	pclose(in);
	system("echo '';ps x");
}
