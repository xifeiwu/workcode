/** sol05.23.c
 ** ------------------------------------------------------------
	A version of mesg is:
	sol05.23.c.
	Students doing this project will discover that write 
	runs with set group id tty.  Discussing why that is done
	can be a useful review of the idea and purpose of the
	set user and set group id bits.  Also, notice that even
	with mesg off, a user can write to herself.

 ** ------------------------------------------------------------
 **
 **
 *  Version of mesg
 *
 *  mesg checks group write permission on the current tty.
 *  well, on what ever is at the end of fd 0, actually
 *
 *  If the argument is 'y', the bit is set, 
 *  If the argument is 'n', the bit is cleared,
 *  If no argument is provided, the state of the bit is reported

	build: cc sol05.23.c -o sol05.23
 */


#include	<stdio.h>
#include	<sys/types.h>
#include	<stdlib.h>
#include	<sys/stat.h>

main( int ac, char *av[] )
{

	struct stat settings;
	int    perms;

	if ( fstat(0, &settings) == -1 ){
		perror("Cannot stat stdin");
		exit(0);
	}
	perms = (settings.st_mode & 07777);	/* ALLPERMS in stat.h */

	/*
	 * if no args, report setting
	 */ 
	if ( ac == 1 ){
		printf("is %c\n", (perms&S_IWGRP)?'y':'n');
		exit(0);
	}

	/* 
	 * Has args - check them and update if ok
	 */

	if ( ac == 2 && av[1][0] == 'n' )
			perms &= ~S_IWGRP;
	else if ( ac == 2 && av[1][0] == 'y' )
			perms |=  S_IWGRP;
	else {
		fprintf(stderr,"usage: mesg [y|n]\n");
		exit(2);
	}
	
	if ( fchmod(0, perms) == -1 ){
		perror("Cannot change mesg status");
		exit(3);
	}

	return 0;
}
