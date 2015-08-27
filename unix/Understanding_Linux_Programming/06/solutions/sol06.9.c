/** sol06.9.c
 ** ------------------------------------------------------------
	A version of sigdemo2.c that turns off ISIG in the terminal driver
	is
	sol06.9.c.
	This program cannot be stopped from the keyboard.  The usual
	control characters that generate signals are passed along as
	regular characters.  The only way to stop this process is to
	open another xterm or login from another machine and kill the
	process running the program.  That skill is extremely useful.

 ** ------------------------------------------------------------
 **
 **
 *   A version of sigdemo2.c that turns off ISIG in the terminal driver
 *   Warning: this program cannot be stopped from the keyboard.  You
 *            have to open a new shell and kill this process.
 *
	build: cc sol06.9.c -o sol06.9
 */

#include	<stdio.h>
#include	<termios.h>

main()
{
	struct termios settings;

	tcgetattr(0, &settings);		/* get settings		*/
	settings.c_lflag  &= ~ISIG;		/* no sig generation	*/
	tcsetattr( 0 , TCSANOW, &settings);	/* install settings	*/

	printf("you can't stop me!\n");
	while( 1 )
	{
		sleep(1);
		printf("haha\n");
	}
}
