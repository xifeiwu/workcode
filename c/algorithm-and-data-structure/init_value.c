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
}
void init_value(int* arr, int value, int length)
{
    for(int i=0; i<length; i++)
    {
        arr[i] = value;
    }
}
int main()
{
    int size = 10;
    unsigned int* int_arr = (unsigned int*)malloc(size * sizeof(int));
    print_value(int_arr, size);
    memset(int_arr, 5, size*sizeof(int));
    print_value(int_arr, size);
    init_value(int_arr, 5, size);
    print_value(int_arr, size);

}
/*
用memset来初始化一个内存块,如果被初始化的值是0,是正确的，如：
int buffer[256];
memset(buffer,0,sizeof(buffer));
但如果初始化是非0值，如：
int buffer[256];
memset(buffer,5,sizeof(buffer));
会发现输出的数组值并非指定的5。
原因：因为memset的第一个参数是void* 所以是在8位上操作。f[0]实际填充表示为2进制为00000001000000010000000100000001也就是16843009。
完善：试用自己写的init_value方法。
*/
