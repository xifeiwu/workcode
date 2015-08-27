/** sol09.5.c
 ** ------------------------------------------------------------
	A version of builtin.c that supports the exit command and
	also the cd command is
	sol09.5.c.

	This file can be linked with 

	 cc smsh2.c splitline.c execute.c process2.c controlflow.c \
	 	sol09.5.c varlib.c -o sol5

	to produce a shell that accepts exit and cd.  The cd command
	is part of the answer to problem 9.11.  

	By replacing splitline.c with sol09.4.c, you can produce a 
	shell that also supports multiple commands on a line.

 ** ------------------------------------------------------------
 **
 **
 *  A revised version of builtin.c that supports exit and cd
 *
 * builtin.c
 * contains the switch and the functions for builtin commands
 */

#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<ctype.h>
#include	"smsh.h"
#include	"varlib.h"

int assign(char *);
int okname(char *);
int numeric(char *);

int builtin_command(char **args, int *resultp)
/*
 * purpose: run a builtin command 
 * returns: 1 if args[0] is builtin, 0 if not
 * details: test args[0] against all known builtins.  Call functions
 */
{
	int rv = 0;

	if ( strcmp(args[0],"set") == 0 ){	     /* 'set' command? */
		VLlist();
		*resultp = 0;
		rv = 1;
	}
	else if ( strchr(args[0], '=') != NULL ){   /* assignment cmd */
		*resultp = assign(args[0]);
		if ( *resultp != -1 )		    /* x-y=123 not ok */
			rv = 1;
	}
	else if ( strcmp(args[0], "export") == 0 ){
		if ( args[1] != NULL && okname(args[1]) )
			*resultp = VLexport(args[1]);
		else
			*resultp = 1;
		rv = 1;
	}
	else if ( strcmp(args[0], "exit") == 0 ){
		int	exitval = 0;
		if ( args[1] == NULL || numeric(args[1]) ){
			if ( args[1] )
				exitval = atoi(args[1]);
			exit(exitval);
		}
		/* falls here is args[1] is not null and not numeric */
		fprintf(stderr,"error: %s is not numeric\n", args[1]);
		*resultp = -1;
		rv = 1;
	}
	else if ( strcmp(args[0],"cd") == 0 ){
		char *where = args[1];
		if ( args[1] == NULL )		/* cd alone => cd $HOME */
			where = VLlookup("HOME");
		if ( where == NULL || !*where ){
			fprintf(stderr,"cd: HOME not set\n");
			*resultp = -1;
		}
		else {
			*resultp = chdir(where);
			if ( *resultp == -1 )
				perror(where);
		}
		rv = 1;
	}
	return rv;
}

int assign(char *str)
/*
 * purpose: execute name=val AND ensure that name is legal
 * returns: -1 for illegal lval, or result of VLstore 
 * warning: modifies the string, but retores it to normal
 */
{
	char	*cp;
	int	rv ;

	cp = strchr(str,'=');
	*cp = '\0';
	rv = ( okname(str) ? VLstore(str,cp+1) : -1 );
	*cp = '=';
	return rv;
}
int okname(char *str)
/*
 * purpose: determines if a string is a legal variable name
 * returns: 0 for no, 1 for yes
 */
{
	char	*cp;

	for(cp = str; *cp; cp++ ){
		if ( (isdigit(*cp) && cp==str) || !(isalnum(*cp) || *cp=='_' ))
			return 0;
	}
	return ( cp != str );	/* no empty strings, either */
}

int numeric(char *str)
/*
 * purpose: determines if a string is all numeric
 * returns: 0 for no, 1 for yes
 */
{
	char	*cp;
	for (cp = str; *cp && isdigit(*cp); cp++)
		;
	return (*cp == '\0');
}
