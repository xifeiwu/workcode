/** sol12.5.c
 ** ------------------------------------------------------------
	A version of webserv that puts some typical CGI
	variables into the environment before calling
	exec is
	sol12.5.c.
	Three kinds of variables are demonstrated in this solution: 
	      variables about the server,
	      variables sent as part of the http header,
	  and variables about the client.

 ** ------------------------------------------------------------
 **
 **
 *   Version of webserv.c that includes some environment variables
 *   when running CGI programs.
 *
 *   The variables included for cgi programs are:
 *      SERVER_SOFTWARE
 *      SERVER_NAME
 *      REMOTE_PORT
 *      REMOTE_ADDR
 *      REQUEST_URI
 *      REQUEST_METHOD
 *      HTTP_USER_AGENT
 *
 *   This program uses the varlib.c system from the smsh.c
 *   program in the shell Chapter.   That system does most
 *   of the filing and export work for us.
 *
 *      usage: ws portnumber
 *   features: supports the GET command only
 *             runs in the current directory
 *             forks a new child to handle each request
 *             has MAJOR security holes, for demo purposes only
 *             has many other weaknesses, but is a good start
 *
 *      build: cc sol12.5.c socklib.c varlib.c -o sol12.5
 *
 *      By showing how easily we can use varlib.c to manage the
 *      variables for the web server, we see in concrete terms
 *      how much a web browser work a shell and OS.

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
#include	<errno.h>

#include	"varlib.h"
#include	"socklib.h"

#define	oops(s,x) { perror(s); exit(x); }

void	setupvars();
void	process_request(int , struct sockaddr_in *);
void	process_header(FILE *);
void	act_on_request(char *, char *, int );
void	header( FILE *, char *);
void	cannot_do(int );
void	do_404(char *, int );
int	isadir(char *);
int	not_exist(char *);
void	do_ls(char *, int );
char	* file_type(char *);
int	ends_in_cgi(char *);
void	do_exec( char *, int );
void	do_cat(char *, int );

int main(int ac, char *av[])
{
	int 		   sock, fd;
	int		   len;
	struct sockaddr_in clnt_addr;

	if ( ac == 1 ){
		fprintf(stderr,"usage: ws portnum\n");
		exit(1);
	}
	sock = make_server_socket( atoi(av[1]) );
	if ( sock == -1 ) 
		oops("socket", 2);

	setupvars();

	/* main loop here */

	while(1){
		/*
		 * wait for a call
		 */
		len = sizeof(struct sockaddr);
		fd = accept(sock, (struct sockaddr *)&clnt_addr, &len);

		/* 
		 * take a call and buffer it
		 */ 
		if ( fd >= 0 ){
			process_request(fd, &clnt_addr);
			close(fd);
		}
		else
			perror("accept");
	}
	return 0;
}

/*
 * load the environment table from environ and
 * set some fixed values that describe this host and server
 */

void setupvars()
{
	char	hostname[512];

	VLstore("SERVER_SOFTWARE", "Simple-WebServer 0.2");
	VLexport("SERVER_SOFTWARE");
	gethostname(hostname, 512);
	VLstore("SERVER_NAME", hostname);
	VLexport("SERVER_NAME");
	/*
	   add here other fixed values that can be 
	   computed at startup
	*/
}

/*
 * A more general process_request than the earlier version
 * This one has to read the header and put some parts into
 * the environment list.
 */
void process_request(int fd, struct sockaddr_in *caller)
{
	FILE	*fpin;
	char	request[BUFSIZ];
	char	portnumstr[10];
	char	cmd[BUFSIZ], arg[BUFSIZ];

	/* process the request in a child process */

	if ( fork() != 0 )
		return;

	/* jot down the caller's adress and port */

	VLstore("REMOTE_ADDR", inet_ntoa(caller->sin_addr));
	VLexport("REMOTE_ADDR");
	sprintf(portnumstr, "%d", ntohs(caller->sin_port));
	VLstore("REMOTE_PORT", portnumstr);
	VLexport("REMOTE_PORT");

	/* buffer the socket for input and read request */

	fpin = fdopen(fd, "r" );
 	if ( fpin == NULL )
		exit(2);
	fgets(request,BUFSIZ,fpin);
	printf("got a call: request = %s", request);
	process_header(fpin);

	/* then do the request */

	strcpy(arg, "./");
	if ( sscanf(request,"%s%s", cmd, arg+2) == 2 ){
		VLstore("REQUEST_METHOD", cmd);
		VLexport("REQUEST_METHOD");
		VLstore("REQUEST_URI",    arg+2);
		VLexport("REQUEST_URI");
		act_on_request(cmd, arg, fd);
	}
}

/* ------------------------------------------------------ *
   process_header(FILE *)
   after the HTTP request may be several lines
   of header information.  These variables provide
   define the parameters of the request.
   The set of parameters is terminated by a blank line.

   This function looks for the header line marked

     User-Agent: name-of-browser

   And puts the value for that tag in the environment
   under HTTP_USER_AGENT.  It's pretty easy to add other
   http variables to the CGI environment.  
   ------------------------------------------------------ */

void process_header(FILE *fp)
{
	char	buf[BUFSIZ];
	char	browser_tag[] = "User-Agent: ";
	int	b_taglen = strlen(browser_tag);

	while( fgets(buf,BUFSIZ,fp) != NULL && strcmp(buf,"\r\n") != 0 )
	{
		printf("Header line: %s", buf);
		if ( strncmp(buf, browser_tag, b_taglen) == 0 )
		{
			VLstore("HTTP_USER_AGENT",buf+b_taglen);
			VLexport("HTTP_USER_AGENT");
		}
	}
}
/* ------------------------------------------------------ *
   act_on_request( char *cmd, char *arg, int fd )
   do what the request asks for and write reply to fd 
   if the request is the HTTP command:  GET /foo/bar.html HTTP/1.0
   then  cmd is "GET" and arg is "/foo/bar.html"
   ------------------------------------------------------ */

void act_on_request(char *cmd, char *arg, int fd)
{

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

char * file_type(char *f)
/* returns 'extension' of file */
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
	extern char **environ;
	FILE	*fp ;

	fp = fdopen(fd,"w");
	header(fp, NULL);
	fflush(fp);
	dup2(fd, 1);
	dup2(fd, 2);
	close(fd);
	environ = VLtable2environ();
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
