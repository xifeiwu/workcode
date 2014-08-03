#include <dbus/dbus-glib.h>
#include <stdio.h>
#include <stdlib.h>

static void lose (const char *fmt, ...) G_GNUC_NORETURN G_GNUC_PRINTF (1, 2);
static void lose_gerror (const char *prefix, GError *error) G_GNUC_NORETURN;

static void
lose (const char *str, ...)
{
  va_list args;

  va_start (args, str);

  vfprintf (stderr, str, args);
  fputc ('\n', stderr);

  va_end (args);

  exit (1);
}

static void
lose_gerror (const char *prefix, GError *error) 
{
  lose ("%s: %s", prefix, error->message);
}

typedef struct TestObj TestObj;
typedef struct TestObjClass TestObjClass;

GType test_obj_get_type (void);

struct TestObj
{
  GObject parent;
};

struct TestObjClass
{
  GObjectClass parent;
};

#define TEST_TYPE_OBJECT              (test_obj_get_type ())
#define TEST_OBJECT(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), TEST_TYPE_OBJECT, TestObj))
#define TEST_OBJECT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TEST_TYPE_OBJECT, TestObjClass))
#define TEST_IS_OBJECT(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), TEST_TYPE_OBJECT))
#define TEST_IS_OBJECT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TEST_TYPE_OBJECT))
#define TEST_OBJECT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TEST_TYPE_OBJECT, TestObjClass))

G_DEFINE_TYPE(TestObj, test_obj, G_TYPE_OBJECT)

gboolean test_obj_add (TestObj *obj, int num1, int num2, int *sum, GError **error);

#include "example-service-glue.h"

static void test_obj_init (TestObj *obj)
{
}

static void test_obj_class_init (TestObjClass *klass)
{
}

gboolean test_obj_add (TestObj *obj, int num1, int num2, int *sum, GError **error)
{
  *sum = num1 + num2;
  return TRUE;
}

int main (int argc, char **argv)
{
  DBusGConnection *bus;
  DBusGProxy *bus_proxy;
  GError *error = NULL;
  TestObj *obj;
  GMainLoop *mainloop;
  guint request_name_result;

  g_type_init ();

  {
    GLogLevelFlags fatal_mask;
    
    fatal_mask = g_log_set_always_fatal (G_LOG_FATAL_MASK);
    fatal_mask |= G_LOG_LEVEL_WARNING | G_LOG_LEVEL_CRITICAL;
    g_log_set_always_fatal (fatal_mask);
  }
  
  dbus_g_object_type_install_info (TEST_TYPE_OBJECT, &dbus_glib_test_obj_object_info);

  mainloop = g_main_loop_new (NULL, FALSE);

  bus = dbus_g_bus_get (DBUS_BUS_SESSION, &error);
  if (!bus)
    lose_gerror ("Couldn't connect to session bus", error);

  bus_proxy = dbus_g_proxy_new_for_name (bus, "org.freedesktop.DBus",
					 "/org/freedesktop/DBus",
					 "org.freedesktop.DBus");

  if (!dbus_g_proxy_call (bus_proxy, "RequestName", &error,
			  G_TYPE_STRING, "org.fmddlmyy.Test",
			  G_TYPE_UINT, 0,
			  G_TYPE_INVALID,
			  G_TYPE_UINT, &request_name_result,
			  G_TYPE_INVALID))
    lose_gerror ("Failed to acquire org.fmddlmyy.Test", error);

  obj = g_object_new (TEST_TYPE_OBJECT, NULL);

  dbus_g_connection_register_g_object (bus, "/TestObj", G_OBJECT (obj));

  printf ("service running\n");

  g_main_loop_run (mainloop);

  exit (0);
}
