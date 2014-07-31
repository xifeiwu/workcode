#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
void print_value(int* arr, int length)
{
    for(int i=0; i<length; i++)
    {
        printf("%d: %d\n", i, arr[i]);
    }
    printf("\n");
}
void swap(int a, int b)
{
    int c;
    c = b;
    b = a;
    a = c;
}
void swap_point(int* arr, int i, int j)
{
    int tmp;
    tmp = arr[j];
    arr[j] = arr[i];
    arr[i] = tmp;
}
int main()
{
    int int_arr[] = {2, 3, 1 ,7 ,5, 4, 9};
    int length = sizeof(int_arr)/sizeof(int_arr[0]);
    print_value(int_arr, length);

    int i, j;
    for(i=0; i<length; i++)
    {
        for(j=i; j<length; j++)
        {
            if(int_arr[i] > int_arr[j])
            {
                swap(int_arr[i], int_arr[j]);
            }
        }
    } 
    print_value(int_arr, length);
    
    for(i=0; i<length; i++)
    {
        for(j=i; j<length; j++)
        {
            if(int_arr[i] > int_arr[j])
            {
                swap_point(int_arr, i, j);
            }
        }
    }
    print_value(int_arr, length);
}
/*
被传递的参数不会被调用方法改变值。
加入引用编译会出错void swap(int &a, int &b)，c语言不支持引用。
传指针是可以的。
*/
