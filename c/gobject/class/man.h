#ifndef __MAN_H__
#define __MAN_H__
#include "boy.h"
#define MAN_TYPE  (man_get_type())
#define MAN(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj),MAN_TYPE,Man))
typedef struct _Man Man;
typedef struct _ManClass ManClass;
struct _Man {
    Boy parent;
    char *job;
    void (*bye)(void);
};
struct _ManClass {
    BoyClass parent_class;
};
GType man_get_type(void);
Man*  man_new(void);
gchar* man_get_gob(Man *man);
void  man_set_job(Man *man, gchar *job);
Man*  man_new_with_name_age_and_job(gchar *name, gint age, gchar *job);
void man_info(Man *man);
#endif //__MAN_H__
