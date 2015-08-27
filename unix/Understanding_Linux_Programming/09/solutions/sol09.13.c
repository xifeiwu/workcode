/** sol09.13.c
 ** ------------------------------------------------------------
	A version of splitline.c that handles quoted arguments is
	sol09.13.c.
	This file can be linked with

	cc sol09.8a.c sol09.13.c sol09.8c.c sol09.8d.c sol09.7.c \
		sol09.5.c varlib.c -o sol09.13

	to produce a shell with all the features in the solutions 
	presented here.

	Note that this version does not allow you to quote quotes.  
	That is, there is no way to put " in a string.  To add that 
	feature you need to add backslash quoting and/or single quotes 
	to quote double quotes.  That increases the complexity of the code.

 ** ------------------------------------------------------------
 **
 **
 *    A version of splitline.c that allows quoted arguments and still
 *    handles \n and ; and & correctly.  The next_cmd function ignores
 *    \n and ; and & inside quotes.  And splitline then parses them
 *    correctly.  A team effort.
 *
 *    A version of splitline.c that splits on '\n', ';', and '&'
 *    The next_cmd function has a new parameter: a pointer to the
 *    termination character.
 *
 *    A version of splitline.c with a revised next_cmd.  This next_cmd
 *    breaks on ';'s and keeps track if the last cmd ended with a ';'
 *    If it did, then no prompt is printed.
 *
 * splitline.c - commmand reading and parsing functions for smsh
 *    
 *    char *next_cmd(char *prompt, FILE *fp) - get next command
 *    char **splitline(char *str);           - parse a string
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	"sol09.8.h"

char * next_cmd(char *prompt, FILE *fp, int *termcp)
/*
 * purpose: read next command line from fp
 * returns: dynamically allocated string holding command line
 *  errors: NULL at EOF (not really an error)
 *          calls fatal from emalloc()
 *   notes: allocates space in BUFSIZ chunks.  
 */
{
	char	*buf ; 				/* the buffer		*/
	int	bufspace = 0;			/* total size		*/
	int	pos = 0;			/* current position	*/
	int	c;				/* input char		*/
	int	inquote = 0;			/* a boolean		*/

	static  char prevterm = '\n';		/* terminator for prev	*/
						/* command		*/

	if ( prevterm == '\n' )
		printf("%s", prompt);			/* prompt user	*/
	while( ( c = getc(fp)) != EOF ) {

		/* need space? */
		if( pos+1 >= bufspace ){		/* 1 for \0	*/
			if ( bufspace == 0 )		/* y: 1st time	*/
				buf = emalloc(BUFSIZ);
			else				/* or expand	*/
				buf = erealloc(buf,bufspace+BUFSIZ);
			bufspace += BUFSIZ;		/* update size	*/
		}

		/* toggle quoted strings */
		if ( c == '"' )
			inquote = !inquote;

		/* end of command? */
		if ( !inquote && (c == '\n' || c == ';' || c == '&') ){
			*termcp  = c;
			prevterm = c;
			break;
		}

		/* no, add to buffer */
		buf[pos++] = c;
	}
	if ( c == EOF && pos == 0 )		/* EOF and no input	*/
		return NULL;			/* say so		*/
	buf[pos] = '\0';
	return buf;
}

/**
 **	splitline ( parse a line into an array of strings )
 **/
#define	is_delim(x) ((x)==' '||(x)=='\t')

char ** splitline(char *line)
/*
 * purpose: split a line into array of white-space separated tokens
 * returns: a NULL-terminated array of pointers to copies of the tokens
 *          or NULL if line if no tokens on the line
 *  action: traverse the array, locate strings, make copies
 *    note: strtok() could work, but we may want to add quotes later
 *    note: this version handles "quoted strings  like this"
 */
{
	char	*next_token(char **cp);
	char	**args ;
	int	spots = 0;			/* spots in table	*/
	int	bufspace = 0;			/* bytes in table	*/
	int	argnum = 0;			/* slots used		*/
	char	*cp = line;			/* pos in string	*/

	if ( line == NULL )			/* handle special case	*/
		return NULL;

	args     = emalloc(BUFSIZ);		/* initialize array	*/
	bufspace = BUFSIZ;
	spots    = BUFSIZ/sizeof(char *);

	while( *cp != '\0' )
	{
		while ( is_delim(*cp) )		/* skip leading spaces	*/
			cp++;
		if ( *cp == '\0' )		/* quit at end-o-string	*/
			break;

		/* make sure the array has room (+1 for NULL) */
		if ( argnum+1 >= spots ){
			args = erealloc(args,bufspace+BUFSIZ);
			bufspace += BUFSIZ;
			spots += (BUFSIZ/sizeof(char *));
		}

		/* extract next token from string and modify cp */
		args[argnum++] = next_token(&cp);
	}
	args[argnum] = NULL;
	return args;
}
/*
 * purpose:  extract next token from string pointed to by *cpp
 * returns:  a malloc'd string 
 * effects:  modifies *cpp to point to spot after token
 * details:  handles quotes correctly
 */
char *
next_token(char **cpp)
{
	char	*newstr();
	char	*cp = *cpp;	/* a pointer to travel string */
	char	*dest;
	char	*rv;		/* the new token	      */
	int	len, inquote;

	len = inquote = 0;

	/* first figure out how long the token is (without the quotes) */
	while( *cp != '\0' && ( inquote || !is_delim(*cp) ) ){
		if ( *cp == '"' )
			inquote = !inquote;
		else
			len++;
		cp++;
	}
	/* then make an empty string to hold the token */
	dest = rv = newstr("", len);

	/* finally, copy the token into the string */
	cp      = *cpp;
	inquote = 0;
	while( *cp != '\0' && ( inquote || !is_delim(*cp)) ){
		if ( *cp == '"' )
			inquote = !inquote;
		else
			*dest++ = *cp;
		cp++;
	}

	/* wrap up */
	*dest = '\0';
	*cpp = cp;
	return rv;
}

/*
 * purpose: constructor for strings
 * returns: a string, never NULL
 */
char *newstr(char *s, int l)
{
	char *rv = emalloc(l+1);

	rv[l] = '\0';
	strncpy(rv, s, l);
	return rv;
}

void 
freelist(char **list)
/*
 * purpose: free the list returned by splitline
 * returns: nothing
 *  action: free all strings in list and then free the list
 */
{
	char	**cp = list;
	while( *cp )
		free(*cp++);
	free(list);
}

void * emalloc(size_t n)
{
	void *rv ;
	if ( (rv = malloc(n)) == NULL )
		fatal("out of memory","",1);
	return rv;
}
void * erealloc(void *p, size_t n)
{
	void *rv;
	if ( (rv = realloc(p,n)) == NULL )
		fatal("realloc() failed","",1);
	return rv;
}

