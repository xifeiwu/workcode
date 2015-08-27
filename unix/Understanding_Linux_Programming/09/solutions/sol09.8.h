/*
 * Revised header file for background processsing
 * This version has prototypes that match the versions
 * of functions that pass the background flag around
 */

#define	YES	1
#define	NO	0

char	*next_cmd();
char	**splitline(char *);
void	freelist(char **);
void	*emalloc(size_t);
void	*erealloc(void *, size_t);
int	execute(char **,int);
void	fatal(char *, char *, int );

