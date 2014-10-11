#include <stdio.h>
#include <stdlib.h>
int main()
{
    int i;
    int*pn = (int*)malloc(5*sizeof(int));
    printf("malloci: %p\n",pn);
    for(i=0;i<5;i++)
        pn[i] = i;
    pn = (int*)realloc(pn,10*sizeof(int));
    printf("realloc: %p\n",pn);
    for(i=5;i<10;i++)
        pn[i]=i;
    for(i=0;i<10;i++)
        printf("%3d",pn[i]);
    printf("\n");
    free(pn);
    return 0;
}
