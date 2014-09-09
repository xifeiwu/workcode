#include <gio/gio.h>
#include <stdio.h>
#include <stdlib.h>
void iterate_container_recursive(GVariant *container) {
    g_print("container type: %s\n", g_variant_get_type_string(container));
    GVariantIter iter;
    GVariant *child;
    g_variant_iter_init(&iter, container);
    while ((child = g_variant_iter_next_value(&iter))) {
        g_print("type '%s' , %s\n", g_variant_get_type_string(child), (g_variant_is_container(child)? "is container":"is not a container"));
        if (g_variant_is_container(child)){
            iterate_container_recursive(child);
            //g_print("is a container\n");
        }
        //else{
            //g_print("is not a container\n");            
        //}
        g_variant_unref(child);
    }
}
GDBusProxy *proxy_avahi_service;
GDBusProxy *proxy_avahi_service_browser;
static void avahi_service_browser_on_signal(GDBusProxy  *proxy,
        const gchar *sender_name,
        const gchar *signal_name,
        GVariant    *parameters,
        gpointer     user_data)
{
    g_message("sender_name: %s, signal_name: %s, tpye: %s", sender_name, signal_name, g_variant_get_type_string(parameters));
    int interface = 0;
    int protocol = 0;
    char *name = "";
    char *stype = "";
    char *domain = "";
    unsigned int flags = 0;
    if (g_strcmp0 (signal_name, "ItemNew") == 0){
        g_variant_get (parameters, "(iisssu)",  &interface, &protocol, &name, &stype, &domain, &flags);
//        printf ("ItemNew: %d, %d, %s, %s, %s, %d.\n", interface, protocol, name, stype, domain, flags);
        GVariant *result;
        GError *error = NULL;
        char *host;
        int aprotocol = 0;
        char *address;
        guint16 port = 0;
        //GVariant *aay;
//        GPtrArray *byte_arraies;
        GVariantIter aay_iter;
        GVariant *aay ;//= g_variant_new("aay");
        result = g_dbus_proxy_call_sync (
                    proxy_avahi_service,
                    "ResolveService",
                    g_variant_new ("(iisssiu)", interface, protocol, name, stype, domain, -1, 0),
                    G_DBUS_CALL_FLAGS_NONE,
                     -1,
                    NULL,
                    &error);
        g_assert_no_error (error);
        g_assert (result != NULL);
        g_assert_cmpstr (g_variant_get_type_string (result), ==, "(iissssisqaayu)");
//        g_message("ResolveService result type: %s", g_variant_get_type_string (result));
        
        g_variant_get (result, "(iissssisq@aayu)",  &interface, &protocol, &name, &stype, &domain, &host,
                &aprotocol, &address, &port, &aay, &flags);
        g_printf("Results of ResolveService:%d, %d, %s, %s, %s, %s, %d, %s, %d, %d.\n",
                interface, protocol, name, stype, domain, host, aprotocol, address,
                port, flags);

        //iterate_container_recursive(aay);
        GVariantIter *y_iter;
        guchar y_i;
        unsigned char *value;
        GVariant *child;
//        g_message("size of aay1: %d\n", g_variant_n_children (aay));
        g_message("size of aay2: %d\n", g_variant_iter_init (&aay_iter, aay));
        while ((child = g_variant_iter_next_value(&aay_iter))) {
            g_print("type of child: %s\n", g_variant_get_type_string(child));
            if (g_variant_is_of_type (child, G_VARIANT_TYPE_BYTESTRING)){g_printf("is a bytestring type");};
            g_variant_get(child, "ay", &y_iter);
            while (g_variant_iter_loop(y_iter, "y", &y_i)) {
                g_print("value: %c\n", y_i);
            }
//            g_variant_get(child, "ay", &value);
//            g_print("value: %s\n", value);
            }
        //iterate_container_recursive(result);
//        while (g_variant_iter_next (&aay_iter, "ay", &key, &value)){
//        GVariant *child;
//        while ((child = g_variant_iter_next_value (aay_iter))){
//            printf("type of child: %s ", g_variant_get_type_string(child));
//        }

        
//        gchar **arr = g_variant_get_bytestring_array(aay, NULL);
        /*
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
            */
        
    }else if(g_strcmp0 (signal_name, "ItemRemove") == 0){
        g_variant_get (parameters, "(iisssu)",  &interface, &protocol, &name, &stype, &domain, &flags);
        printf ("ItemRemove: %d, %d, %s, %s, %s, %d.\n", interface, protocol, name, stype, domain, flags);
    }
}
static void start_avahi_service_browser(char *path){
    GVariant *result;
    GError *error = NULL;
//    printf("proxy_avahi_service1: %d\n", proxy_avahi_service_browser);
    
    proxy_avahi_service_browser = g_dbus_proxy_new_for_bus_sync (
                G_BUS_TYPE_SYSTEM,
                G_DBUS_PROXY_FLAGS_DO_NOT_LOAD_PROPERTIES,
                NULL,                                               /* GDBusInterfaceInfo */
                "org.freedesktop.Avahi",                            /* name */
                path,                                               /* object path */
                "org.freedesktop.Avahi.ServiceBrowser",             /* interface */
                NULL,                                               /* GCancellable */
                &error);
    g_assert_no_error (error);
//    printf("proxy_avahi_service2: %d\n", proxy_avahi_service_browser);
//sleep(1);
    gulong signal_handler_id;
    GString *data = g_string_new (NULL);
    signal_handler_id = g_signal_connect (proxy_avahi_service_browser,
                                          "g-signal",
                                          G_CALLBACK (avahi_service_browser_on_signal),
                                          data);
}
int main (int argc, char *argv[])
{
    GMainLoop *mainloop;
    g_type_init();
    g_test_init (&argc, &argv, NULL);

    /* all the tests rely on a shared main loop */
    mainloop = g_main_loop_new (NULL, FALSE);
    GError *error;
    error = NULL;
    proxy_avahi_service = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
                                           G_DBUS_PROXY_FLAGS_DO_NOT_LOAD_PROPERTIES,
                                           NULL,                            /* GDBusInterfaceInfo */
                                           "org.freedesktop.Avahi",         /* name */
                                           "/",                             /* object path */
                                           "org.freedesktop.Avahi.Server",  /* interface */
                                           NULL, /* GCancellable */
                                           &error);
    g_assert_no_error (error);
    
    char *service_browser_path;
    GVariant *result;
    result = g_dbus_proxy_call_sync (
                proxy_avahi_service,
                "ServiceBrowserNew",
                g_variant_new ("(iissu)", -1, -1, "_http._tcp", "local", 0),
                G_DBUS_CALL_FLAGS_NONE,
                 -1,
                NULL,
                &error);
    g_assert_no_error (error);
    g_assert (result != NULL);
    g_assert_cmpstr (g_variant_get_type_string (result), ==, "(o)");
    g_variant_get (result, "(o)", &service_browser_path);
    g_message("new service browser (%s) has started.\n", service_browser_path);
    
    start_avahi_service_browser(service_browser_path);
    
    g_main_loop_run(mainloop);
    exit(0);
}