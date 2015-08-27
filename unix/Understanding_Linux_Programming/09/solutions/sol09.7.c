/** sol09.7.c
 ** ------------------------------------------------------------
	A version of controlflow.c that supports the else part of the
	if..then control structure by using one state variable rather than
	two is
	sol09.7.c.

	This file can be linked with

	 cc smsh2.c splitline.c execute.c process2.c sol09.7.c \
	 	builtin.c varlib.c -o sol7

	to produce a shell that allows else in if structures with a 
	different implementation than the one in solution 6.  
	If you replace splitline.c with sol09.4.c, you can add 
	multiple commands on one line, and if you replace 
	builtin.c with sol09.5.c, you can add cd and exit.

 ** ------------------------------------------------------------
 **
 **
 *   controlflow.c with else included and changed to use one state variable
 *
 * "if" processing is done with one state variable
 */
#include	<stdio.h>
#include	<string.h>
#include	"smsh.h"

enum states   { NEUTRAL, IF_SUCCEEDED, IF_FAILED, SKIPPING_THEN,
		DOING_THEN, SKIPPING_ELSE, DOING_ELSE };

static int if_state  = NEUTRAL;
static int last_stat = 0;

int	syn_err(char *);

int ok_to_execute()
/*
 * purpose: determine the shell should execute a command
 * returns: 1 for yes, 0 for no
 * details: if in THEN_BLOCK and if_result was SUCCESS then yes
 *          if in THEN_BLOCK and if_result was FAIL    then no
 *          if in WANT_THEN  then syntax error (sh is different)
 */
{
	int	rv = 1;		/* default is positive */

	if ( if_state == IF_SUCCEEDED || if_state == IF_FAILED ){
		syn_err("then expected");
		rv = 0;
	}
	else if ( if_state == SKIPPING_THEN || if_state == SKIPPING_ELSE )
		rv = 0;
	return rv;
}

int is_control_command(char *s)
/*
 * purpose: boolean to report if the command is a shell control command
 * returns: 0 or 1
 */
{
    return ( strcmp(s,"if")==0 || strcmp(s,"then")==0 ||
	     strcmp(s,"fi")==0 || strcmp(s,"else")==0  );
}


int do_control_command(char **args)
/*
 * purpose: Process "if", "then", "fi" - change state or detect error
 *    also: process "else"
 * returns: 0 if ok, -1 for syntax error
 *   notes: I would have put returns all over the place, Barry says "no"
 */
{
	char	*cmd = args[0];
	int	rv = -1;

	if( strcmp(cmd,"if")==0 ){
		if ( if_state != NEUTRAL )
			rv = syn_err("if unexpected");
		else {
			last_stat = process(args+1);
			if_state = (last_stat==0 ? IF_SUCCEEDED : IF_FAILED );
			rv = 0;
		}
	}
	else if ( strcmp(cmd,"then")==0 ){
		if ( if_state == IF_SUCCEEDED ){
			if_state = DOING_THEN;
			rv = 0;
		}
		else if ( if_state == IF_FAILED ){
			if_state = SKIPPING_THEN;
			rv = 0;
		}
		else 
			rv = syn_err("then unexpected");
	}
	else if ( strcmp(cmd,"fi")==0 ){
		if ( if_state != DOING_THEN && if_state != SKIPPING_THEN 
		     && if_state != DOING_ELSE && if_state != SKIPPING_ELSE )
			rv = syn_err("fi unexpected");
		else {
			if_state = NEUTRAL;
			rv = 0;
		}
	}
	else if ( strcmp(cmd,"else")==0 ){
		if ( if_state == DOING_THEN ){
			if_state = SKIPPING_ELSE;
			rv = 0;
		}
		else if ( if_state == SKIPPING_THEN ){
			if_state = DOING_ELSE;
			rv = 0;
		}
		else
			rv = syn_err("else unexpected");
	}
	else 
		fatal("internal error processing:", cmd, 2);
	return rv;
}

int syn_err(char *msg)
/* purpose: handles syntax errors in control structures
 * details: resets state to NEUTRAL
 * returns: -1 in interactive mode. Should call fatal in scripts
 */
{
	if_state = NEUTRAL;
	fprintf(stderr,"syntax error: %s\n", msg);
	return -1;
}
