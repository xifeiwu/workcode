#include <stdio.h>
void swap(int *pa, int *pb)
{
    int c;
    c = *pb;
    *pb = *pa;
    *pa = c;
}
void perm(int list[], int k, int m)
{
    int i;
    if(k==m)
    {
        for(i=0; i<=m; i++)
        {
            printf("%d\t", list[i]);
        }
        printf("\n");
    }
    else
    {
        for(i=k; i<=m; i++)
        {
            swap(&list[k], &list[i]);
            perm(list, k+1, m);
            swap(&list[k], &list[i]);
        }
    }
}
void main()
{
    int array[]={1,2,3,4,5};
    perm(array, 0, 4);
//    int length = (sizeof(array) / sizeof(array[0]));
//    printf("length: %d\n", length);
//    printf("hello, world! %s\n", "xifei wu");
}
