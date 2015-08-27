/** sol03.19.c
 ** ------------------------------------------------------------
	sol03.19.c is a version
	of ls2.c that supports the -u option.  This program also
	supports the suid, sgid, and sticky bit handling from solution
	3.12.  The only real change was to use the st_atime
	member from the stat struct instead of the st_mtime
	member.

 ** ------------------------------------------------------------
 *
 * ls2.c - supports the -u option
 *         handles suid, sgid, sticky bits, also fixes bug,
 *
 *	purpose  list contents of directory or directories
 *	action   if no args, use .  else list files in args
 *	note     uses lstat and pwd.h and grp.h 
 *	NOTE	 uses lstat to get info about link not the target
 *	
 *	build: cc sol03.19.c -o sol03.19
 */
#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<dirent.h>
#include	<sys/stat.h>

void do_ls(char[],int);
void dostat(char *,char *, int);
void show_file_info( char *, struct stat *, int);
void mode_to_letters( int , char [] );
char *uid_to_name( uid_t );
char *gid_to_name( gid_t );

main(int ac, char *av[])
{
	int	u_flag = 0;
	int	anyfiles =  0;

	while ( --ac ){
		if ( strcmp("-u", *++av) == 0 )
			u_flag = 1;
		else {
			do_ls( *av , u_flag );
			anyfiles = 1;
		}
	}
	if ( !anyfiles )
		do_ls(".", u_flag);
}

void do_ls( char dirname[] , int u_flag )
/*
 *	list files in directory called dirname
 * 	if u_flag is true, then show 'used' time instead of 'mod' time
 */
{
	DIR		*dir_ptr;		/* the directory */
	struct dirent	*direntp;		/* each entry	 */
	char		*fullpath;

	fullpath = (char *)malloc(strlen(dirname) + 1 + MAXNAMLEN + 1);
	if ( ( dir_ptr = opendir( dirname ) ) == NULL )
		fprintf(stderr,"ls2: cannot open %s\n", dirname);
	else
	{
		while ( ( direntp = readdir( dir_ptr ) ) != NULL ){
			sprintf(fullpath,"%s/%s",dirname,direntp->d_name);
			dostat( fullpath, direntp->d_name, u_flag );
		}
		closedir(dir_ptr);
	}
	free(fullpath);
}

void dostat( char *fullpath, char *filename, int u_flag )
{
	struct stat info;

	if ( lstat(fullpath, &info) == -1 )		/* cannot stat	 */
		perror(filename);			/* say why	 */
	else					/* else show info	 */
		show_file_info(filename, &info, u_flag );
}

void show_file_info( char *filename, struct stat *info_p, int u_flag )
/*
 * display the info about 'filename'.  The info is stored in struct at *info_p
 */
{
	char	*uid_to_name(), *ctime(), *gid_to_name(), *filemode();
	void	mode_to_letters();
        char    modestr[11];

	mode_to_letters( info_p->st_mode, modestr );

	printf( "%s"    , modestr );
	printf( "%4d "  , (int) info_p->st_nlink);	
	printf( "%-8s " , uid_to_name(info_p->st_uid) );
	printf( "%-8s " , gid_to_name(info_p->st_gid) );
	printf( "%8ld " , (long)info_p->st_size);
	printf( "%.12s ", 4+ctime( u_flag ? &info_p->st_atime
				          : &info_p->st_mtime));
	printf( "%s\n"  , filename );
}

/*
 * utility functions
 */

/*
 * This function takes a mode value and a char array
 * and puts into the char array the file type and the 
 * nine letters that correspond to the bits in mode.
 * NOTE: It now codes suid, sgid, and sticky bits
 *       see manual for details.
 */
void mode_to_letters( int mode, char str[] )
{
    strcpy( str, "----------" );           /* default=no perms */

    if ( S_ISDIR(mode) )  str[0] = 'd';    /* directory?       */
    if ( S_ISCHR(mode) )  str[0] = 'c';    /* char devices     */
    if ( S_ISBLK(mode) )  str[0] = 'b';    /* block device     */

    if ( mode & S_IRUSR ) str[1] = 'r';    /* 3 bits for user  */
    if ( mode & S_IWUSR ) str[2] = 'w';
    if ( (mode & S_ISUID) && (mode & S_IXUSR) )
	str[3] = 's';
    else if ( (mode & S_ISUID) && !(mode & S_IXUSR) )
	str[3] = 'S';
    else if ( mode & S_IXUSR ) 
	str[3] = 'x';

    if ( mode & S_IRGRP ) str[4] = 'r';    /* 3 bits for group */
    if ( mode & S_IWGRP ) str[5] = 'w';
    if ( (mode & S_ISGID) && (mode & S_IXGRP) )
	str[6] = 's';
    else if ( (mode & S_ISGID) && !(mode & S_IXGRP) )
	str[6] = 'S';
    else if ( mode & S_IXGRP ) 
	str[6] = 'x';

    if ( mode & S_IROTH ) str[7] = 'r';    /* 3 bits for other */
    if ( mode & S_IWOTH ) str[8] = 'w';
    if ( (mode & S_ISVTX) && (mode & S_IXOTH) )
	str[9] = 't';
    else if ( (mode & S_ISVTX) && !(mode & S_IXOTH) )
	str[9] = 'T';
    else if ( mode & S_IXOTH ) 
	str[9] = 'x';
}

#include	<pwd.h>

char *uid_to_name( uid_t uid )
/* 
 *	returns pointer to username associated with uid, uses getpw()
 */	
{
	struct	passwd *getpwuid(), *pw_ptr;
	static  char numstr[10];

	if ( ( pw_ptr = getpwuid( uid ) ) == NULL ){
		sprintf(numstr,"%d", uid);
		return numstr;
	}
	else
		return pw_ptr->pw_name ;
}

#include	<grp.h>

char *gid_to_name( gid_t gid )
/*
 *	returns pointer to group number gid. used getgrgid(3)
 */
{
	struct group *getgrgid(), *grp_ptr;
	static  char numstr[10];

	if ( ( grp_ptr = getgrgid(gid) ) == NULL ){
		sprintf(numstr,"%d", gid);
		return numstr;
	}
	else
		return grp_ptr->gr_name;
}
