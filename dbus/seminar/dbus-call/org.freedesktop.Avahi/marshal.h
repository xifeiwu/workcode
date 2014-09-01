
#ifndef ___avahi_marshal_MARSHAL_H__
#define ___avahi_marshal_MARSHAL_H__

#include	<glib-object.h>

G_BEGIN_DECLS

/* VOID:INT,INT,STRING,STRING,STRING,UINT (marshal.list:1) */
extern void _avahi_marshal_VOID__INT_INT_STRING_STRING_STRING_UINT (GClosure     *closure,
                                                                    GValue       *return_value,
                                                                    guint         n_param_values,
                                                                    const GValue *param_values,
                                                                    gpointer      invocation_hint,
                                                                    gpointer      marshal_data);

/* VOID:INT,INT,STRING,STRING,STRING,INT,UINT (marshal.list:2) */
extern void _avahi_marshal_VOID__INT_INT_STRING_STRING_STRING_INT_UINT (GClosure     *closure,
                                                                        GValue       *return_value,
                                                                        guint         n_param_values,
                                                                        const GValue *param_values,
                                                                        gpointer      invocation_hint,
                                                                        gpointer      marshal_data);

G_END_DECLS

#endif /* ___avahi_marshal_MARSHAL_H__ */

