/** sol04.18.c
 **
 *   A program to show some superblock information for an ext2 filesytem
 */
/*
 * showe2super.c
 *
 * Show superblock information for an ext2 filesystem.
 * Uses functions in the ext2fs library.  
 *
 * Compile this with
 *   gcc e2showsuper.c -lext2fs -le2p -o e2showsuper
 *
 * Usage:
 *   e2showsuper devicename
 *
 * for example:
 *   e2showsuper /dev/hda2
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ext2_fs.h"

#include "ext2fs.h"
#include "e2p.h"

const char * program_name = "e2showsuper";

static void oops(char *,char *);
static void usage(void);

int 
main(int argc, char *argv[])
{
	int		retval;
	ext2_filsys	fs;
	int		use_superblock = 0;
	int		use_blocksize = 0;
	int		force = 0;
	int		c;
	char 		*device_name;

	fprintf (stderr, "e2showsuper version 1.0\n");

	if ( argc == 1 )
		usage();

	initialize_ext2_error_table();
	device_name = argv[1];
	retval = ext2fs_open (device_name, force,
			      use_superblock, use_blocksize,
			      unix_io_manager, &fs);
	if (retval) 
		oops("Error while trying to open", device_name);

	list_super (fs->super);
	ext2fs_close (fs);
	exit (0);
}


void oops(char *msg1, char *msg2)
{
	fprintf(stderr,"Error: %s",msg1);
	perror(msg2);
	exit(1);
}
static void usage(void)
{
	fprintf (stderr, "usage: %s device\n", program_name);
	exit (1);
}
