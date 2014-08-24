/*
 * Copyright/Licensing information.
 *
 * Reference:
 *
 * http://library.gnome.org/devel/gobject/unstable/howto-gobject.html
 * http://library.gnome.org/devel/gobject/unstable/chapter-gobject.html
 *
 *
 */


/* inclusion guard */
#ifndef __MAMAN_BAR_H__
#define __MAMAN_BAR_H__

#include <glib-object.h>

/*
 * Potentially, include other headers on which this header depends.
 */

/*
 * Type macros.
 */
#define MAMAN_TYPE_BAR                  (maman_bar_get_type ())
#define MAMAN_BAR(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), MAMAN_TYPE_BAR, MamanBar))
#define MAMAN_IS_BAR(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MAMAN_TYPE_BAR))
#define MAMAN_BAR_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), MAMAN_TYPE_BAR, MamanBarClass))
#define MAMAN_IS_BAR_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), MAMAN_TYPE_BAR))
#define MAMAN_BAR_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), MAMAN_TYPE_BAR, MamanBarClass))

typedef struct _MamanBar        MamanBar;
typedef struct _MamanBarClass   MamanBarClass;

/* 
 * Private instance fields 
 * Uses the Pimpl method:
 *
 * http://www.gotw.ca/gotw/024.htm
 * http://www.gotw.ca/gotw/028.htm
 *
 */
typedef struct _MamanBarPrivate MamanBarPrivate;


/* object */
struct _MamanBar
{
    GObject parent_instance;

    /* public */ 
    int public_int;


    /*< private >*/    
    MamanBarPrivate *priv;
};

/* class */
struct _MamanBarClass
{
    GObjectClass parent_class;

    /* class members */
  
    /* Virtual public method */
    void (*do_action_virt) (MamanBar *self, gchar *msg);

};


/*
 * Non-virtual public method
 */
void maman_bar_do_action (MamanBar *self, gchar *msg /*, other params */);

/* Virtual method call declaration */
void maman_bar_do_action_virt (MamanBar *self, gchar *msg /*, other params */);
/* Virtual method default 'super' class method */
void maman_bar_do_action_virt_default (MamanBar *self, gchar *msg);


#endif /* __MAMAN_BAR_H__ */

