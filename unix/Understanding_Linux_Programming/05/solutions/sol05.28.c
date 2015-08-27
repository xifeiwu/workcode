/** sol05.28.c
 ** ------------------------------------------------------------
	A program that toggles the OLCUC bit in the terminal
	driver is
	sol05.28.c.

 ** ------------------------------------------------------------
 **
 **
 *   uppercase.c - toggles and reports the current state of the OLCUC bit
 *
	build: cc sol05.28.c -o sol05.28
 */

#include        <stdio.h>
#include        <termios.h>

#define  oops(s,x) { perror(s); exit(x); }

int
main(int ac, char *av[])
{
        struct termios info;
	char *msg;

        if ( tcgetattr(0,&info) == -1 )          /* get attribs   */
		oops("tcgettattr", 1);

	if ( info.c_oflag & OLCUC ){
		info.c_oflag &= ~OLCUC;
		msg = "off";
	} 
	else {
		info.c_oflag |= OLCUC;
		msg = "ON";
	} 

        if ( tcsetattr(0,TCSANOW,&info) == -1 ) /* set attribs    */
               oops("tcsetattr",2);

	printf("OLCUC is now %s\n", msg);
	return 0;
}
