/** sol12.4.c
 ** ------------------------------------------------------------
	A version of webserv that logs all requests and the
	status of their replies is
	sol12.4.c.
	This program accepts the name of the logfile on the command line
	(with the -l filename option) and uses O_APPEND so
	it can write to the log safely.  The program uses the fact
	that accept() records the address of the client.

 ** ------------------------------------------------------------
 **
 **
 *  webserv with new feature: logs requests and outcome of each request
 *
 *  details of log function
 *      a) Log is opened at start in O_APPEND mode to ensure that
 *         multiple children will not lose data to race conditions
 *      b) Each accept will record the ip address of the caller
 *         That address will be passed to the child.
 *      c) Each time a header is printed out, the webserver will
 *         add to the log the time, the caller's ip number,
 *         the request, and the first line of the header
 *         (note: this requires passing the callers ipaddr down to
 *                the request processing code)
 *
 *      The method is to create a serverlog() object that stores
 *      data in static variables and then can be told to write out
 *      the information.
 *
 * webserv.c - a minimal web server (version 0.3)
 *      usage: ws portnumber [ -l logfile ]
 *   features: supports the GET command only
 *             runs in the current directory
 *             forks a new child to handle each request
 *             has MAJOR security holes, for demo purposes only
 *             has many other weaknesses, but is a good start
 *
 *      build: cc sol12.4.c socklib.c -o sol12.4
 */
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<string.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<errno.h>
#include	<time.h>
#include	<ctype.h>

#include	"socklib.h"

#define	oops(s) { perror(s); exit(1);}

int	server_log(int, void *);
int	setup(int , char **);
void	read_til_crnl(FILE *);
void	process_rq( char *, int , struct in_addr );
void	header( FILE *, char *);
void	cannot_do(int );
void	do_404(char *, int );
int	isadir(char *);
int	not_exist(char *);
void	do_ls(char *, int );
char	* file_type(char *);
int	ends_in_cgi(char *);
void	do_exec( char *, int);
void	do_cat(char *, int );
int	server_log(int , void *);
int	logwrite(int , struct in_addr , char *, char *);
void	safecpy(char *, char *, int );

/* -----------------------------------------------------
 * logfile header material.  Should be in serverlog.h
 */
#define LOGMODE		0600
#define SL_OPEN		0
#define SL_ADDR		1
#define SL_REQ		2
#define SL_REPLY	3
#define	REPLY_MAX	10
#define REQUEST_MAX	2048	/* longest request allowed */


/* --- end of serverlog.h ------------ */

int main(int ac, char *av[])
{
	int 	sock, fd;
	FILE	*fpin;
	char	request[BUFSIZ];
	struct sockaddr_in clnt_addr;
	int	len;

	sock = setup(ac, av);
	/*
	 * setup - process command line arguments
	 * 
	 * returns a socket for the server
	 * handles -l logfile opt
	 *
	 */

	/* main loop here */

	while(1){
		/* wait for a call */
		len = sizeof(struct sockaddr);
		fd = accept( sock, (struct sockaddr *) &clnt_addr, &len);

		/*
		 * skip the EINTR returns from accept
		 */
		if ( fd == -1 && errno == EINTR )
			continue;
		if ( fd == -1 )
			oops("accept");

		fpin = fdopen(fd, "r" );

		/* read request */
		fgets(request,BUFSIZ,fpin);
		printf("got a call: request = %s", request);
		read_til_crnl(fpin);

		/* do what client asks */
		process_rq(request, fd, clnt_addr.sin_addr);

		fclose(fpin);
	}
	return 0;
}
int setup(int ac, char *av[])
{
	int	portnum = -1;
	char	*logfile = NULL;
	int	pos;
	int	sock;

	/* parse */

	for(pos=1; pos<ac; pos++)
	{
		if ( isdigit(av[pos][0]) )
			portnum = atoi(av[pos]);
		else if ( strcmp(av[pos], "-l") == 0 ){
			if ( ++pos < ac )
				logfile = av[pos];
			else {
				fprintf(stderr,"missing logfilename\n");
				exit(1);
			}
		}
	}

	/* act: make socket, make log object */

	if ( portnum == -1 ){
		fprintf(stderr,"usage: ws portnum\n");
		exit(1);
	}
	sock = make_server_socket( portnum );
	if ( sock == -1 ) {
		perror("socket");
		exit(2);
	}

	if ( logfile )
		server_log(SL_OPEN, logfile);

	return sock;
}

/* ------------------------------------------------------ *
   read_til_crnl(FILE *)
   skip over all request info until a CRNL is seen
   ------------------------------------------------------ */

void read_til_crnl(FILE *fp)
{
	char	buf[BUFSIZ];
	while( fgets(buf,BUFSIZ,fp) != NULL && strcmp(buf,"\r\n") != 0 )
		;
}

/* ------------------------------------------------------ *
   process_rq( char *rq, int fd, struct in_addr caller )
   do what the request asks for and write reply to fd 
   handles request in a new process
   rq is HTTP command:  GET /foo/bar.html HTTP/1.0
   ------------------------------------------------------ */

void process_rq( char *rq, int fd , struct in_addr caller)
{
	char	cmd[BUFSIZ], arg[BUFSIZ];

	/* create a new process and return if not the child */
	if ( fork() != 0 )
		return;

	/* add more data to the log entry */
	server_log(SL_REQ, rq);
	server_log(SL_ADDR, &caller);

	strcpy(arg, "./");		/* precede args with ./ */
	if ( sscanf(rq, "%s%s", cmd, arg+2) != 2 )
		return;

	if ( strcmp(cmd,"GET") != 0 )
		cannot_do(fd);
	else if ( not_exist( arg ) )
		do_404(arg, fd );
	else if ( isadir( arg ) )
		do_ls( arg, fd );
	else if ( ends_in_cgi( arg ) )
		do_exec( arg, fd );
	else
		do_cat( arg, fd );
}

/* ------------------------------------------------------ *
   the reply header thing: all functions need one
   if content_type is NULL then don't send content type
   ------------------------------------------------------ */

void header( FILE *fp, char *content_type )
{
	fprintf(fp, "HTTP/1.0 200 OK\r\n");
	if ( content_type )
		fprintf(fp, "Content-type: %s\r\n", content_type );
	server_log(SL_REPLY, "200");
}

/* ------------------------------------------------------ *
   simple functions first:
        cannot_do(fd)       unimplemented HTTP command
    and do_404(item,fd)     no such object
   ------------------------------------------------------ */

void cannot_do(int fd)
{
	FILE	*fp = fdopen(fd,"w");

	fprintf(fp, "HTTP/1.0 501 Not Implemented\r\n");
	fprintf(fp, "Content-type: text/plain\r\n");
	fprintf(fp, "\r\n");

	fprintf(fp, "That command is not yet implemented\r\n");
	fclose(fp);
	server_log(SL_REPLY, "501");
}

void do_404(char *item, int fd)
{
	FILE	*fp = fdopen(fd,"w");

	fprintf(fp, "HTTP/1.0 404 Not Found\r\n");
	fprintf(fp, "Content-type: text/plain\r\n");
	fprintf(fp, "\r\n");

	fprintf(fp, "The item you requested: %s\r\nis not found\r\n", 
			item);
	fclose(fp);
	server_log(SL_REPLY, "404");
}

/* ------------------------------------------------------ *
   the directory listing section
   isadir() uses stat, not_exist() uses stat
   do_ls runs ls. It should not
   ------------------------------------------------------ */

int isadir(char *f)
{
	struct stat info;
	return ( stat(f, &info) != -1 && S_ISDIR(info.st_mode) );
}

int not_exist(char *f)
{
	struct stat info;
	return( stat(f,&info) == -1 );
}

void do_ls(char *dir, int fd)
{
	FILE	*fp ;

	fp = fdopen(fd,"w");
	header(fp, "text/plain");
	fprintf(fp,"\r\n");
	fflush(fp);

	dup2(fd,1);
	dup2(fd,2);
	close(fd);
	execlp("ls","ls","-l",dir,NULL);
	perror(dir);
	exit(1);
}

/* ------------------------------------------------------ *
   the cgi stuff.  function to check extension and
   one to run the program.
   ------------------------------------------------------ */

/* returns 'extension' of file */
char * file_type(char *f)
{
	char	*cp;
	if ( (cp = strrchr(f, '.' )) != NULL )
		return cp+1;
	return "";
}

int ends_in_cgi(char *f)
{
	return ( strcmp( file_type(f), "cgi" ) == 0 );
}

void do_exec( char *prog, int fd )
{
	FILE	*fp ;

	fp = fdopen(fd,"w");
	header(fp, NULL);
	fflush(fp);
	dup2(fd, 1);
	dup2(fd, 2);
	close(fd);
	execl(prog,prog,NULL);
	perror(prog);
}
/* ------------------------------------------------------ *
   do_cat(filename,fd)
   sends back contents after a header
   ------------------------------------------------------ */

void do_cat(char *f, int fd)
{
	char	*extension = file_type(f);
	char	*content = "text/plain";
	FILE	*fpsock, *fpfile;
	int	c;

	if ( strcmp(extension,"html") == 0 )
		content = "text/html";
	else if ( strcmp(extension, "gif") == 0 )
		content = "image/gif";
	else if ( strcmp(extension, "jpg") == 0 )
		content = "image/jpeg";
	else if ( strcmp(extension, "jpeg") == 0 )
		content = "image/jpeg";

	fpsock = fdopen(fd, "w");
	fpfile = fopen( f , "r");
	if ( fpsock != NULL && fpfile != NULL )
	{
		header( fpsock, content );
		fprintf(fpsock, "\r\n");
		while( (c = getc(fpfile) ) != EOF )
			putc(c, fpsock);
		fclose(fpfile);
		fclose(fpsock);
	}
	exit(0);
}

/* ------------------------------------------------------- *
 * serverlog object here
 * -------------------------------------------------------
 *  This section is better as a file called serverlog.c
 *  with its matching serverlog.h
 *  It is folded into the main file just for ease of
 *  distribution as a solution.
 * -------------------------------------------------------
 *   The function server_log(int op, void *arg)
 *   supports the following operations
 *    
 *       op           arg
 *       --------     --------------------
 *       SL_OPEN      nameoffile
 *       SL_ADDR      struct in_addr of caller
 *       SL_REQ       HTTP request
 *       SL_REPLY     HTTP reply code
 *
 *    The SL_REPLY operation also writes the
 *    data to the logfile.
 *
 *    log entries contain
 *    time callerIP result request
 *
 *    Notice how this is used with fork to
 *    create new instances of the logfile object
 *    with different values for member variables.
 */

int
server_log(int op, void *arg)
{
	static int sl_fd = -1;
	static struct in_addr caller;
	static char request[REQUEST_MAX];
	static char reply[REPLY_MAX];
	int	rv = 0;

	/*
	 * open: close if open, then open for append
	 */
	if ( op == SL_OPEN ){
		if ( sl_fd != -1 )
			close(sl_fd);
		sl_fd = open((char *)arg, O_WRONLY|O_CREAT|O_APPEND, LOGMODE);
		if ( sl_fd == -1 )
			oops(arg);
		return 0;
	}

	/* if not open, ignore all requets */

	if ( sl_fd == -1 )
		return 0;

	/* otherwise handle other operations */
	/* be VERY CAREFUL TO PREVENT BUFFER OVERFLOWS */
	/* this is the WEB and intruders will push every input value to a limit */
	else if ( op == SL_ADDR )
		caller = *((struct in_addr *)arg);
	else if ( op == SL_REQ )
		safecpy(request, (char *)arg, REQUEST_MAX);
	else if ( op == SL_REPLY )
	{
		safecpy(reply, (char *)arg, REPLY_MAX);
		rv = logwrite(sl_fd, caller, request, reply);
	}
	return rv;
}

/*
 * compose a log entry and write it in one syscall
 */

int logwrite(int fd, struct in_addr caller, char *req, char *resp)
{
	/*            time+addr+rq+resp+spaces */

	char	entry[30+30+REQUEST_MAX+REPLY_MAX+10];
	time_t	now;
	char	*timeptr;
	char 	*hostptr;
	int	len;

	time(&now);			/* get time of entry	*/
	timeptr = ctime(&now);			/* convert	*/
	timeptr[strlen(timeptr)-1] = '\0';	/* trim '\n'	*/
	hostptr = inet_ntoa(caller);

	sprintf(entry, "%s %s %s %s", timeptr, hostptr, resp, req);
	len = strlen(entry);
	return  write(fd, entry, len) ;
}

/*
 * copy to buffer and trim
 */
void safecpy(char *dest, char *src, int len)
{
	strncpy(dest, src, len-1);
	dest[len-1] = '\0';
}
