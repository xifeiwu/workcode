#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    printf("hello, world.\n");
    char str[10]={ 'I',' ','a','m',' ','h','a','p','p','y'};
    printf("value of str: %s\n", str);
    printf("length of str: %d\n", strlen(str));
    printf("size of str: %d\n", sizeof(str));

    char str1[]="I-am-happy";
    printf("value of str1: %s\n", str1);
    printf("length of str1: %d\n", strlen(str1));
    printf("size of str1: %d\n", sizeof(str1));

    char *str2="I-am-happy";
    printf("value of str2: %s\n", str2);
    printf("length of str2: %d\n", strlen(str2));
    printf("size of str2: %d\n", sizeof(str2));

    char str3[10]={ 'I',' ','a','m','\0','h','a','p','p','y'};
    printf("value of str3: %s\n", str3);
    printf("length of str3: %d\n", strlen(str3));
    printf("size of str3: %d\n", sizeof(str3));

    char c1[]={'I',' ','a','m',' ','h','a','p','p','y'};
    char c2[]="I am happy";
    int i1=sizeof(c1);
    int i2=sizeof(c2);
    printf("size of c1: %d\n",i1);
    printf("size of c2: %d\n",i2);
    int i;
    for(i=0; i< 100; i++){
        printf("%d: %c\n", i, c2[i]);
    }

    return 0;
}
