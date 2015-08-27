/** sol04.17.c
 ** ------------------------------------------------------------
	A slightly more robust version of rename coded with 
	link and unlink is: 
	sol04.17.c.
	This problem is a short exercise is understanding the
	sequence of links and unlinks to move a file from
	one place to another.

 ** ------------------------------------------------------------
 **
 **
 * Expanded version of rename() using link and unlink from the text.
 * 
 * Note: this short piece of code does not provide many of the
 * features of rename().
 */

/*
 * This function makes sure only one link to the file is left.
 * If things go well, the newname is the link.  If there is an
 * error, the oldname is left intact.  If there is a bigger
 * error, both names are left.
 */

int link_based_rename(char *oldname, char *newname)
{
	if ( link(oldname, newname) != -1 )
	{
		if ( unlink(oldname) == -1 ){
			unlink(newname);
			return -1;
		}
		return 0;
	}
	return -1;
}
