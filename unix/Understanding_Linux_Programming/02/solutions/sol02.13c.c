/** sol02.13c.c
 ** ------------------------------------------------------------

	A modified version of logout_tty is
	sol02.13c.c.

 ** ------------------------------------------------------------
 *
 *    modified version of logout_tty()
 *    This version uses utmp_seek() and utmp_write()
 *
 *	build: cc -c sol02.13c.c 
 */
/*
 * logout_tty(char *line)
 *   marks a utmp record as logged out
 *   does not blank username or remote host
 *   returns -1 on error, 0 on success
 */

#include	<stdio.h>
#include	<utmp.h>
#include	<sys/types.h>
#include	<unistd.h>

struct utmp *utmp_next();

int logout_tty(char *line)
{
    struct utmp *recp;
    int         retval = -1 ;                      /* pessimism */

    if ( utmp_open() == -1 )
        return -1;

    /* search and replace */
    while ( ( recp = utmp_next() ) != NULL )
        if ( strncmp(recp->ut_line, line, sizeof(recp->ut_line)) == 0)
        {
            recp->ut_type = DEAD_PROCESS;              /* set type */
            if ( time( &recp->ut_time ) != -1 )        /* and time */
                if ( utmp_seek(-1, SEEK_CUR)!= -1 )   /* back up  */
                    if ( utmp_write( &recp ) == 1 )    /* update   */
                        retval = 0;                        /* success! */
            break;
        }

    /* close the file */
    if ( utmp_close() == -1 )
        retval = -1;
    return retval;
}
