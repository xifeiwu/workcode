/** sol12.7.c
 ** ------------------------------------------------------------
	A version of webserv that returns some of the HTTP
	header fields is
	sol12.7.c.
	This program sends back the name of the server and the
	current time. When sending back a file, the server
	sends the last modified time and the number of bytes
	in the file.  

 ** ------------------------------------------------------------
 **
 **
 *   webserv with more header details.
 *
 *      build: cc sol12.7.c socklib.c -o sol12.7
 */
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<sys/stat.h>
#include	<string.h>
#include        <time.h>
#include	"socklib.h"

#define	SERVER_MODEL	"Simple-WebServer 0.3/SP4"

void	read_til_crnl(FILE *);
void	process_rq( char *, int );
void	http_reply_lines(FILE *, char *);
char	* rfc822_time(time_t );
void	header( FILE *, char *);
void	cannot_do(int );
void	do_404(char *, int );
int	isadir(char *);
int	not_exist(char *);
int	do_ls(char *, int );
char	* file_type(char *);
int	ends_in_cgi(char *);
void	do_exec( char *, int );
void	do_cat(char *, int );

int main(int ac, char *av[])
{
	int 	sock, fd;
	FILE	*fpin;
	char	request[BUFSIZ];

	if ( ac == 1 ){
		fprintf(stderr,"usage: ws portnum\n");
		exit(1);
	}
	sock = make_server_socket( atoi(av[1]) );
	if ( sock == -1 ) {
		perror("socket");
		exit(2);
	}

	/* main loop here */

	while(1){
		/* take a call and buffer it */
		fd = accept( sock, NULL, NULL );
		fpin = fdopen(fd, "r" );

		/* read request */
		fgets(request,BUFSIZ,fpin);
		printf("got a call: request = %s", request);
		read_til_crnl(fpin);

		/* do what client asks */
		process_rq(request, fd);

		fclose(fpin);
	}
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
   process_rq( char *rq, int fd )
   do what the request asks for and write reply to fd 
   handles request in a new process
   rq is HTTP command:  GET /foo/bar.html HTTP/1.0
   ------------------------------------------------------ */

void process_rq( char *rq, int fd )
{
	char	cmd[BUFSIZ], arg[BUFSIZ];

	/* create a new process and return if not the child */
	if ( fork() != 0 )
		return;

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
   HTTP reply lines include the Server, the Date, the
   Connection type, and many other items.  This function
   prints some of them to a stream.

   The file name lets us use stat to get mod time and
   size for regualr files.
   ------------------------------------------------------ */
void http_reply_lines(FILE *fp, char *fname)
{
	time_t now;
	char *rfc822_time(time_t);
	struct stat info;

	fprintf(fp,"Server: %s\r\n", SERVER_MODEL);
	time(&now);
	fprintf(fp,"Date: %s\r\n", rfc822_time(now));
	if ( stat(fname,&info) != -1 && S_ISREG(info.st_mode)){
		fprintf(fp,"Last-modified: %s\r\n", rfc822_time(info.st_mtime));
		fprintf(fp,"Content-length: %u\r\n", (unsigned)info.st_size);
	}
	fprintf(fp,"Accept-range: bytes\r\n");
	fprintf(fp,"Connection: close\r\n");
}


/*
 *      function        rfc822_time()
 *      purpose         return a string suitable for web servers
 *      details         Sun, 06 Nov 1994 08:49:37 GMT    
 *      method          use gmtime() to get struct
 *                      then use asctime to translate to English
 *                      Tue Nov  9 15:37:29 1993\n\0
 *                      012345678901234567890123456789
 *                      then rearrange using sprintf
 *      arg             a time_t value 
 *      returns         a pointer to a static buffer
 */

char * rfc822_time(time_t thetime)
{
        struct tm *t ;
        char    *str;
        int     d;
        static  char retval[36];
        
        t = gmtime( &thetime );         /* break into parts     */
        str = asctime( t );             /* create string        */
        d = atoi( str + 8 );
        sprintf(retval,"%.3s, %02d %.3s %.4s %.8s GMT", 
                        str ,   d, str+4, str+20, str+11 );
        return retval;
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

int do_ls(char *dir, int fd)
{
	FILE	*fp ;

	fp = fdopen(fd,"w");
	header(fp, "text/plain");
	http_reply_lines(fp, dir);
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
	else
		content = "text/plain";

	fpsock = fdopen(fd, "w");
	fpfile = fopen( f , "r");
	if ( fpsock != NULL && fpfile != NULL )
	{
		header( fpsock, content );
		http_reply_lines(fpsock, f);
		fprintf(fpsock, "\r\n");
		while( (c = getc(fpfile) ) != EOF )
			putc(c, fpsock);
		fclose(fpfile);
		fclose(fpsock);
	}
	exit(0);
}
