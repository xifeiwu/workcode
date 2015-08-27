/** sol02.13b.c
 ** ------------------------------------------------------------

	(b)
	A modified version of utmplib.c is 
	sol02.13b.c.
	This version contains two new functions, one to seek to a new
	position in a utmp file, and one to write a record to the file.

 ** ------------------------------------------------------------
 *
 *    modified version of utmplib.c
 *    This version includes utmp_seek() and utmp_write()
 *
 *	build: cc -c sol02.13b.c 
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
 *		utmp_seek( int record_offset, int base )  - move position
 *		utmp_write(struct utmp* recp )            - write to file
 *
 *      reads NRECS per read and then doles them out from the buffer
 */
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
        fd_utmp = open( filename, O_RDWR );             /* open it      */
        cur_rec = num_recs = 0;                         /* no recs yet  */
        return fd_utmp;                                 /* report       */
}

struct utmp *utmp_next()
{
        struct utmp *recp;

        if ( fd_utmp == -1 )                            /* error ?      */
                return NULLUT;
        if ( cur_rec==num_recs && utmp_reload()==0 )    /* any more ?   */
                return NULLUT;
                                        /* get address of next record    */
        recp = ( struct utmp *) &utmpbuf[cur_rec * UTSIZE];
	cur_rec++;
        return recp;
}

int utmp_reload()
/*
 *      read next bunch of records into buffer
 */
{
        int     amt_read;
                                                /* read them in         */
        amt_read = read( fd_utmp , utmpbuf, NRECS * UTSIZE );   

        num_recs = amt_read/UTSIZE;		/* how many did we get? */
                                                /* reset pointer        */
        cur_rec  = 0;
        return num_recs;
}

utmp_close()
{
        if ( fd_utmp != -1 )                    /* don't close if not   */
                close( fd_utmp );               /* open                 */
}

/*
 * utmp_seek(recoffet, base)
 *   adjust internal data so the next call to utmp_next() will return
 *   a utmp record at recoffset relative to base (just like lseek)
 *
 *   Returns new offset measured in utmprecs or -1 for an error
 *
 * The simplest solution is to do a real lseek to that offset and
 * adjust cur_rec to force a reload at next call.
 *
 * It might seem extreme to invalidate the entire buffer, but the
 * caller may plan to write data to the file.  It is safer to allow
 * that write to take place and then let the next call reload from
 * disk with current data.  
 *
 * The tricky part of this function is the SEEK_CUR option.
 * We cannot use SEEK_CUR with the request from the caller because
 * the current position is REALLY after the buffer, not after the
 * current record.  Therefore we adjust the rec_offset value to
 * make it work.
 */
int 
utmp_seek(int rec_offset, int base)
{
	off_t newpos;
	int   rv;

	if ( base == SEEK_CUR )
		rec_offset += ( cur_rec - num_recs );
	newpos = lseek(fd_utmp, (off_t) rec_offset*UTSIZE, base);
	if( newpos == -1 )
		return -1;
	cur_rec = num_recs ;			/* will force reload */
	return  newpos/UTSIZE;
}
/*
 * function: utmp_write
 *   Writes record at current position in utmp file
 *   Note: you must call utmp_seek() before calling utmp_write
 *         or else the position for the written data will be
 *         after the buffer, not after the record.
 *   This example shows the problems of mixing reads and writes
 *   for buffered I/O.
 *   Returns -1 on error, 1 for success
 */
int
utmp_write( struct utmp *recp )
{
	if ( write( fd_utmp, recp, UTSIZE ) != UTSIZE )
		return -1;
	return 1;
}
