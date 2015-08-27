/** sol11.19.c
 ** ------------------------------------------------------------
	server
	A program that uses popen to run "ps ax" as a server is
	sol11.19.c.
	This program finds the name it is running under by
	searching through the output of the ps command.

 ** ------------------------------------------------------------
 **
 **
 *   myname.c - a bizarre use of popen as a server to return the name of the
 *              current program.  This program calls popen to run ps ax and
 *              searches the output of ps for its pid.  It then looks in the
 *              CMD column to find the command.
 *
 *              Note: this program was designed for a particular version of ps.
 *              You may need to modify it to work with your version.
 *              The output of "ps ax" this program works for looks like:
			  PID TTY      STAT   TIME COMMAND
			    1 ?        S      0:04 init
			    2 ?        SW     0:07 [keventd]
 *

 *	build: cc sol11.19.c -o sol11.19
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/types.h>
#include	<unistd.h>

int main()
{
	char	*getmyname(pid_t);
	char	*myname;

	myname = getmyname(getpid());
	printf("My name is %s\n", myname);
	return 0;
}

/*
 *  search output of ps ax for a line starting with pid
 *  Then get field 5 
 */
char *
getmyname(pid_t pid)
{
	FILE		*fp;
	char		buf[BUFSIZ];
	static char	cmd[BUFSIZ];

	if ( (fp = popen("ps ax", "r")) == NULL )
		return "Cannot_run_ps";
	/*
	 * otherwise start looking
	 */

	strcpy(cmd, "unknown");
	while( fgets(buf, BUFSIZ, fp) ){
		if ( atoi(buf) == pid ){
			sscanf(buf,"%*s %*s %*s %*s %s", cmd);
			break;
		}
	}
	pclose(fp);
	return cmd;
}
