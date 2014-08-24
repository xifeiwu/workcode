#ifndef GO_HELLO_H
#define GO_HELLO_H

#include <glib-object.h>

#define GO_TYPE_HELLO   (go_hello_get_type()) 

typedef struct _GoHello GoHello;
struct _GoHello {
    GObject parent;
    int a;
    void (*test)(void);
};
typedef struct _GoHelloClass GoHelloClass;
struct _GoHelloClass {
    GObjectClass    parent_class;
};
GType go_hello_get_type(void);

#endif
