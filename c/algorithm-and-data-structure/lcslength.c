#include <stdio.h>
int** init_array(int row, int col)
{
    int i;
    int **ppc=(int**)malloc(row * col * sizeof(int*));
    for(i = 0; i < row; i++)
    {
        int *pc = (int*)malloc(col * sizeof(int));
        ppc[i] = pc;
    }
    return ppc;
}
void print_array1(int** ppa, int row, int col)
{
    int i, j;
    for(i = 0; i < row; i++)
    {
        for(j = 0; j < col; j++)
        {
            printf("%d * %d:\t%d\n", i, j, ppa[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
void print_array(int** ppa, int row, int col)
{
    int i, j;
    for(i = 0; i < row; i++)
    {
        for(j = 0; j < col; j++)
        {
            printf("%d", ppa[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void lcslength(int rowA, int rowB, char *x, char *y, int **c, int **b)
{
    int i, j;
    for(i =0; i < rowA; i++)
    {
        for(j =0; j < rowB; j++)
        {
            if(x[i] == y[j])
            {
                c[i+1][j+1] = c[i][j]+1;
                b[i+1][j+1] = 1;
            }
            else if(c[i][j+1] >= c[i+1][j])
            {
                c[i+1][j+1] = c[i][j+1];
                b[i+1][j+1] = 2;
            }
            else
            {
                c[i+1][j+1] = c[i+1][j];
                b[i+1][j+1] = 3;
            }
        }
    }
}
void main()
{
    int i;
    char A[]={'A', 'B', 'C', 'B', 'D', 'A', 'B'};
    char B[]={'B', 'D', 'C', 'A', 'B', 'A'};
    int rowA = 7;
    int rowB = 6;
    int **ppc=init_array(rowA + 1, rowB + 1);
    int **ppb=init_array(rowA + 1, rowB + 1);
    lcslength(rowA, rowB, A, B, ppc, ppb);
    for(i=0; i<rowA; i++)
    {
        printf("%c", A[i]);
    }
    printf("\n");
    for(i=0; i<rowB; i++)
    {
        printf("%c", B[i]);
    }
    printf("\n");
    printf("\n");
    print_array(ppc, rowA+1, rowB+1);
    print_array(ppb, rowA+1, rowB+1);
/*
    for(i = 0; i < rowA; i++)
    {
        for(int j = 0; j < rowB; j++)
        {
            printf("%d * %d:\t%d\n", i, j, ppc[i][j]);
        }
        printf("\n");
    }
    printf("\n");
*/
}
