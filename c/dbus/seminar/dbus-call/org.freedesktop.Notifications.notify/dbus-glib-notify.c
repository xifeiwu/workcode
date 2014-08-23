#include <glib.h> 
#include <gio/gio.h> 
#include <stdio.h> 

#define NOTIFY_OBJECT "/org/freedesktop/Notifications" 
#define NOTIFY_INTERFACE "org.freedesktop.Notifications" 

guint32 notify_sync(   GDBusProxy* proxy, 
         const gchar* app_name, 
         guint32 replaces_id, 
         const gchar* app_icon, 
         const gchar* summary, 
         const gchar* body, 
         const gchar* const *actions, 
         GVariant* hints, 
         guint32 expire_timeout, 
         GError** err) 
{ 
   GVariant* result; 
   GVariantBuilder b; 
   guint32 out; 

   g_variant_builder_init(&b,G_VARIANT_TYPE_ARRAY); 
   for(int i=0;actions[i];i++) 
      g_variant_builder_add(&b,"s",actions[i]); 

   result = g_dbus_proxy_call_sync( 
      proxy, 
      "Notify", 
      g_variant_new("(susssas@a{sv}i)", 
         app_name, 
         replaces_id, 
         app_icon, 
         summary, 
         body, 
         &b, 
         hints, 
         expire_timeout), 
      G_DBUS_CALL_FLAGS_NONE, 
      -1, 
      NULL, 
      err); 

   out=g_variant_get_uint32(g_variant_get_child_value(result,0)); 
   g_variant_unref(result);    

   return out; 
} 

int main(int argc, char** argv) 
{ 
   GDBusConnection* conn; 
   GDBusProxy* proxy; 
   GError* err = NULL; 
   const gchar *const notify_actions[3] = {"v1","v2",NULL}; 
     GVariant *notify_hints, *notify_entry[1]; 

   g_type_init(); 

   conn = g_bus_get_sync(G_BUS_TYPE_SESSION,NULL,&err); 

   if(err) 
   { 
      fprintf(stderr,"g_bus_get_sync: %s\n",err->message); 
      g_error_free(err); 
      return 1; 
   } 

   proxy = g_dbus_proxy_new_sync(  conn, 
               G_DBUS_PROXY_FLAGS_NONE, 
               NULL, 
               NOTIFY_INTERFACE, 
               NOTIFY_OBJECT, 
               NOTIFY_INTERFACE, 
               NULL, 
               &err); 

   if(err) 
   { 
      fprintf(stderr,"g_dbus_proxy_new_sync: %s\n",err->message); 
      g_error_free(err); 
      return 1; 
   } 

   notify_entry[0] = g_variant_new_dict_entry( 
               g_variant_new_string("foo"), 
               g_variant_new_variant( 
                  g_variant_new_uint32(42) 
               ) 
            ); 

   notify_hints = g_variant_new_array(G_VARIANT_TYPE("{sv}"), 
            notify_entry, 
            G_N_ELEMENTS(notify_entry) 
         ); 

   notify_sync(   proxy, 
         "app_name", 
         -1, 
         "/home/rhiakath/.dropbox-dist/icons/emblems/emblem-dropbox-syncing.png", 
         "summary", 
         "body", 
         notify_actions, 
         notify_hints, 
         -1, 
         &err 
      ); 

   if(err) 
   { 
      fprintf(stderr,"notify_sync: %s\n",err->message); 
      g_error_free(err); 
      return 1; 
   } 

   g_object_unref(proxy); 
   g_object_unref(conn); 
   for(int i=0; i<G_N_ELEMENTS(notify_entry); i++) 
      g_variant_unref(notify_entry[i]); 
   g_variant_unref(notify_hints);    

   return 0; 
} 

