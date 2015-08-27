/** sol03.15.c
 ** ------------------------------------------------------------
	sol03.15.c is a
	a version of ls1.c that sorts the list of items.
	It supports the -r option to sort in descending order.
	This version presents the list in a single column, but
	the code to display multiple columns is included.

 ** ------------------------------------------------------------
 **
 **
 *    ls1.c with sorting (ascending and descending)
 *
 *   purpose  list contents of directory or directories
 *   action   if no args, use .  else list files in args
 *   extra    read all names into an array that grows
 *		then sort it
 *
 *	build: cc sol03.15.c -o sol03.15
 **/

#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<dirent.h>
#include	<string.h>

#define	SCREEN_WIDTH 80

void do_ls(char [],int);

main(int ac, char *av[])
{
	int	r_flag = 0;
	int	anyfiles =  0;

	while ( --ac ){
		if ( strcmp("-r", *++av) == 0 )
			r_flag = 1;
		else {
			printf("%s:\n", *av );
			do_ls( *av , r_flag );
			anyfiles = 1;
		}
	}
	if ( !anyfiles )
		do_ls(".", r_flag);
}

void do_ls( char dirname[] , int reverse )
/*
 *	list files in directory called dirname, reverse them if
 *	reverse is true
 */
{
	DIR		*dir_ptr;		/* the directory */
	struct dirent	*direntp;		/* each entry	 */

	list_init();

	if ( ( dir_ptr = opendir( dirname ) ) == NULL )
		fatal("cannot open ", dirname);
	else
	{
		while ( ( direntp = readdir( dir_ptr ) ) != NULL )
			list_append( direntp->d_name );
		closedir(dir_ptr);
	}

	list_sort(reverse);
	list_show(1);
	list_finish();
}

/*
 * list functions here
 *  a list of names is an array that grows as needed
 */

void *emalloc(unsigned), *erealloc(void*, unsigned);
char **namelist = NULL;
int  listlen = 0;
int  taken = 0;

#define	CHUNKLEN  100

list_init()
{
	namelist = (char **) emalloc(CHUNKLEN * sizeof(char *));
	listlen = CHUNKLEN;
	taken = 0;
}

/*
 * add an item to the list, extending it if needed
 */
list_append(char *str)
{
	char	*newstring(char *);
	char	*name = newstring(str);

	if ( taken == listlen ){
		listlen += CHUNKLEN;
		namelist = (char **) erealloc(namelist,listlen);
	}
	namelist[taken++] = name ;
}

/*
 *  use qsort to sort list 
 */
list_sort(int revflag)
{
	int comp_increasing(const void *, const void *);
	int comp_decreasing(const void *, const void *);

	qsort((void *)namelist, (size_t) taken, sizeof(char *),
			(revflag?comp_decreasing:comp_increasing));
}

comp_increasing(char **p1, char **p2)
{
	return strcmp( (char*) *p1, (char *) *p2);
}

comp_decreasing(char **p1, char **p2)
{
	return strcmp( (char*) *p2, (char *) *p1);
}
/*
 * list_show: show the list in one or more columns
 *            if onecol is true, one column, otherwise
 *            do as many columns as can fit across the page
 */
list_show(int onecol)
{
	int	i;
	int	maxlen = 0;
	int	pagewidth;
	int	cols, col_len, col_wid;
	int	row, col;
	char	*name;

	if ( onecol ){
		for (i=0; i<taken; i++)
			printf("%s\n", namelist[i]);
		return 0;
	}
	/* figure out lengths and widths of columns */
	for(i=0; i<taken; i++)
		if ( strlen(namelist[i]) > maxlen )
			maxlen = strlen(namelist[i]);

	col_wid = maxlen+1;
	pagewidth = get_screen_width();
	cols = pagewidth / col_wid ;
	col_len = taken / cols ;
	if ( taken % cols )		/* for uneven cols, put the */
		col_len++;		/* extra on the left	    */

	/* print the list using these numbers */

	for( row = 0 ; row < col_len ; row++ )
		for( col = 0 ; col < cols ; col++ ){
			i = row + col*col_len ;
			if ( i < taken ){
				if ( i + col_len < taken )
					printf("%-*s",col_wid,namelist[i]);
				else
					printf("%s\n",namelist[i]);
			}
		}
}

/*
 * delete the words and the list
 */
list_finish()
{
	int	i;

	for(i = 0 ; i<taken ; i++ )
		free(namelist[i]);
	free(namelist);
	namelist = NULL;
	taken = listlen = 0 ;
}
	
/*
 * in the future, we can ask the system for the width of
 * the screen
 */
int
get_screen_width()
{
	return SCREEN_WIDTH;
}
/*
 * construct a string 
 */
char *newstring(char *s)
{
	char	*rv = emalloc( 1 + strlen(s) );
	strcpy(rv, s);
	return rv;
}

void *
emalloc(unsigned n)
{
	void *rv = malloc(n);
	if ( rv )
		return rv;
	fatal("out of memory","");
}

void *
erealloc(void *p, unsigned n)
{
	void *rv = realloc(p,n);
	if ( rv )
		return rv;
	fatal("out of memory","");
}
/*
 * report and die
 */

fatal(char *s1, char *s2)
{
	fprintf(stderr,"ls1: %s%s\n", s1, s2);
	exit(1);
}

