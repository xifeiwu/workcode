/** sol03.22.c
 ** ------------------------------------------------------------
	sol03.22.c is a version
	of who2.c (from Chapter 2) that prints the time when the
	terminal line was last modified instead of the time the
	user logged in.  The solution also prints the number of
	seconds since the last modification.  You can use this
	program to see which users have been idle for a while.

 ** ------------------------------------------------------------
 *
 *     Version of who that displays time of last change to tty 
 *     instead of time of login.  Shows when the user last sent
 *     or received data
 *
 *     The format of the output is similar to that of who, except
 *     this program shows the seconds part of the time in addition
 *     to the hour and minutes.
 *
 *     This solution also prints out the number of seconds since
 *     the last change to the terminal.
 *
 *     A nice shell script to show this program in action is:
 *
 *     	while true
 *     	do
 *        	tput home
 *        	./sol03.22 | sort +5n
 *        	tput ed
 *        	sleep 2
 *      done
 *
 *	build: cc sol03.22.c -o sol03.22
 */
#include        <stdio.h>
#include        <stdlib.h>
#include        <unistd.h>
#include        <utmp.h>
#include        <fcntl.h>
#include        <time.h>
#include	<sys/types.h>
#include	<sys/stat.h>

/* #define      SHOWHOST */

void showmodtime(char *);
void show_info(struct utmp *);

int main()
{
        struct utmp     *utbufp;        /* read info into here */

	setutent();
	while( utbufp = getutent() )
                show_info( utbufp );
        return 0;
}
/*
 *      show info()
 *                      displays the contents of the utmp struct
 *                      in human readable form
 *                      * displays nothing if record has no user name
 */
void show_info( struct utmp *utbufp )
{
        if ( utbufp->ut_type != USER_PROCESS )
                return;

        printf("%-8.8s", utbufp->ut_name);      /* the logname  */
        printf(" ");                            /* a space      */
        printf("%-8.8s", utbufp->ut_line);      /* the tty      */
        printf(" ");                            /* a space      */
        showmodtime( utbufp->ut_line );         /* tty modtime  */
#ifdef SHOWHOST
        if ( utbufp->ut_host[0] != '\0' )
                printf(" (%s)", utbufp->ut_host);/* the host    */
#endif
        printf("\n");                          /* newline      */
}

void showmodtime( char *linename )
/*
 *   Uses stat to find the mtime for /dev/linename then
 *   displays that time in ctime format, using seconds, too
 */
{
        char        *devname;       /* to hold address of time      */
	struct stat info;	    /* to hold file info about tty  */
	char	    *cp;	    /* for ctime string             */
	time_t      now;

	/*
	 * build name by prepending "/dev/"
	 */
	devname = (char *)malloc(strlen(linename) + strlen("/dev/") + 1 );
	if ( devname == NULL ){
		perror("out of memory");
		exit(2);
	}

	/*
	 * stat that name
	 */
	sprintf(devname, "/dev/%s", linename);
	if ( stat(devname, &info) == -1 ){
		printf("%s: no such device", devname);
		return;
	}

        cp = ctime(&info.st_mtime);     /* convert time to string       */
                                        /* string looks like            */
                                        /* Mon Feb  4 00:46:40 EST 1991 */
                                        /* 0123456789012345678          */
        printf("%15.15s", cp+4 );       /* pick 12 chars from pos 4     */

	/*
	 * Compute idle time by comparing st_mtime on device to
	 * the current time and printing the result in seconds.
	 * An improvement would be to present the idle time in
	 * hours, minutes, and seconds, maybe even days..
	 */
	time( &now );
	printf(" %6d", now-info.st_mtime);
}
