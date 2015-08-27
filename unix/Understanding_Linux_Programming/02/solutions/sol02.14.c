/** sol02.14.c
 ** ------------------------------------------------------------
	The complete set of fields in a utmp struct varies from
	one version of Unix to another.  The solution here is
	based on the struct used on a Linux 2.2.14 system.
	sol02.14.c.

 ** ------------------------------------------------------------
 *
 *    See description in next comment 
 *
 *	build: cc sol02.14.c -o sol02.14
 */

#include	<stdio.h>
#include	<sys/types.h>
#include	<utmp.h>

/*
 * dumputmp.c
 *   purpose
 *      reads from any file containing utmp records and prints
 *      the records in text format to standard output
 *   usage
 *      dumputmp [filename|-size]
 *   action
 *      reads from default utmp file or from named file
 *      if argument is "-size", it prints the size of a utmp record
 */

main(int ac, char *av[])
{
	if ( ac == 1 )
		dumpfile( UTMP_FILE );
	else
		dumpfile( av[1] );
}

dumpfile( char *fn )
/*
 * open file and dump records
 */
{
	struct utmp	utrec;
	int		fd;

	if ( strcmp(fn,"-size") == 0 ){
		printf("%d\n", sizeof(utrec));
		return;
	}
	fd = open( fn, 0 );
	if ( fd == -1 )
	{
		perror( fn );
		return ;
	}
	while( read( fd, &utrec, sizeof(utrec) ) == sizeof(utrec) )
		show_utrec(&utrec);
	close( fd );
}

show_utrec( struct utmp *rp )
{
	char	*typename();
	
	printf("%-*s ", UT_NAMESIZE, rp->ut_user );
	printf("%-*s ", UT_LINESIZE, rp->ut_line );
	printf("%12d ", rp->ut_time );
	printf("%6d ", rp->ut_pid);

	printf("%4d %-12.12s ", rp->ut_type , typename(rp->ut_type) );
	/*
	 * dead processes have an exit status recorded in a struct
	 * I don't get meaningful numbers here.  I think it is not
	 * used.
	 */
	if ( rp->ut_type == DEAD_PROCESS ){
		int killed_by = 0xFF & (rp->ut_exit).e_termination;
		int left_with = 0xFF & (rp->ut_exit).e_exit;

		printf("[%4d,%4d] ", killed_by, left_with );
	}
	else
		printf("[%9s] ",   "not dead");

	printf("%10d ", rp->ut_session );
	printf("%s", rp->ut_host );
	putchar('\n');
}

char *uttypes[] = {  	"EMPTY", "RUN_LVL", "BOOT_TIME", "OLD_TIME", 
			"NEW_TIME", "INIT_PROCESS", "LOGIN_PROCESS", 
			"USER_PROCESS", "DEAD_PROCESS", "ACCOUNTING"
	};

char *
typename( typenum )
{
	return uttypes[typenum];
}
