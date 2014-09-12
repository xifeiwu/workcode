#include <stdlib.h>
#include <gtk/gtk.h>
#include <webkit/webkit.h>
#include <JavaScriptCore/JavaScript.h>

/* Class initialize */
static void class_init_cb(JSContextRef ctx,
                          JSObjectRef object)
{
//    g_message("Custom class initialize.");
    g_print("Custom class initialize.\n");
}

/* Class finalize */
static void class_finalize_cb(JSObjectRef object)
{
//    g_message("Custom class finalize.");
    g_print("Custom class finalize.\n");
}

/* Destroy callback */
static void destroy(GtkWidget *widget,
                    gpointer   data )
{
    gtk_main_quit();
}
/************************************************/

void away_Initialize(JSContextRef ctx, JSObjectRef object)
{
}

void away_Finalize(JSObjectRef object)
{
}

JSValueRef away_GetVerbose(
        JSContextRef ctx,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef *exception)
{
    // verbose is false
    return JSValueMakeBoolean(ctx, false);
}

JSValueRef away_GetVersion(
        JSContextRef ctx,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef *exception)
{
    // verbose is false
    static JSStringRef version = NULL;
    if (version == NULL)
    {
        version = JSStringCreateWithUTF8CString("0.1");
    }
    return JSValueMakeString(ctx, version);
}

JSValueRef away_Show(JSContextRef ctx,
    JSObjectRef function,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef *exception)
{
    JSStringRef str = JSValueToStringCopy(ctx, arguments[0], exception);
    size_t size = JSStringGetMaximumUTF8CStringSize(str);
    char* utf8 = (char*)malloc(size * sizeof(char));
//NEW(char, size);
    
    JSStringGetUTF8CString(str, utf8, size);
    
    GtkWidget *dia = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, utf8);
    
    gtk_dialog_run(dia);
    
    gtk_widget_destroy(dia);
    
    return JSValueMakeNumber(ctx, size);
}

JSValueRef away_Print(JSContextRef ctx,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef *exception)
{
    JSStringRef str = JSValueToStringCopy(ctx, arguments[0], exception);
    size_t size = JSStringGetMaximumUTF8CStringSize(str);
    char* utf8 = (char*)malloc(size * sizeof(char));
//NEW(char, size);

    JSStringGetUTF8CString(str, utf8, size);
    
    fprintf(stderr, "%s\n", utf8);
    
    return JSValueMakeNumber(ctx, size);
}
JSClassRef Away_ClassCreate(JSContextRef ctx)
{
    static JSClassRef awayClass = NULL;
    if (awayClass)
    {
        // already created, just return
        return awayClass;
    }

    JSStaticFunction awayStaticFunctions[] =
    {
        { "show", away_Show, kJSPropertyAttributeNone },
        { "print", away_Print, kJSPropertyAttributeNone },
        { NULL, 0, 0 },
    };

    JSStaticValue awayStaticValues[] =
    {
        { "Version", away_GetVersion, NULL, kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { "Verbose", away_GetVerbose, NULL, kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { NULL, 0, 0, 0},
    };

    JSClassDefinition classdef = kJSClassDefinitionEmpty;
    classdef.className = "Away";
    classdef.initialize = away_Initialize;
    classdef.finalize = away_Finalize;
    classdef.staticValues = awayStaticValues;
    classdef.staticFunctions = awayStaticFunctions;

    return awayClass = JSClassCreate(&classdef);
}


void window_object_cleared_cb(
WebKitWebView *wv,
WebKitWebFrame *wf,
gpointer ctx,
gpointer arg3,
gpointer user_data)
{
    JSStringRef name = JSStringCreateWithUTF8CString("Away");
    // Make the javascript object
    JSObjectRef obj = JSObjectMake(ctx, Away_ClassCreate(ctx), NULL);
    // Set the property
    JSObjectSetProperty(ctx, JSContextGetGlobalObject(ctx), name, obj,kJSPropertyAttributeNone, NULL);
    
    JSObjectRef obj2 = JSObjectMake(ctx, Away_ClassCreate(ctx), NULL);
    // Set the property
    JSObjectSetProperty(ctx, obj, name, obj2,kJSPropertyAttributeNone, NULL);
}

int
main (int argc, char* argv[])
{
    /* Initialize the widget set */
    gtk_init (&argc, &argv);
    
    /* Create the window widgets */
    GtkWidget *main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    GtkWidget *scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    
    /* Create the WebKit Web View widget */
    GtkWidget *web_view = webkit_web_view_new ();
    
    /* Connect the window object cleared event with callback */
    g_signal_connect (G_OBJECT (web_view), "window-object-cleared", G_CALLBACK(window_object_cleared_cb), web_view);
    webkit_web_view_set_custom_encoding(WEBKIT_WEB_VIEW(web_view), "UTF-8"); /*设置编码为UTF-8 */

    /* Place the WebKitWebView in the GtkScrolledWindow */
    gtk_container_add (GTK_CONTAINER (scrolled_window), web_view);
    gtk_container_add (GTK_CONTAINER (main_window), scrolled_window);

    /* Connect the destroy window event with destroy function */
    g_signal_connect (G_OBJECT (main_window), "destroy", G_CALLBACK (destroy), NULL);
    
    /* Open webpage */
    webkit_web_view_load_uri (WEBKIT_WEB_VIEW (web_view), "file:///home/cos/Public/ccode/c/WebKit-JavaScriptCore-Extensions/webkit-00.html");

    /* Create the main window */
    gtk_window_set_default_size (GTK_WINDOW (main_window), 800, 600);
    
    /* Show the application window */
    gtk_widget_show_all (main_window);

    /* Enter the main event loop, and wait for user interaction */
    gtk_main ();
    
    /* The user lost interest */
    return 0;
}
