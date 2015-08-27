/** sol05.22.c
 ** ------------------------------------------------------------
	An enhanced version of write0.c is:
	sol05.22.c.
	This solution shows how to use the standard getutent() function 
	instead of our own utmplib functions.  This solution also shows
	how to get information about the current user session 
	such as hostname, username, and terminal name.

 ** ------------------------------------------------------------
 **
 **
 *  Enhanced version of write0.c
 *
 *  This version accepts a username on the command line
 *  then searches the utmp file for the first line on which
 *  that user is found.  This program skips the line for the
 *  calling process.  If more than one other tty is found, 
 *  the program says there is more than one choice.
 *
 *	purpose: send messages to another terminal
 *	 method: open the other terminal for output then
 *	 	 copy from stdin to that terminal
 *	  shows: a terminal is just a file supporting regular i/o
 *	  usage: write0 {ttyname|username}
 *
	build: cc sol05.22.c -o sol05.22
 */


#include	<stdio.h>
#include	<fcntl.h>
#include	<unistd.h>
#include	<utmp.h>
#include	<pwd.h>
#include	<sys/types.h>
#include	<stdlib.h>
#include	<sys/stat.h>
#include	<time.h>

main( int ac, char *av[] )
{
	int	fd;
	char	buf[BUFSIZ];
	char	*termname, *get_termname(char *);

	/* check args */
	if ( ac != 2 ){
		fprintf(stderr,"usage: write0 {ttyname|username}\n");
		exit(1);
	}

	/* find user or terminal or quit */
	if ( (termname = get_termname(av[1])) == NULL )
		exit(1);

	/* open devices */
	fd = open( termname, O_WRONLY );
	if ( fd == -1 ){
		perror(termname); 
		exit(1);
	}

	/* say hello */
	hello(fd);

	/* loop until EOF on input */
	while( fgets(buf, BUFSIZ, stdin) != NULL )
		if ( write(fd, buf, strlen(buf)) == -1 )
			break;
	close( fd );
}

/*
 * say hello
 */
hello(int fd)
{
	char	       host[256];
	struct passwd  *pwd;
	time_t         now;
	char	       greeting[1024];

	/* gather all the data we need to build the greeting */
	if ( gethostname(host, 256) == -1 )
		strcpy(host,"unknown");

	pwd = getpwuid(getuid());
	time(&now);

	sprintf(greeting,
		"Message from %s@%s on %s at %5.5s ...\n",
			pwd->pw_name, host,
			ttyname(0)+strlen("/dev/"),
			ctime(&now)+11
	);
	write(fd, greeting, strlen(greeting));
}

/*
 * If s is a file in /dev, then use that name.
 * Otherwise search utmp for entries with that user.
 * Find the line from that entry.  Skip lines that
 * match ttyname() (i.e. the caller)
 * If there are more than one, say so but use the first one.
 */

char *
get_termname(char *s)
{
	char	*devname, *is_a_device(char *);
	char	*find_user(char *);

	/* check "s" and "/dev/s" 	*/
	if ( devname = is_a_device(s) )
		return devname;

	/* search for s as username	*/
	devname = find_user(s);

	return devname;
}

/*
 * try the name flat out, then try it with /dev prepended
 * If so, then return the string, else return NULL
 */

char *
is_a_device(char *s)
{
	char	*rv;
	char	*str_add(char*, char*);

	if ( isacdev(s) )		/* try string as is 	*/
		return s;

	rv = str_add("/dev/", s);	/* then try "/dev/s"	*/
	if ( isacdev(rv) )
		return rv;
	return NULL;
}


isacdev(char *s)
{
	struct stat info;

	return( stat(s,&info) != -1 && S_ISCHR(info.st_mode) );
}


/*
 * concatenate two strings into new storage
 * Warning: this function does not always return heap memory
 */
char *
str_add(char *s1, char *s2)
{
	char *rv;
	if ( s1 == NULL )
		return s2;
	if ( s2 == NULL )
		return s1;

	rv = (char *)malloc(1 + strlen(s1) + strlen(s2) );
	if ( rv == NULL ){
		perror("out of memory");
		exit(1);
	}
	sprintf(rv,"%s%s", s1, s2);
	return rv;
}

/*
 * Look through utmp (using getutid()) for a line with the
 * specified user as ut_user.  Skip current tty and report
 * if more than one.  Also, skip terminals with names that
 * start with ':'; those are displays on an X server.
 */

char *
find_user(char *user)
{
	struct utmp *entryp;
	char	*result = NULL;		/* result dev	  */
	int	num = 0;		/* # of matches	  */

	char	*mytty = ttyname(0);	/* begins "/dev/" */
	char	*ttydev = mytty + strlen("/dev/");

	setutent();			/* open utmp	  */
	while( entryp = getutent() )
	{
		/* if user matches and tty is not ours */
		if ( entryp->ut_type == USER_PROCESS 
			&& strcmp(entryp->ut_user,user)==0 
			&& strcmp(entryp->ut_line,ttydev)!=0 
			&& entryp->ut_line[0] != ':' 
			&& is_a_device(entryp->ut_line) 
		   )
		{
			num++;
			if ( num == 1 )
				result = str_add("/dev/", entryp->ut_line );
		}
	}
	endutent();

	if ( num > 1 )
		printf("Warning: %s is logged in %d times, using %s\n",
			user, num, result);
	return result;
}

