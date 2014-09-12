#include "go-hello.h"

G_DEFINE_TYPE(GoHello, go_hello, G_TYPE_OBJECT);
static void go_hello_test(void);

static
void go_hello_init(GoHello *self)
{
    g_printf("go hello init!\n");
    self->a = NULL;
    self->test = go_hello_test;
}

static 
void go_hello_class_init(GoHelloClass *klass)
{
    g_printf("go hello class init!\n");
}

static
void go_hello_test(void)
{
    g_printf("I LOVE YOU CJ!\n");
}
