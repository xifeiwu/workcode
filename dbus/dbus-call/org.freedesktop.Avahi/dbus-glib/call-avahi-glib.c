#include <dbus/dbus-glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <dbus/dbus.h>
#include "marshal.h"

static void lose(const char *fmt, ...)
G_GNUC_NORETURN G_GNUC_PRINTF(1, 2);
static void lose_gerror(const char *prefix, GError *error)
G_GNUC_NORETURN;

static void lose(const char *str, ...) {
    va_list args;
    va_start(args, str);
    vfprintf(stderr, str, args);
    fputc('\n', stderr);
    va_end(args);
    exit(1);
}

static void lose_gerror(const char *prefix, GError *error) {
    lose("%s: %s", prefix, error->message);
}

static void print_hash_value(gpointer key, gpointer val, gpointer data) {
    printf("%s -> %s\n", (char *) key, (char *) val);
}

#define DBUS_TYPE_G_UCHAR_ARRAY_ARRAY    (dbus_g_type_get_collection ("GPtrArray", DBUS_TYPE_G_UCHAR_ARRAY ))

DBusGProxy *proxy_avahi_service, *proxy_avahi_service_browser;

static void service_browser_ItemNew(DBusGProxy *proxy, int interface, int protocol, char *name,
        char *stype, char *domain, unsigned int flags, gpointer user_data) {
    GError *error = NULL;
    //printf ("discovered:%d, %d, %s, %s, %s, %d.\n", interface, protocol, name, stype, domain, flags);

    char *host, *address;
    dbus_int32_t aprotocol = -1;
    dbus_uint32_t m_flags = 0;
    dbus_uint16_t port;
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
    printf("\n");
    printf("Parameter for ResolvesService:%d, %d, %s, %s, %s, %d, %d.\n",
            interface, protocol, name, stype, domain, aprotocol, m_flags);
    if (!dbus_g_proxy_call(proxy_avahi_service, "ResolveService", &error, G_TYPE_INT,
            interface, G_TYPE_INT, protocol, G_TYPE_STRING, name, G_TYPE_STRING,
            stype, G_TYPE_STRING, domain, G_TYPE_INT, aprotocol, G_TYPE_UINT,
            m_flags, G_TYPE_INVALID, G_TYPE_INT, &interface, G_TYPE_INT,
            &protocol, G_TYPE_STRING, &name, G_TYPE_STRING, &stype,
            G_TYPE_STRING, &domain, G_TYPE_STRING, &host, G_TYPE_INT,
            &aprotocol, G_TYPE_STRING, &address, G_TYPE_UINT, &port,
            DBUS_TYPE_G_UCHAR_ARRAY_ARRAY, &byte_arraies, G_TYPE_UINT, &flags,
            G_TYPE_INVALID)) {
        lose_gerror("Failed to call ServiceBrowserNew", error);
    }
    printf(
            "Results of ResolveService:%d, %d, %s, %s, %s, %s, %d, %s, %d, %d.\n",
            interface, protocol, name, stype, domain, host, aprotocol, address,
            port, flags);
    if (byte_arraies->len == 0) {
        printf("value of txt: %s\n", "is null.");
    } else {
        for (int i = 0; i < byte_arraies->len; i++) {
            GArray *arr = g_ptr_array_index(byte_arraies, i);
            char *txt = malloc(sizeof(char) * (arr->len + 1));
            strncpy(txt, arr->data, arr->len);txt[arr->len] = '\0';
            //arr = g_ptr_array_index(byte_arraies, 0);
            printf("value of txt: %d, %s\n", arr->len, txt);
        }
    }
}
static void service_browser_ItemRemove(DBusGProxy *proxy, int interface, int protocol,
        char *name, char *stype, char *domain, unsigned int flags,
        gpointer user_data) {
    printf("removed:%d, %d, %s, %s, %s, %d.\n", interface, protocol, name,
            stype, domain, flags);
}
static void start_avahi_service_browser(DBusGConnection *bus_system){
    GError *error = NULL;
    char *service_browser_path;
    if (!dbus_g_proxy_call(proxy_avahi_service, "ServiceBrowserNew", &error,
            G_TYPE_INT, -1, G_TYPE_INT, -1, G_TYPE_STRING, "_http._tcp",
            G_TYPE_STRING, "local", G_TYPE_UINT, 0, G_TYPE_INVALID,
            DBUS_TYPE_G_OBJECT_PATH, &service_browser_path, G_TYPE_INVALID))
        lose_gerror("Failed to call ServiceBrowserNew", error);

    printf("new service browser (%s) has started.\n", service_browser_path);

    proxy_avahi_service_browser = dbus_g_proxy_new_for_name(bus_system,
            "org.freedesktop.Avahi", service_browser_path,
            "org.freedesktop.Avahi.ServiceBrowser");
    dbus_g_object_register_marshaller(
            _avahi_marshal_VOID__INT_INT_STRING_STRING_STRING_UINT, G_TYPE_NONE,
            G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
            G_TYPE_UINT, G_TYPE_INVALID);
//    dbus_g_object_register_marshaller(
//            _avahi_marshal_VOID__INT_INT_STRING_STRING_STRING_INT_UINT,
//            G_TYPE_NONE, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING,
//            G_TYPE_STRING, G_TYPE_INT, G_TYPE_UINT, G_TYPE_INVALID);
    dbus_g_proxy_add_signal(proxy_avahi_service_browser, "ItemNew", G_TYPE_INT,
            G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
            G_TYPE_UINT, G_TYPE_INVALID);
    dbus_g_proxy_connect_signal(proxy_avahi_service_browser, "ItemNew",
            G_CALLBACK(service_browser_ItemNew), NULL, NULL);
    dbus_g_proxy_add_signal(proxy_avahi_service_browser, "ItemRemove", G_TYPE_INT,
            G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
            G_TYPE_UINT, G_TYPE_INVALID);
    dbus_g_proxy_connect_signal(proxy_avahi_service_browser, "ItemRemove",
            G_CALLBACK(service_browser_ItemRemove), NULL, NULL);    
}


static void entry_group_AddService();
DBusGProxy *proxy_avahi_entry_group;
static void start_avahi_entry_group(DBusGConnection *bus_system)
{
    GError *error = NULL;
    char *entry_group_path;
    if (!dbus_g_proxy_call(proxy_avahi_service, "EntryGroupNew", &error, G_TYPE_INVALID,
            DBUS_TYPE_G_OBJECT_PATH, &entry_group_path, G_TYPE_INVALID))
        lose_gerror("Failed to call EntryGroupNew", error);

    printf("new entry group (%s) has started.\n", entry_group_path);

    proxy_avahi_entry_group = dbus_g_proxy_new_for_name(bus_system,
            "org.freedesktop.Avahi", entry_group_path,
            "org.freedesktop.Avahi.EntryGroup");
    entry_group_AddService();
    /*
    dbus_g_proxy_add_signal(proxy_avahi_service_browser, "ItemNew", G_TYPE_INT,
            G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
            G_TYPE_UINT, G_TYPE_INVALID);
    dbus_g_proxy_connect_signal(proxy_avahi_service_browser, "ItemNew",
            G_CALLBACK(service_browser_ItemNew), NULL, NULL);
    dbus_g_proxy_add_signal(proxy_avahi_service_browser, "ItemRemove", G_TYPE_INT,
            G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
            G_TYPE_UINT, G_TYPE_INVALID);
    dbus_g_proxy_connect_signal(proxy_avahi_service_browser, "ItemRemove",
            G_CALLBACK(service_browser_ItemRemove), NULL, NULL);    
            */
}
static void entry_group_AddService(){
    GError *error = NULL;
    dbus_int32_t interface = -1;
    dbus_int32_t protocol = -1;
    dbus_uint32_t flags = 0;
    char *name = "xifei";
    char *type = "_http._tcp";
    char *domain = "local";
    char *host = "192.168.160.3";
    dbus_uint16_t port = 500;
    char *txt1 = "xifei";
    char *txt2 = "password of xifei";

    GPtrArray *txt = g_ptr_array_new();
    GArray *garr;
    garr = g_array_new(FALSE, FALSE, sizeof(char));
    garr->data = txt1;
    garr->len = strlen(txt1);
    g_ptr_array_add(txt, garr);
    garr = g_array_new(FALSE, FALSE, sizeof(char));
    garr->data = txt2;
    garr->len = strlen(txt2);
    g_ptr_array_add(txt, garr);
    
   // initialiset the errors
    printf("Calling remote method: %s\n", "org.freedesktop.Avahi.EntryGroup");

    if (!dbus_g_proxy_call(proxy_avahi_entry_group, "AddService", &error,
            G_TYPE_INT, interface,
            G_TYPE_INT, protocol,
            G_TYPE_UINT, flags,
            G_TYPE_STRING, name,
            G_TYPE_STRING, type,
            G_TYPE_STRING, domain,
            G_TYPE_STRING, host,
            G_VARIANT_CLASS_UINT16, port,//G_TYPE_UCAHRDBUS_TYPE_UINT16
            DBUS_TYPE_G_UCHAR_ARRAY_ARRAY, txt,
            G_TYPE_INVALID,
            G_TYPE_INVALID))
        lose_gerror("Failed to call ServiceBrowserNew", error);

    if (!dbus_g_proxy_call(proxy_avahi_entry_group, "Commit", &error,
            G_TYPE_INVALID,
            G_TYPE_INVALID))
        lose_gerror("Failed to call ServiceBrowserNew", error);
}

int main(int argc, char **argv) {
    DBusGConnection *bus_system;
    GError *error = NULL;
    GMainLoop *mainloop;

    g_type_init();

    {
        GLogLevelFlags fatal_mask;

        fatal_mask = g_log_set_always_fatal(G_LOG_FATAL_MASK);
        fatal_mask |= G_LOG_LEVEL_WARNING | G_LOG_LEVEL_CRITICAL;
        g_log_set_always_fatal(fatal_mask);
    }

    mainloop = g_main_loop_new(NULL, FALSE);

    bus_system = dbus_g_bus_get(DBUS_BUS_SYSTEM, &error); //DBUS_BUS_SESSION
    if (!bus_system)
        lose_gerror("Couldn't connect to bus_system", error);

    proxy_avahi_service = dbus_g_proxy_new_for_name(bus_system, "org.freedesktop.Avahi", "/",
            "org.freedesktop.Avahi.Server");
    
    start_avahi_service_browser(bus_system);
//    start_avahi_entry_group(bus_system);
    g_main_loop_run(mainloop);

    g_object_unref(G_OBJECT(proxy_avahi_service));
    g_object_unref(G_OBJECT(proxy_avahi_service_browser));
    exit(0);
}
