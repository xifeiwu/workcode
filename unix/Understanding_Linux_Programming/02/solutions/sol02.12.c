/** sol02.12.c
 ** ------------------------------------------------------------
	A modified version of utmplib.c is:
	sol02.12.c.
	This change to utmplib.c affects who3.c in a simple way: The
	two lines that check to see if the record represents a login
	session are no longer needed.  Is this change a good thing?
	No.  utmplib.c is has lost some generality, and who3.c is not
	significantly simpler.

 ** ------------------------------------------------------------
 *
 *   a modified version of utmplib.c
 *   This version only returns records that represent logged-in users.
 *   Enable this behavior with   #define ACTIVE_ONLY
 *
 *	build: cc -c sol02.12.c 
 */
/*
 * utmplib.c  - functions to buffer reads from utmp file 
 *
 *      functions are
 *              utmp_open( filename )   - open file
 *                      returns -1 on error
 *              utmp_next( )            - return pointer to next struct
 *                      returns NULL on eof
 *              utmp_close()            - close file
 *
 *      reads NRECS per read and then doles them out from the buffer
 *
 * change: only return records that represent logged in users
 *         set by #define ACTIVE_ONLY
 */

#define	ACTIVE_ONLY
#include        <stdio.h>
#include        <fcntl.h>
#include        <sys/types.h>
#include        <utmp.h>

#define NRECS   16
#define NULLUT  ((struct utmp *)NULL)
#define UTSIZE  (sizeof(struct utmp))

static  char    utmpbuf[NRECS * UTSIZE];                /* storage      */
static  int     num_recs;                               /* num stored   */
static  int     cur_rec;                                /* next to go   */
static  int     fd_utmp = -1;                           /* read from    */

utmp_open( char *filename )
{
        fd_utmp = open( filename, O_RDONLY );           /* open it      */
        cur_rec = num_recs = 0;                         /* no recs yet  */
        return fd_utmp;                                 /* report       */
}

struct utmp *utmp_next()
{
        struct utmp *recp;

        if ( fd_utmp == -1 )                            /* error ?      */
                return NULLUT;

	/*
	 * loop until a USER_PROCESS record shows up
	 */
	while(1)
	{
		if ( cur_rec==num_recs && utmp_reload()==0 )   /* any more ? */
			return NULLUT;
                                        /* get address of next record    */
		recp = ( struct utmp *) &utmpbuf[cur_rec * UTSIZE];
		cur_rec++;
#ifdef ACTIVE_ONLY
		if ( recp->ut_type == USER_PROCESS )
#endif
			return recp;
	}
}

int utmp_reload()
/*
 *      read next bunch of records into buffer
 */
{
        int     amt_read;

                                                /* read them in         */
        amt_read = read( fd_utmp , utmpbuf, NRECS * UTSIZE );   

                                                /* how many did we get? */
        num_recs = amt_read/UTSIZE;
                                                /* reset pointer        */
        cur_rec  = 0;
        return num_recs;
}

utmp_close()
{
        if ( fd_utmp != -1 )                    /* don't close if not   */
                close( fd_utmp );               /* open                 */
}
