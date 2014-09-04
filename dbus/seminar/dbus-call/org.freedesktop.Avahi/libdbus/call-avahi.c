/*
* Example low-level D-Bus code.
* Written by xifei
*/
#include <dbus/dbus.h>
#include <glib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
char* get_service_browser_path()
{
    DBusMessage* msg;
    DBusMessageIter args;
    DBusConnection* conn;
    DBusError err;
    dbus_error_init(&err);
    DBusPendingCall* pending;
    dbus_int32_t interface = -1;
    dbus_int32_t protocol = -1;
    char *type = "_http._tcp";
    char *domain = "local";
    dbus_uint32_t flags = 0;
    char *service_browser_path;

   // initialiset the errors
    printf("Calling remote method: %s\n", "org.freedesktop.Avahi");

   // connect to the system bus and check for errors
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if (dbus_error_is_set(&err)) { 
      fprintf(stderr, "Connection Error (%s)\n", err.message); 
      dbus_error_free(&err);
    }
    if (NULL == conn) { 
      exit(1);
    }

    msg = dbus_message_new_method_call("org.freedesktop.Avahi",// target for the method call
                                        "/", 							// object to call on
                                        "org.freedesktop.Avahi.Server",    // interface to call on
                                        "ServiceBrowserNew");             // method nameResolveHostName
    if (NULL == msg) 
     { 
      fprintf(stderr, "Message Null\n"); 
      exit(1); 
    }
   // append arguments
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &interface))
    {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &protocol))
    {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &type))
    {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &domain))
    {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &flags))
    {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
      
   // send message and get a handle for a reply
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

    // read the parameters
    if (!dbus_message_iter_init(msg, &args))
        fprintf(stderr, "Message has no arguments!\n"); 
    else if (DBUS_TYPE_OBJECT_PATH != dbus_message_iter_get_arg_type(&args)) 
        fprintf(stderr, "Argument is not boolean!\n"); 
    else
        dbus_message_iter_get_basic(&args, &service_browser_path);


    printf("Got Reply: %s\n", service_browser_path);
    dbus_message_unref(msg);  
    return service_browser_path;
}
char* resolve_service(int interface, int protocol, char *name, char *type, char *domain)
{
    DBusMessage* msg;
    DBusMessageIter args;
    DBusConnection* conn;
    DBusError err;
    dbus_error_init(&err);
    DBusPendingCall* pending;
    //dbus_int32_t interface = 2;
    //dbus_int32_t protocol = 1;
    //char *name = "TestService";
    //char *type = "_http._tcp";
    //char *domain = "local";
    dbus_int32_t aprotocol = -1;
    dbus_uint32_t flags = 0;
    printf ("parameter for ResolvesService:%d, %d, %s, %s, %s, %d, %d.\n", interface, protocol, name, type, domain, aprotocol, flags);

   // initialiset the errors
    printf("Calling remote method: %s\n", "org.freedesktop.Avahi");

   // connect to the system bus and check for errors
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if (dbus_error_is_set(&err)) { 
      fprintf(stderr, "Connection Error (%s)\n", err.message); 
      dbus_error_free(&err);
    }
    if (NULL == conn) { 
      exit(1);
    }

    msg = dbus_message_new_method_call("org.freedesktop.Avahi",// target for the method call
                                        "/", 							// object to call on
                                        "org.freedesktop.Avahi.Server",    // interface to call on
                                        "ResolveService");             // method nameResolveHostName
    if (NULL == msg) 
     { 
      fprintf(stderr, "Message Null\n"); 
      exit(1); 
    }
   // append arguments
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &interface))
    {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &protocol))
    {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &name))
    {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &type))
    {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &domain))
    {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &aprotocol))
    {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &flags))
    {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
      
   // send message and get a handle for a reply
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


    char *address;
    char *host;
    dbus_uint16_t port;//gint16
    GPtrArray *byte_arraies;
    dbus_uint32_t m_flags;
    if (!dbus_message_iter_init(msg, &args))
        g_message("dbus_message_iter_init fail\n");
    else
    { 
//g_message("signature of container args: %s", dbus_message_iter_get_signature(&args));
//g_message("first type: %d", dbus_message_iter_get_arg_type(&args));
//g_message("DBUS_TYPE_INT32: %d", DBUS_TYPE_INT32);
//g_message("DBUS_TYPE_STRING: %d", DBUS_TYPE_STRING);
//char *value;
//dbus_message_iter_get_basic(&args, &value);
//g_message("DBUS_TYPE_STRING: %s", value);

        if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args)) 
            g_message( "Argument is not error!\n"); 
        else{
            dbus_message_iter_get_basic(&args, &interface);
            printf("interface : %d\n", interface); 
        }

        if (!dbus_message_iter_next(&args))
            g_message( "Message has too few arguments!\n"); 
        else if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args)) 
            g_message("Argument is not error!\n"); 
        else{
            dbus_message_iter_get_basic(&args, &protocol);
            printf("protocol : %d\n", protocol); 
        }

        if (!dbus_message_iter_next(&args))
            g_message("Message has too few arguments!\n"); 
        else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)) 
            g_message("Argument is not error!\n"); 
        else{
           dbus_message_iter_get_basic(&args, &name);
            printf("name : %s\n", name); 
        }

        if (!dbus_message_iter_next(&args))
            g_message("Message has too few arguments!\n"); 
        else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)) 
            g_message("Argument is not error!\n"); 
        else{
           dbus_message_iter_get_basic(&args, &type);
            printf("type : %s\n", type); 
        }

        if (!dbus_message_iter_next(&args))
            g_message("Message has too few arguments!\n"); 
        else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)) 
            g_message("Argument is not error!\n"); 
        else{
            dbus_message_iter_get_basic(&args, &domain);
            printf("domain : %s\n", domain); 
        }

        if (!dbus_message_iter_next(&args))
            g_message("Message has too few arguments!\n"); 
        else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)) 
            g_message("Argument is not error!\n"); 
        else{
            dbus_message_iter_get_basic(&args, &host);
            printf("host : %s\n", host); 
        }

        if (!dbus_message_iter_next(&args))
            g_message( "Message has too few arguments!\n"); 
        else if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args)) 
            g_message("Argument is not error!\n"); 
        else{
            dbus_message_iter_get_basic(&args, &aprotocol);
            printf("aprotocol : %d\n", aprotocol); 
        }

        if (!dbus_message_iter_next(&args))
            g_message("Message has too few arguments!\n"); 
        else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)) 
            g_message("Argument is not error!\n"); 
        else{
           dbus_message_iter_get_basic(&args, &address);
            printf("address : %s\n", address); 
        }

        if (!dbus_message_iter_next(&args))
            g_message( "Message has too few arguments!\n"); 
        else if (DBUS_TYPE_UINT16 != dbus_message_iter_get_arg_type(&args)) 
            g_message("Argument is not error!\n"); 
        else{
            dbus_message_iter_get_basic(&args, &port);
            printf("port : %d\n", port); 
        }

        if (!dbus_message_iter_next(&args))
            g_message("Message has too few arguments!\n"); 
        else if (strcmp("aay", dbus_message_iter_get_signature(&args))) 
            g_message("Argument is not error!\n"); 
        else{
            DBusMessageIter container_ay, container_y;
            unsigned char *values;int size;
            //dbus_message_iter_get_basic(&args, &flags);
            //dbus_message_iter_open_container(&args, DBUS_TYPE_ARRAY, "ay", &container_ay);
            //dbus_message_iter_open_container(&container_ay, DBUS_TYPE_BYTE, "y", &container_y);

            dbus_message_iter_recurse(&args, &container_ay);

            dbus_message_iter_recurse(&container_ay, &container_y);
            dbus_message_iter_get_fixed_array(&container_y, &values, &size);
            g_message("size: %d, %s", size, values);
/*
g_message("container args: %s", dbus_message_iter_get_signature(&args));
g_message("container_ay: %s", dbus_message_iter_get_signature(&container_ay));
g_message("container_y: %s", dbus_message_iter_get_signature(&container_y));
g_message("type of container_y: %d", dbus_message_iter_get_arg_type(&container_y));
g_message("value of DBUS_TYPE_BYTE: %d", DBUS_TYPE_BYTE);
*/
// && strcmp("ay", dbus_message_iter_get_signature(&container_ay))
            while(dbus_message_iter_next(&container_ay)){
                dbus_message_iter_recurse(&container_ay, &container_y);
                dbus_message_iter_get_fixed_array(&container_y, &values, &size);
                g_message("size: %d, %s", size, values);
            }
        }
        if (!dbus_message_iter_next(&args))
            g_message( "Message has too few arguments!\n"); 
        else if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&args)) 
            g_message("Argument is not error!\n"); 
        else{
            dbus_message_iter_get_basic(&args, &m_flags);
            printf("m_flags : %d\n", m_flags); 
        }

/*
dbus_message_iter_next(&args);
g_message("g_message_iter_get_signature: %s", dbus_message_iter_get_signature(&args));
*/
/*
        if (!dbus_message_iter_next(&args))
            g_message("Message has too few arguments!\n"); 
        else if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&args)) 
            g_message("Argument is not error!\n"); 
        else
            dbus_message_iter_get_basic(&args, &flags);
*/
  //      printf("discovered:%d, %d, %s, %s, %s, %d.\n", interface, protocol, name, type, domain, flags);
    }
    dbus_message_unref(msg);  
}
void signal_ServiceBrowser_item(char *path){
    DBusMessage* msg;
    DBusMessageIter args;
    DBusConnection* conn;
    DBusError err;
    dbus_error_init(&err);

    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if (dbus_error_is_set(&err)) { 
      fprintf(stderr, "Connection Error (%s)\n", err.message); 
      dbus_error_free(&err);
    }
    if (NULL == conn) { 
      exit(1);
    }

    gchar* filter_itemnew = g_new(gchar, strlen("type='signal',interface='org.freedesktop.Avahi.ServiceBrowser',member='ItemNew', path=''") + strlen(path) + 2);
    sprintf(filter_itemnew, "type='signal',interface='org.freedesktop.Avahi.ServiceBrowser',member='ItemNew', path='%s'", path);
    gchar* filter_itemremove = g_new(gchar, strlen("type='signal',interface='org.freedesktop.Avahi.ServiceBrowser',member='ItemRemove', path=''") + strlen(path) + 2);
    sprintf(filter_itemremove, "type='signal',interface='org.freedesktop.Avahi.ServiceBrowser',member='ItemRemove', path='%s'", path);
    //printf("signal_filter:%s", signal_filter);
    // add a rule for which messages we want to see
    dbus_bus_add_match(conn, filter_itemnew, &err);
    dbus_bus_add_match(conn, filter_itemremove, &err);

    dbus_connection_flush(conn);
    if (dbus_error_is_set(&err)) { 
        fprintf(stderr, "Match Error (%s)\n", err.message);
        exit(1); 
   }
    printf("Match rule sent\n");

    int interface;
    int protocol;
    char *name;
    char *stype;
    char *domain;
    unsigned int flags;
//int cnt = 0;
   // loop listening for signals being emmitted
    while (true) {
        // non blocking read of the next available message
        dbus_connection_read_write(conn, 0);
        msg = dbus_connection_pop_message(conn);
        // loop again if we haven't read a message
        if (NULL == msg) { 
//g_message("cnt: %d", cnt++);
           sleep(1);
           continue;
    }
    // check if the message is a signal from the correct interface and with the correct name
    if (dbus_message_is_signal(msg, "org.freedesktop.Avahi.ServiceBrowser", "ItemNew")) {
        // read the parameters
//g_message("is the message we need.%d", cnt++);
//g_message("Message Has No Parameters\n");

        if (!dbus_message_iter_init(msg, &args))
            g_message("dbus_message_iter_init fail\n");
        else
        { 
            if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args)) 
               g_message( "Argument is not error!\n"); 
            else
               dbus_message_iter_get_basic(&args, &interface);

            if (!dbus_message_iter_next(&args))
                g_message( "Message has too few arguments!\n"); 
            else if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args)) 
                g_message("Argument is not error!\n"); 
            else
                dbus_message_iter_get_basic(&args, &protocol);

            if (!dbus_message_iter_next(&args))
                g_message("Message has too few arguments!\n"); 
            else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)) 
                g_message("Argument is not error!\n"); 
            else
               dbus_message_iter_get_basic(&args, &name);

            if (!dbus_message_iter_next(&args))
                g_message("Message has too few arguments!\n"); 
            else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)) 
                g_message("Argument is not error!\n"); 
            else
               dbus_message_iter_get_basic(&args, &stype);

            if (!dbus_message_iter_next(&args))
                g_message("Message has too few arguments!\n"); 
            else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)) 
                g_message("Argument is not error!\n"); 
            else
               dbus_message_iter_get_basic(&args, &domain);

            if (!dbus_message_iter_next(&args))
                g_message("Message has too few arguments!\n"); 
            else if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&args)) 
                g_message("Argument is not error!\n"); 
            else
                dbus_message_iter_get_basic(&args, &flags);
            printf("\n");
            printf("discovered:%d, %d, %s, %s, %s, %d.\n", interface, protocol, name, stype, domain, flags);
            resolve_service(interface, protocol, name, stype, domain);
        //printf("Got Signal with value %s\n", sigvalue);
        }
    }else if(dbus_message_is_signal(msg, "org.freedesktop.Avahi.ServiceBrowser", "ItemRemove")){
        if (!dbus_message_iter_init(msg, &args))
            g_message("dbus_message_iter_init fail\n");
        else
        { 
            if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args)) 
               g_message( "Argument is not error!\n"); 
            else
               dbus_message_iter_get_basic(&args, &interface);

            if (!dbus_message_iter_next(&args))
                g_message( "Message has too few arguments!\n"); 
            else if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args)) 
                g_message("Argument is not error!\n"); 
            else
                dbus_message_iter_get_basic(&args, &protocol);

            if (!dbus_message_iter_next(&args))
                g_message("Message has too few arguments!\n"); 
            else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)) 
                g_message("Argument is not error!\n"); 
            else
               dbus_message_iter_get_basic(&args, &name);

            if (!dbus_message_iter_next(&args))
                g_message("Message has too few arguments!\n"); 
            else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)) 
                g_message("Argument is not error!\n"); 
            else
               dbus_message_iter_get_basic(&args, &stype);

            if (!dbus_message_iter_next(&args))
                g_message("Message has too few arguments!\n"); 
            else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args)) 
                g_message("Argument is not error!\n"); 
            else
               dbus_message_iter_get_basic(&args, &domain);

            if (!dbus_message_iter_next(&args))
                g_message("Message has too few arguments!\n"); 
            else if (DBUS_TYPE_UINT32 != dbus_message_iter_get_arg_type(&args)) 
                g_message("Argument is not error!\n"); 
            else
                dbus_message_iter_get_basic(&args, &flags);
            printf("\n");
            printf("removed:%d, %d, %s, %s, %s, %d.\n", interface, protocol, name, stype, domain, flags);
            //resolve_service(interface, protocol, name, stype, domain);
        //printf("Got Signal with value %s\n", sigvalue);
        }
            }
    // free the message
    dbus_message_unref(msg);
    }
}
char* get_entry_group_path()
{
    DBusMessage* msg;
    DBusMessageIter args;
    DBusConnection* conn;
    DBusError err;
    dbus_error_init(&err);
    DBusPendingCall* pending;
    char *entry_group_path;

   // initialiset the errors
    printf("Calling remote method: %s\n", "org.freedesktop.Avahi");

   // connect to the system bus and check for errors
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if (dbus_error_is_set(&err)) { 
      fprintf(stderr, "Connection Error (%s)\n", err.message); 
      dbus_error_free(&err);
    }
    if (NULL == conn) { 
      exit(1);
    }

    msg = dbus_message_new_method_call("org.freedesktop.Avahi",// target for the method call
                                        "/", 							// object to call on
                                        "org.freedesktop.Avahi.Server",    // interface to call on
                                        "EntryGroupNew");             // method nameResolveHostName
    if (NULL == msg) 
     { 
      fprintf(stderr, "Message Null\n"); 
      exit(1); 
    }
   // send message and get a handle for a reply
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
    if (dbus_message_get_type(msg) == DBUS_MESSAGE_TYPE_ERROR) {
			   fprintf("message returned error: %s", dbus_message_get_error_name(msg));
	}
    // free the pending message handle
    dbus_pending_call_unref(pending);

    // read the parameters
    if (!dbus_message_iter_init(msg, &args))
        fprintf(stderr, "Message has no arguments!\n"); 
    else if (DBUS_TYPE_OBJECT_PATH != dbus_message_iter_get_arg_type(&args)) 
        fprintf(stderr, "Argument is not DBUS_TYPE_OBJECT_PATH!\n"); 
    else
        dbus_message_iter_get_basic(&args, &entry_group_path);


    printf("Got Reply: %s\n", entry_group_path);
    dbus_message_unref(msg);  
    return entry_group_path;
}

void method_AddService(char *path)
{
    DBusMessage* msg;
    DBusMessageIter args;
    DBusConnection* conn;
    DBusError err;
    dbus_error_init(&err);
    DBusPendingCall* pending;
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
 /*
    <arg name="interface" type="i" direction="in"/>
    <arg name="protocol" type="i" direction="in"/>
    <arg name="flags" type="u" direction="in"/>
    <arg name="name" type="s" direction="in"/>
    <arg name="type" type="s" direction="in"/>
    <arg name="domain" type="s" direction="in"/>
    <arg name="host" type="s" direction="in"/>
    <arg name="port" type="q" direction="in"/>
    <arg name="txt" type="aay" direction="in"/>
 */
   // initialiset the errors
    printf("Calling remote method: %s\n", "org.freedesktop.Avahi.EntryGroup");

   // connect to the system bus and check for errors
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if (dbus_error_is_set(&err)) { 
      fprintf(stderr, "Connection Error (%s)\n", err.message); 
      dbus_error_free(&err);
    }
    if (NULL == conn) { 
      exit(1);
    }

    msg = dbus_message_new_method_call("org.freedesktop.Avahi",// target for the method call
                                        path, 							// object to call on
                                        "org.freedesktop.Avahi.EntryGroup",    // interface to call on
                                        "AddService");             // method nameResolveHostName
    if (NULL == msg) 
     { 
      fprintf(stderr, "Message Null\n"); 
      exit(1); 
    }
   // append arguments
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &interface))
    {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &protocol))
    {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &flags))
    {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &name))
    {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &type))
    {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &domain))
    {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &host))
    {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT16, &port))
    {
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
    }
    /*
	DBusMessageIter iter_ay, iter_y;
	// Open dict entry container
	if (!dbus_message_iter_open_container(&args, DBUS_TYPE_ARRAY, "ay", &iter_ay)) {
			printf("Can't open container for iter_ay\n");
			exit(1);
	}
	if (!dbus_message_iter_open_container(&iter_ay, DBUS_TYPE_ARRAY, "y", &iter_y)) {
			printf("Can't open container for iter_y\n");
			exit(1);
	}
	dbus_uint16_t  bb=98;
	dbus_uint16_t  cc=97;
	dbus_message_iter_append_basic(&iter_y, DBUS_TYPE_BYTE, &bb);
	dbus_message_iter_append_basic(&iter_y, DBUS_TYPE_BYTE, &cc);
	dbus_message_iter_close_container(&iter_ay, &iter_y);
	dbus_message_iter_close_container(&args, &iter_ay);
	g_message("signature of iter_ay: %s", dbus_message_iter_get_signature(&iter_ay));
	g_message("signature of args: %s", dbus_message_iter_get_signature(&args));
*/

	 DBusMessageIter iter_ay, iter_y;
  if (!dbus_message_iter_open_container(&args, DBUS_TYPE_ARRAY, "ay", &iter_ay))
      {
		printf("Can't open container for iter_ay\n");
      exit(1);
      }
	if (!dbus_message_iter_open_container(&iter_ay, DBUS_TYPE_ARRAY, "y", &iter_y)) {
			printf("Can't open container for iter_y\n");
			exit(1);
	}
	if (!dbus_message_iter_append_fixed_array (&iter_y, DBUS_TYPE_BYTE, &txt1, sizeof(txt1)))
	 {
	     fprintf (stderr, "No memory!\n");
	}
	dbus_message_iter_close_container(&iter_ay, &iter_y);
	dbus_message_iter_close_container(&args, &iter_ay);
	g_message("arrive here1.");
	g_message("signature of msg: %s", dbus_message_get_signature(msg));
//	g_message("signature of iter_ay: %s", dbus_message_iter_get_signature(&iter_ay));
//	g_message("signature of args: %s", dbus_message_iter_get_signature(&args));
	
  // send message and get a handle for a reply
  if (!dbus_connection_send (conn, msg, NULL)) { // -1 is default timeout
     fprintf(stderr, "Out Of Memory!\n"); 
     exit(1);
     }

    dbus_connection_flush(conn);
   
    printf("Request Sent\n");
   
    // free message
    dbus_message_unref(msg);
}
void method_GetState(char *path){
    DBusMessage* msg;
    DBusMessageIter args;
    DBusConnection* conn;
    DBusError err;
    dbus_error_init(&err);
    DBusPendingCall* pending;
    // connect to the system bus and check for errors
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if (dbus_error_is_set(&err)) { 
      fprintf(stderr, "Connection Error (%s)\n", err.message); 
      dbus_error_free(&err);
     }
    if (NULL == conn) { 
       exit(1);
     }
    //char *path = malloc(sizeof(char) * strlen(mpath));
    //strcpy(path, mpath);
    //printf("*0mpath: %s / %s\n", path, mpath);
    msg = dbus_message_new_method_call("org.freedesktop.Avahi",// target for the method call
                                        path, 							// object to call on
                                        "org.freedesktop.Avahi.EntryGroup",    // interface to call on
                                        "GetState");             // method nameResolveHostName
    //printf("*1mpath: %s / %s\n", path, mpath);
    if (!dbus_connection_send_with_reply (conn, msg, &pending, -1)) { // -1 is default timeout
        fprintf(stderr, "Out Of Memory!\n"); 
        exit(1);
     }
    //printf("*2mpath: %s / %s\n", path, mpath);
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

     dbus_uint32_t m_state;
     if (!dbus_message_iter_init(msg, &args)){
         g_message("dbus_message_iter_init fail\n");
     }
     if(dbus_message_get_type(msg) == DBUS_MESSAGE_TYPE_ERROR){
         char *result;
         g_message("arg type: %d", dbus_message_iter_get_arg_type(&args));
         dbus_message_iter_get_basic(&args, &result);
         g_message("error:  %s", result);    	 
     }else{
         if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args)) 
             g_message("Argument is not DBUS_TYPE_INT32!\n"); 
         else{
             dbus_message_iter_get_basic(&args, &m_state);
             printf("m_state : %d\n", m_state);
         }
     }
     dbus_message_unref(msg);
}
void method_Commit(char *path){
    DBusConnection* conn;
    DBusMessage* msg;
    DBusError err;
    dbus_error_init(&err);
    // connect to the system bus and check for errors
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if (dbus_error_is_set(&err)) { 
      fprintf(stderr, "Connection Error (%s)\n", err.message); 
      dbus_error_free(&err);
     }
     if (NULL == conn) { 
       exit(1);
     }
    msg = dbus_message_new_method_call("org.freedesktop.Avahi",// target for the method call
                                        path, 							// object to call on
                                        "org.freedesktop.Avahi.EntryGroup",    // interface to call on
                                        "Commit");             // method nameResolveHostName
    if (!dbus_connection_send (conn, msg, NULL)) { //
       fprintf(stderr, "Out Of Memory!\n"); 
       exit(1);
       }
    dbus_connection_flush(conn);
    dbus_message_unref(msg);	
}

void method_Reset(char *path){
    DBusMessage* msg;
    DBusConnection* conn;
    DBusError err;
    dbus_error_init(&err);
    // connect to the system bus and check for errors
     conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
     if (dbus_error_is_set(&err)) { 
       fprintf(stderr, "Connection Error (%s)\n", err.message); 
       dbus_error_free(&err);
     }
     if (NULL == conn) { 
       exit(1);
     }
    msg = dbus_message_new_method_call("org.freedesktop.Avahi",// target for the method call
                                        path, 							// object to call on
                                        "org.freedesktop.Avahi.EntryGroup",    // interface to call on
                                        "Reset");             // method nameResolveHostName
    if (!dbus_connection_send (conn, msg, NULL)) { // -1 is default timeout
       fprintf(stderr, "Out Of Memory!\n"); 
       exit(1);
       }
    dbus_connection_flush(conn);
    dbus_message_unref(msg);	
}
void main()
{
  	 //signal_ServiceBrowser_item(get_service_browser_path());
    //resolve_service();
    char *entry_group_path = get_entry_group_path();
    char *mypath = malloc(sizeof(char) * strlen(entry_group_path));
    strcpy(mypath, entry_group_path);
/*
    printf("*mypath: %s / %s\n", mypath, entry_group_path);
    method_AddService(entry_group_path);
    printf("*mypath: %s / %s\n", mypath, entry_group_path);
    method_Commit(entry_group_path);
    printf("*mypath: %s / %s\n", mypath, entry_group_path);
*/
    method_GetState(mypath);
    printf("*mypath: %s / %s\n", mypath, entry_group_path);
	//method_Commit(entry_group_path);

	//printf("*entry_group_path: %s\n", entry_group_path);
}
