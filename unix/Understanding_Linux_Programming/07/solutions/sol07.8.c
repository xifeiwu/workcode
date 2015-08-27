/** sol07.8.c
 ** ------------------------------------------------------------
	A simple application of curses and sleep to produce a
	blinking message is
	sol07.8.c.
	Many terminals/emulators support a blinking attribute,
	but this program produces blinking with software not
	with (virtual) hardware.  Does anyone actually use a
	terminal anymore?

 ** ------------------------------------------------------------
 **
 **
 *   Blinking text example.  Prints Hello, world. or any argument
 *   provided on the command line.  Some terminals have hardware
 *   blinking on the screen, but this program uses curses to run
 *   the blinking.
 *
 *   build:  cc sol07.8.c -lcurses -o sol07.8
 */

#include	<curses.h>

main(int ac, char *av[])
{
	char	*msg = "Hello, world.";
	int	row, col;

	if ( ac > 1 )
		msg = av[1];

	initscr();
	clear();

	/* figure out centering */
	row = LINES/2;
	col = (COLS-strlen(msg))/2;
	if ( col<0 )
		col = 0;

	/* main loop */
	while(1){
		mvaddstr(row,col,msg);
		move(LINES-1,COLS-1);
		refresh();
		sleep(1);
		clear();
		move(LINES-1,COLS-1);
		refresh();
		sleep(1);
	}
}
