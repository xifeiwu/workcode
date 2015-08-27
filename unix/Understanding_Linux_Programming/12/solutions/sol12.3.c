/** sol12.3.c
 ** ------------------------------------------------------------
	The revised generic server is
	sol12.3.c.

 ** ------------------------------------------------------------
 **
 **
 *   generic server modified to handle EINTER for accept
 *

 */

#include	<stdio.h>
#include	<errno.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	"socklib.h"

int main()
{
	int sock, fd;
	int port;

	sock = make_server_socket(port);
	if ( sock == -1 )
		exit(1);

	while( 1 ){
		fd = accept(sock, NULL, NULL);
		if ( fd == -1 )
			if ( errno == EINTR )
				continue;
			else
				break;
		process_request(fd);
		close(fd);
	}
	return 0;
}
