/** sol14.11.c
 ** ------------------------------------------------------------
	A version of twebserv.c that uses a mutex to protect
	the server stats variables is
	sol14.11.c.

 ** ------------------------------------------------------------
 **
 **
 *  A version of the threaded web server that uses a mutex to
 *  protect the server statistics.
 *
 *  Note: the tracking of stats is not completely implemented.
 *        This example shows how to get started with the task,
 *        but has not finished all the details.
 */

/* twebserv.c - a threaded minimal web server (version 0.2)
 *    usage: tws portnumber
 * features: supports the GET command only
 *           runs in the current directory
 *           creates a thread to handle each request
 *           supports a special status URL to report internal state
 *
 * build: cc sol14.11.c socklib.c -lpthread -o sol14.11
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <string.h>

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include <dirent.h>
#include <time.h>
#include "socklib.h"

/* server facts here, plus a mutex for them */

time_t	 server_started ;
int	 server_bytes_sent;
int	 server_requests;

pthread_mutex_t stats_lock = PTHREAD_MUTEX_INITIALIZER;

/*
 * prototypes
 */

static void stats_get(time_t *started, int *bytesp, int *hitsp);
static void stats_add(int bytesamt, int hitsamt);
int	    main(int ac, char *av[]);
static void setup(pthread_attr_t *attrp);
static void *handle_call(void *fdptr);
static void skip_rest_of_header(FILE *fp);
static void process_rq( char *rq, int fd);
static void sanitize(char *str);
int 	    built_in(char *arg, int fd); 
int         http_reply(int fd, FILE **fpp, int code, char *msg, 
 		char	*type, char *content);
static void not_implemented(int fd);
static void do_404(char *item, int fd);
int  	    isadir(char *f);
static int  not_exist(char *f);
static void do_ls(char *dir, int fd);
char *      file_type(char *f);
static void do_cat(char *f, int fd);

/*
 * Access to the server stats are managed through
 * these functions below:
 *
 *     stats_get(time_t *started, int *bytesp, int *hitsp )
 *     stats_add(int bytesamt, int hitsamt)
 *
 * These two functions use the mutex to prevent trouble
 */

static void stats_get(time_t *started, int *bytesp, int *hitsp)
{
	pthread_mutex_lock(&stats_lock);
	*started = server_started;
	*bytesp  = server_bytes_sent;
	*hitsp   = server_requests;
	pthread_mutex_unlock(&stats_lock);
}

static void stats_add(int bytesamt, int hitsamt)
{
	pthread_mutex_lock(&stats_lock);
	server_bytes_sent += bytesamt;
	server_requests   += hitsamt;
	pthread_mutex_unlock(&stats_lock);
}


int main(int ac, char *av[])
{
	int 		sock, fd;
	int		*fdptr;
	pthread_t	worker;
	pthread_attr_t	attr;


	if ( ac == 1 ){
		fprintf(stderr,"usage: tws portnum\n");
		exit(1);
	}
	sock = make_server_socket( atoi(av[1]) );
	if ( sock == -1 ) { perror("making socket"); exit(2); }

	setup(&attr);

	/* main loop here: take call, handle call in new thread  */

	while(1){
		fd = accept( sock, NULL, NULL );
		stats_add(0,1);

		fdptr = malloc(sizeof(int));
		*fdptr = fd;
		pthread_create(&worker,&attr,handle_call,fdptr);
	}
	return 0;
}

/*
 * initialize the status variables and
 * set the thread attribute to detached
 */
static void setup(pthread_attr_t *attrp)
{
	pthread_attr_init(attrp);
	pthread_attr_setdetachstate(attrp,PTHREAD_CREATE_DETACHED);

	time(&server_started);
	server_requests = 0;
	server_bytes_sent = 0;
}

static void *handle_call(void *fdptr)
{
	FILE	*fpin;
	char	request[BUFSIZ];
	int	fd ;

	fd = *(int *)fdptr;
	free(fdptr);				/* get fd from arg  */

	fpin = fdopen(fd, "r");			/* buffer input	*/
	fgets(request,BUFSIZ,fpin);		/* read client request */
	printf("got a call on %d: request = %s", fd, request);
	skip_rest_of_header(fpin);

	process_rq(request, fd);		/* process client rq */

	fclose(fpin);
	return NULL;
}

/* ------------------------------------------------------ *
   skip_rest_of_header(FILE *)
   skip over all request info until a CRNL is seen
   ------------------------------------------------------ */
static void skip_rest_of_header(FILE *fp)
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
static void process_rq( char *rq, int fd)
{
	char	cmd[BUFSIZ], arg[BUFSIZ];

	if ( sscanf(rq, "%s%s", cmd, arg) != 2 )
		return;
	sanitize(arg);
	printf("sanitized version is %s\n", arg);

	if ( strcmp(cmd,"GET") != 0 )
		not_implemented(fd);
	else if ( built_in(arg, fd) )
		;
	else if ( not_exist( arg ) )
		do_404(arg, fd);
	else if ( isadir( arg ) )
		do_ls( arg, fd );
	else
		do_cat( arg, fd );
}
/*
 * make sure all paths are below the current directory
 */
static void sanitize(char *str)
{
	char	*src, *dest;

	src = dest = str;

	while( *src ){
		if( strncmp(src,"/../",4) == 0 )
			src += 3;
		else if ( strncmp(src,"//",2) == 0 )
			src++;
		else
			*dest++ = *src++;
	}
	*dest = '\0';
	if ( *str == '/' )
		strcpy(str,str+1);

	if ( str[0]=='\0' || strcmp(str,"./")==0 || strcmp(str,"./..")==0 )
		strcpy(str,".");
}

/* handle built-in URLs here.  Only one so far is "status" */
int built_in(char *arg, int fd)
{
	FILE	*fp;
	time_t	start_time;
	int	requests;
	int	volume;

	if ( strcmp(arg,"status") != 0 )
		return 0;
	http_reply(fd, &fp, 200, "OK", "text/plain",NULL);

	stats_get(&start_time, &volume, &requests);
	fprintf(fp,"Server started: %s", ctime(&start_time));
	fprintf(fp,"Total requests: %d\n", requests);
	fprintf(fp,"Bytes sent out: %d\n", volume);
	fclose(fp);
	return 1;
}

int http_reply(int fd, FILE **fpp, int code, char *msg, char *type, char *content)
{
	FILE	*fp = fdopen(fd, "w");
	int	bytes = 0;

	if ( fp != NULL ){
		bytes = fprintf(fp,"HTTP/1.0 %d %s\r\n", code, msg);
		bytes += fprintf(fp,"Content-type: %s\r\n\r\n", type);
		if ( content )
			bytes += fprintf(fp,"%s\r\n", content);
	}
	fflush(fp);
	if ( fpp )
		*fpp = fp;
	else
		fclose(fp);
	return bytes;
}

/* ------------------------------------------------------ *
   simple functions first:
        not_implemented(fd)      unimplemented HTTP command
        and do_404(item,fd)     no such object
   ------------------------------------------------------ */
static void not_implemented(int fd)
{
	http_reply(fd,NULL,501,"Not Implemented","text/plain",
			"That command is not implemented");
}

static void do_404(char *item, int fd)
{
	http_reply(fd,NULL,404,"Not Found","text/plain",
			"The item you seek is not here");
}

/* ------------------------------------------------------ *
   the directory listing section
   isadir() uses stat, not_exist() uses stat
   ------------------------------------------------------ */
int isadir(char *f)
{
	struct stat info;
	return ( stat(f, &info) != -1 && S_ISDIR(info.st_mode) );
}

static int not_exist(char *f)
{
	struct stat info;
	return( stat(f,&info) == -1 );
}

static void do_ls(char *dir, int fd)
{
	DIR	      *dirptr;
	struct dirent *direntp;
	FILE	      *fp;
	int	      bytes = 0;

	bytes = http_reply(fd,&fp,200,"OK","text/plain",NULL);
	bytes += fprintf(fp,"Listing of Directory %s\n", dir);

	if ( (dirptr = opendir(dir)) != NULL ){
		while( (direntp = readdir(dirptr)) != NULL ){
			bytes += fprintf(fp, "%s\n", direntp->d_name);
		}
		closedir(dirptr);
	}
	fclose(fp);

	stats_add(bytes, 0);
}

/* ------------------------------------------------------ *
   functions to cat files here.
   file_type(filename) returns the 'extension': cat uses it
   ------------------------------------------------------ */
char * file_type(char *f)
{
	char	*cp;
	if ( (cp = strrchr(f, '.' )) != NULL )
		return cp+1;
	return "";
}

/* do_cat(filename,fd): sends header then the contents */

static void do_cat(char *f, int fd)
{
	char	*extension = file_type(f);
	char	*type = "text/plain";
	FILE	*fpsock, *fpfile;
	int	c;
	int	bytes = 0;

	if ( strcmp(extension,"html") == 0 )
		type = "text/html";
	else if ( strcmp(extension, "gif") == 0 )
		type = "image/gif";
	else if ( strcmp(extension, "jpg") == 0 )
		type = "image/jpeg";
	else if ( strcmp(extension, "jpeg") == 0 )
		type = "image/jpeg";

	fpsock = fdopen(fd, "w");
	fpfile = fopen( f , "r");
	if ( fpsock != NULL && fpfile != NULL )
	{
		bytes = http_reply(fd,&fpsock,200,"OK",type,NULL);
		while( (c = getc(fpfile) ) != EOF ){
			putc(c, fpsock);
			bytes++;
		}
		fclose(fpfile);
		fclose(fpsock);
	}
	stats_add(bytes,0);

}
