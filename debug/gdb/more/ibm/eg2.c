#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
  printf("start thread.");
  int i;
  for(i = 0; i < 60; i++)
  {
    sleep(1);
	printf("sleep: %i", i);
  }
  return 0;
}
