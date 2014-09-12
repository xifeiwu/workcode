#include "maman-bar.h"

/*
    http://library.gnome.org/devel/gobject/2.21/gobject-Type-Information.html#G-DEFINE-TYPE--CAPS

    A convenience macro for type implementations, which declares a class 
    initialization function, an instance initialization function (see GTypeInfo
    for information about these) and a static variable named t_n_parent_class 
    pointing to the parent class. Furthermore, it defines a *_get_type() 
    function. See G_DEFINE_TYPE_EXTENDED() for an example.
*/
G_DEFINE_TYPE (MamanBar, maman_bar, G_TYPE_OBJECT);


/* Define the private structure in the .c file */
#define MAMAN_BAR_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MAMAN_TYPE_BAR, MamanBarPrivate))

struct _MamanBarPrivate
{
  int hsize;
  gchar *msg;
};


/* Init functions */
static void
maman_bar_class_init (MamanBarClass *klass)
{
    g_type_class_add_private (klass, sizeof (MamanBarPrivate));

    /* Setup the default handler for virtual method */
    klass->do_action_virt = maman_bar_do_action_virt_default;
}


static void
maman_bar_init (MamanBar *self)
{
    
    g_print("maman_bar_init() - init object\n");
    

    /* Initialize all public and private members to reasonable default values. */
    
    /* Initialize public fields */
    self->public_int = 99;

    g_print("  initializing public_int to %d\n", self->public_int);
 

    /* Initialize private fields */
    MamanBarPrivate *priv;
    self->priv = priv = MAMAN_BAR_GET_PRIVATE(self);
    priv->hsize = 42;

    g_print("  init'd private variable priv->hsize to %d\n", priv->hsize);


    /* If you need specific construction properties to complete initialization,
     * delay initialization completion until the property is set. 
     */

}


/* Object non-virtual method */
void maman_bar_do_action (MamanBar *self, gchar *msg) {
    /* First test that 'self' is of the correct type */
    g_return_if_fail (MAMAN_IS_BAR (self));


    // Assign to private 'msg' 
    self->priv->msg = msg;

    g_print("maman_bar_do_action() - %s\n", self->priv->msg);

}

/* Object virtual method call - performs the override */
void maman_bar_do_action_virt (MamanBar *self, gchar *msg) {
     /* First test that 'self' is of the correct type */
    g_return_if_fail (MAMAN_IS_BAR (self));

    g_print("maman_bar_do_action_virt() -> ");
    MAMAN_BAR_GET_CLASS (self)->do_action_virt(self, msg);  
}

/* Object virtual method default action (can be overridden) */
void maman_bar_do_action_virt_default (MamanBar *self, gchar *msg) {

    g_print("maman_bar_do_action_virt_default() - %s\n", msg );

}

int
main (int argc, char *argv[])
{
    /*
     * Prior to any use of the type system, g_type_init() has to be called 
     * to initialize the type system and assorted other code portions 
     * (such as the various fundamental type implementations or the signal 
     * system).
     */
    g_type_init();

    /* Create our object */
    MamanBar *bar = g_object_new (MAMAN_TYPE_BAR, NULL);

    bar->public_int +=1;
    g_print("incremented bar->public_int:  %d\n", bar->public_int);

    /* Call object method */
    maman_bar_do_action(bar, "helowrld");

    /* Call virtual object method - we could subclass and override... */
    maman_bar_do_action_virt(bar, "HELOWRLD");

    return 0; 
}
