#include "apue.h"

int
main(void)
{
	if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1){
		printf("cannot seek %d\n", STDIN_FILENO);
	}
	else{
		printf("seek OK. %d\n", STDIN_FILENO);
	}
	exit(0);
}
