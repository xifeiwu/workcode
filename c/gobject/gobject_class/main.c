#include "go-hello.h"

int main(int argc, char *argv[])
{
    g_type_init();

    GoHello *hello;
    GoHello *hello2;

    hello = g_object_new(GO_TYPE_HELLO, NULL);
    hello->a = 1;
    hello->test();
    g_printf("hello->a=%d\n",hello->a);
    hello2 = g_object_new(GO_TYPE_HELLO, NULL);
    hello2->test();
    return 0;
}
