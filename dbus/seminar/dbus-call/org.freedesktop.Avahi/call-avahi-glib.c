#include <dbus/dbus-glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <dbus/dbus.h>
#include "marshal.h"

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

static void
print_hash_value (gpointer key, gpointer val, gpointer data)
{
  printf ("%s -> %s\n", (char *) key, (char *) val);
}

DBusGProxy *avahi_service, *avahi_service_browser;
#define DBUS_TYPE_G_UCHAR_ARRAY_ARRAY    (dbus_g_type_get_collection ("GPtrArray", DBUS_TYPE_G_UCHAR_ARRAY ))

static void
item_new (DBusGProxy *proxy, int interface, int protocol, char *name, char *stype, char *domain, unsigned int flags, gpointer user_data)
{
  GError *error = NULL;
  printf ("discovered:%d, %d, %s, %s, %s, %d.\n", interface, protocol, name, stype, domain, flags);

  char *host, *address;
  dbus_int32_t aprotocol = -1;
  dbus_uint32_t m_flags = 0;
  dbus_uint16_t port;//gint16
  GPtrArray *byte_arraies;
/*
    dbus_int32_t minterface = 2;
    dbus_int32_t mprotocol = 1;
    char *mname = "TestService";
    char *mstype = "_http._tcp";
    char *mdomain = "local";
    dbus_int32_t maprotocol = -1;
    dbus_uint32_t mflags = 0;
*/
  printf ("parameter for ResolvesService:%d, %d, %s, %s, %s, %d, %d.\n", interface, protocol, name, stype, domain, aprotocol, m_flags);
  if (!dbus_g_proxy_call (avahi_service, "ResolveService", &error,
                G_TYPE_INT, interface,
                G_TYPE_INT, protocol, 
                G_TYPE_STRING, name,
                G_TYPE_STRING, stype,
                G_TYPE_STRING, domain,
                G_TYPE_INT, aprotocol, 
                G_TYPE_UINT, m_flags, 
                G_TYPE_INVALID, 
                G_TYPE_INT, &interface,
                G_TYPE_INT, &protocol, 
                G_TYPE_STRING, &name,
                G_TYPE_STRING, &stype,
                G_TYPE_STRING, &domain,
                G_TYPE_STRING, &host,
                G_TYPE_INT, &aprotocol, 
                G_TYPE_STRING, &address,
                G_TYPE_UINT, &port, 
                DBUS_TYPE_G_UCHAR_ARRAY_ARRAY, &byte_arraies,
                G_TYPE_UINT, &flags, 
                G_TYPE_INVALID
    ))
	{
    lose_gerror ("Failed to call ServiceBrowserNew", error);
	}
  printf ("parameter for ResolvesService:%d, %d, %s, %s, %s, %s, %d, %s, %d, %d.\n", interface, protocol, name, stype, domain, host, aprotocol, address, port,  flags);
//gpointer* tmp = g_ptr_array_index (byte_arraies, 0);
//printf("tmp: %s", tmp);
/*
  DBusConnection* conn;
  DBusMessage* msg;
  DBusMessageIter args;
  DBusPendingCall* pending;
  DBusError err;
  conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
  msg = dbus_message_new_method_call("org.freedesktop.Avahi",           // target for the method call
                                    "/", 							                     // object to call on
                                    "org.freedesktop.Avahi.Server",     // interface to call on
                                    "ResolveService");                  // method nameResolveHostName
  if (NULL == msg) 
   { 
      fprintf(stderr, "Message Null\n"); 
      exit(1); 
   }
  dbus_message_iter_init_append(msg, &args);
  if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &interface)) {
      fprintf(stderr, "Out Of Memory!\n"); 
      exit(1);
  }
  if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &protocol)) {
      fprintf(stderr, "Out Of Memory!\n"); 
      exit(1);
  }
  if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, name)) {
      fprintf(stderr, "Out Of Memory!\n"); 
      exit(1);
  }
  if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, stype)) {
      fprintf(stderr, "Out Of Memory!\n"); 
      exit(1);
  }
  if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, domain)) {
      fprintf(stderr, "Out Of Memory!\n"); 
      exit(1);
  }
  if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &aprotocol)) {
      fprintf(stderr, "Out Of Memory!\n"); 
      exit(1);
  }
  if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &flags)) {
      fprintf(stderr, "Out Of Memory!\n"); 
      exit(1);
  }
   if (!dbus_connection_send_with_reply (conn, msg, &pending, -1)) { // -1 is default timeout
      fprintf(stderr, "Out Of Memory!\n"); 
      exit(1);
   }
   if (NULL == pending) { 
      fprintf(stderr, "Pending Call Null\n"); 
      exit(1); 
   }
   dbus_connection_flush(conn);
   
   printf("Request Sent\n");
   
   // free message
   dbus_message_unref(msg);
   
   // block until we recieve a reply
   dbus_pending_call_block(pending);

   // get the reply message
   msg = dbus_pending_call_steal_reply(pending);
   if (NULL == msg) {
      fprintf(stderr, "Reply Null\n"); 
      exit(1); 
   }
   // free the pending message handle
   dbus_pending_call_unref(pending);
*/


/*
  if (!dbus_g_proxy_call (avahi_service, "ResolveHostName", &error,
				G_TYPE_INT, interface,
				G_TYPE_INT, protocol, 
				G_TYPE_STRING, name,
				G_TYPE_INT, -1, 
				G_TYPE_UINT, 0, 
            G_TYPE_INVALID, 
				G_TYPE_INT, &interface,
				G_TYPE_INT, &protocol, 
				G_TYPE_STRING, name,
				G_TYPE_INT, &aprotocol, 
				G_TYPE_STRING, address,
				G_TYPE_UINT, &flags, 
				G_TYPE_INVALID))
	{
    lose_gerror ("Failed to call ResolveHostName", error);
	}
*/
}
static void
item_remove (DBusGProxy *proxy, int interface, int protocol, char *name, char *stype, char *domain, unsigned int flags, gpointer user_data)
{
  printf ("removed:%d, %d, %s, %s, %s, %d.\n", interface, protocol, name, stype, domain, flags);
}
int
main (int argc, char **argv)
{
  DBusGConnection *bus;
  DBusGProxy *remote_object_introspectable;
  GError *error = NULL;
  char *introspect_data;
  char *service_browser_path;
  guint i;
  GMainLoop *mainloop;

  g_type_init ();

  {
    GLogLevelFlags fatal_mask;
    
    fatal_mask = g_log_set_always_fatal (G_LOG_FATAL_MASK);
    fatal_mask |= G_LOG_LEVEL_WARNING | G_LOG_LEVEL_CRITICAL;
    g_log_set_always_fatal (fatal_mask);
  }

  mainloop = g_main_loop_new (NULL, FALSE);

  bus = dbus_g_bus_get (DBUS_BUS_SYSTEM, &error);//DBUS_BUS_SESSION
  if (!bus)
    lose_gerror ("Couldn't connect to session bus", error);

  avahi_service = dbus_g_proxy_new_for_name (bus,
					     "org.freedesktop.Avahi",
					             "/",
					     "org.freedesktop.Avahi.Server");
  if (!dbus_g_proxy_call (avahi_service, "ServiceBrowserNew", &error,
				G_TYPE_INT, -1,
				G_TYPE_INT, -1, 
				G_TYPE_STRING, "_http._tcp",
				G_TYPE_STRING, "local",
				G_TYPE_UINT, 0, 
            G_TYPE_INVALID, DBUS_TYPE_G_OBJECT_PATH, &service_browser_path, G_TYPE_INVALID))
    lose_gerror ("Failed to call ServiceBrowserNew", error);

  printf("new service browser (%s) has started.\n", service_browser_path);

  avahi_service_browser = dbus_g_proxy_new_for_name (bus,
					     "org.freedesktop.Avahi",
					     service_browser_path,
					     "org.freedesktop.Avahi.ServiceBrowser");
  dbus_g_object_register_marshaller (_avahi_marshal_VOID__INT_INT_STRING_STRING_STRING_UINT, G_TYPE_NONE, 
					G_TYPE_INT, 
					G_TYPE_INT, 
					G_TYPE_STRING, 
					G_TYPE_STRING, 
					G_TYPE_STRING, 
					G_TYPE_UINT, 
					G_TYPE_INVALID);
  dbus_g_object_register_marshaller (_avahi_marshal_VOID__INT_INT_STRING_STRING_STRING_INT_UINT, G_TYPE_NONE, 
					G_TYPE_INT, 
					G_TYPE_INT, 
					G_TYPE_STRING, 
					G_TYPE_STRING, 
					G_TYPE_STRING, 
					G_TYPE_INT, 
					G_TYPE_UINT, 
					G_TYPE_INVALID);
  dbus_g_proxy_add_signal (avahi_service_browser, "ItemNew", 
					G_TYPE_INT, 
					G_TYPE_INT, 
					G_TYPE_STRING, 
					G_TYPE_STRING, 
					G_TYPE_STRING, 
					G_TYPE_UINT,
					G_TYPE_INVALID);
  dbus_g_proxy_connect_signal (avahi_service_browser, "ItemNew", G_CALLBACK (item_new),
 		       NULL, NULL);
  dbus_g_proxy_add_signal (avahi_service_browser, "ItemRemove", G_TYPE_INT, G_TYPE_INT, 
G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_UINT, G_TYPE_INVALID);
  dbus_g_proxy_connect_signal (avahi_service_browser, "ItemRemove", G_CALLBACK (item_remove),
 		       NULL, NULL);


//  avahi_service_introspectable = dbus_g_proxy_new_for_name (bus,
//							    "org.freedesktop.Avahi",
//							    "/",
//							    "org.freedesktop.DBus.Introspectable");
//  if (!dbus_g_proxy_call (avahi_service_introspectable, "Introspect", &error,
//			  G_TYPE_INVALID,
//			  G_TYPE_STRING, &introspect_data, G_TYPE_INVALID))
//    lose_gerror ("Failed to complete Introspect", error);
//  printf ("%s", introspect_data);
//  g_free (introspect_data);
//
//  g_object_unref (G_OBJECT (avahi_service_introspectable));

  g_main_loop_run (mainloop);

  g_object_unref (G_OBJECT (avahi_service));
  g_object_unref (G_OBJECT (avahi_service_browser));
  exit(0);
}
