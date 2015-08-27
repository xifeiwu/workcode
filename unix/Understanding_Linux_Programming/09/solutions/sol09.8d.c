/** sol09.8d.c
 ** ------------------------------------------------------------
	Adding background processing requires minor changes to four files.

		The replacement to smsh2.c is 
		sol09.8a.c,
		the replacement to splitline.c is 
		sol09.8b.c,
		the replacement to execute.c is 
		sol09.8c.c, 
	the replacement to process2.c is 
	sol09.8d.c, and
	   This version of process simply passes the background
	   flag to execute for non-builtin operations

		the replacement to smsh.h is 
		sol09.8.h.

	Therefore, linking:

	cc sol09.8a.c sol09.8b.c sol09.8c.c sol09.8d.c \
		controlflow.c builtin.c varlib.c -o sol8

	will produce a shell that supports use of the & symbol to run 
	a command in the background.  The splitline replacement also 
	includes the feature to launch several commands on one line.  
	Therefore

	cmd1 & cmd2 & cmd3 ; cmd4 ; cmd5

	will work as expected.  As before, you can replace 
	controlflow.c with sol09.6.c or sol09.7.c for else support, 
	and you can replace builtin.c with sol09.5.c for exit and cd.

 ** ------------------------------------------------------------
 **
 **
 *    A version of process that accepts a wait_for_child argument
 */

#include	<stdio.h>
#include	"sol09.8.h"

/* process2.c   - version 2 - supports builtins
 * command processing layer
 * 
 * The process(char **arglist) function is called by the main loop
 * It sits in front of the execute() function.  This layer handles
 * two main classes of processing:
 * 	a) built-in functions (e.g. exit(), set, =, read, .. )
 * 	b) control structures (e.g. if, while, for)
 */

int is_control_command(char *);
int do_control_command(char **);
int ok_to_execute();
int builtin_command(char **, int *);

int process(char **args, int background)
/*
 * purpose: process user command
 * returns: result of processing command
 * details: if a built-in then call appropriate function, if not execute()
 *  errors: arise from subroutines, handled there
 */
{
	int		rv = 0;

	if ( args[0] == NULL )
		rv = 0;
	else if ( is_control_command(args[0]) )
		rv = do_control_command(args);
	else if ( ok_to_execute() )
		if ( !builtin_command(args,&rv) )
			rv = execute(args,background);
	return rv;
}

