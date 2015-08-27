/** sol14.18s.c
 ** ------------------------------------------------------------
	A curses server is coded here in
	sol14.18s.c

	This datagram server receives screen requests at
	a port specified on the command line.  The
	requets are strings of the form

	CLEAR
	DRAW row col string
	SIZE -> sends back LINES COLS as two string
	QUIT

	Note: this program does not read any input from the
	terminal.  We assume other programs act as the input
	side of the application.

	Note: the row and col may be negative.  In that case
	they indicate an offset from the bottom of the screen
	and right edge, respectively.

	Note:  this runs in the background for convenience

	Todo: add SIGWINCH handling

 */
/************************************************************************
 * sol14.18s  - a datagram-based text display server
 * 	         usage: sol14.18s portnum 
 * 	                note: sol14.18c defaults to port 2828 
 * 	        action: listens at the specfied port and controls screen
 * 	         notes:  writes narration to stderr 
 * 	                    run as ./sol14.18s 2> serverlog
 * 	                    or     ./sol14.18s 2> /dev/null
 *
 * build: cc sol14.18s.c dgram.c -lcurses -o sol14.18s
 * 		
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<curses.h>
#include	<signal.h>

#define oops(m,x)  { perror(m);exit(x);}

int  make_dgram_server_socket(int);
int  get_internet_address(char *, int,  int *, struct sockaddr_in *);
void say_who_called(struct sockaddr_in *);
void process_request(char *, struct sockaddr *, int);
void do_quit(int);

int main(int ac, char *av[])
{
	int	port;			/* use this port		*/
	int	sock;			/* for this socket		*/
	char	buf[BUFSIZ];		/* to receive data here		*/
	size_t	msglen;			/* store its length here	*/
	struct  sockaddr_in   saddr;	/* put sender's address here	*/
	socklen_t saddrlen;		/* and its length here		*/
	char	*me;

	me = av[0];
	if ( ac == 1 || (port = atoi(av[1])) <= 0 ){
		fprintf(stderr,"usage: %s portnumber\n", me);
		exit(1);
	}

	if ( fork() != 0 )
		return 0;

    /*  get a socket and assign it a port number */

	if( (sock = make_dgram_server_socket(port)) == -1 )
		oops("cannot make socket",2);

    /* initialize curses */

	initscr();
	clear();
	refresh();

    /* signal handling: curses handles INT and QUIT  */

	signal(SIGHUP, do_quit);

    /* receive messaages on that socket */

	saddrlen = sizeof(saddr);
	while( (msglen = recvfrom(sock,buf,BUFSIZ,0,
				(struct sockaddr*)&saddr,&saddrlen))>0 ) {
		buf[msglen] = '\0';
		fprintf(stderr,"screenserv: got a message: %s\n", buf);
		say_who_called(&saddr);
		process_request(buf,(struct sockaddr *)&saddr,sock);
	}
	return 0;
}
void say_who_called(struct sockaddr_in *addrp)
{
	char	host[BUFSIZ];
	int	port;

	get_internet_address(host,BUFSIZ,&port,addrp);
	fprintf(stderr,"  from: %s:%d\n", host, port);
}

/*
 * process_request(char *cmd)
 *   purpose: act on the command sent by a client
 *   details: commands supported are
 *            CLEAR        - calls clear()
 *            DRAW r c str - calls mvaddstr(r,c,str);
 *            SIZE	   - return LINES COLS to caller
 *	      QUIT         - calls endwin, exit
 */
void process_request(char *cmd, struct sockaddr *callerp,int sock)
{
	if ( strncasecmp(cmd, "CLEAR", 5) == 0 ){
		clear();
		refresh();
	}
	else if ( strncasecmp(cmd,"DRAW ",4) == 0 )
		do_draw(cmd+5);
	else if ( strncasecmp(cmd,"QUIT",4) == 0 )
		do_quit(0);
	else if ( strncasecmp(cmd,"SIZE",4) == 0 )
		do_size(callerp, sock);
}

do_draw( char *cmd )
{
	char	 *cp = cmd;
	int	row, col;

	row = atoi(cp);			/* get first arg */
	if ( row < 0 )			/* rel to bot    */
		row += LINES;
	cp = strchr(cp,' ');		/* skip to space */
	if ( !cp )
		return;
	col = atoi(++cp);		/* get col arg	 */
	if ( col < 0 )			/* rel to right  */
		col += COLS;
	cp = strchr(cp,' ');		/* skip to space */
	if ( !cp )
		return;

	mvaddstr(row,col,cp+1);
	refresh();
}

/*
 * do_size(struct sockaddr *addrp)
 *  purpose: send back size of the display to the caller
 *  details: string is lines cols with one space between them and a newline at the end
 */
do_size(struct sockaddr *addrp, int sock)
{
	char	msg[100];
	int	ret;

	sprintf(msg,"%d %d\n", LINES, COLS);
	ret = sendto(sock, msg, strlen(msg),0, addrp, sizeof(*addrp));
}
/*
 * do_quit
 * cleanup
 */
void do_quit(int s)
{
	clear();
	refresh();
	endwin();
}

