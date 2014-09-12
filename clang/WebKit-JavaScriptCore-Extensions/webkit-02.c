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

static void custom_feedback_cb()
{

    g_print("Custom feedback.\n");
}
/* Class method declarations */
static const JSStaticFunction custom_staticfuncs[] =
{
    { "feedback", custom_feedback_cb, kJSPropertyAttributeReadOnly },
    { NULL, NULL, 0 }
};

/* Class constructor. Called at "new CustomClass()" */
JSObjectRef class_constructor_cb(JSContextRef ctx,
                                 JSObjectRef constructor,
                                 size_t argumentCount,
                                 const JSValueRef arguments[],
                                 JSValueRef* exception)
{
    g_print("Custom class constructor\n");
}

static const JSClassDefinition class_def =
{
    0,                     // version
    kJSClassAttributeNone, // attributes
    "CustomClass",         // className
    NULL,                  // parentClass
    NULL,                  // staticValues
    custom_staticfuncs,                  // staticFunctions
    class_init_cb,         // initialize
    class_finalize_cb,     // finalize
    NULL,                  // hasProperty
    NULL,                  // getProperty
    NULL,                  // setProperty
    NULL,                  // deleteProperty
    NULL,                  // getPropertyNames
    NULL,                  // callAsFunction
    NULL,  // callAsConstructor class_constructor_cb
    NULL,                  // hasInstance  
    NULL                   // convertToType
};

/* Callback - JavaScript window object has been cleared */
static void window_object_cleared_cb(WebKitWebView  *web_view,
                                     WebKitWebFrame *frame,
                                     gpointer        context,
                                     gpointer        window_object,
                                     gpointer        user_data)

{
    /* Add classes to JavaScriptCore */
    JSClassRef classDef = JSClassCreate(&class_def);
    JSObjectRef classObj = JSObjectMake(context, classDef, context);
    JSObjectRef globalObj = JSContextGetGlobalObject(context);
    JSStringRef str = JSStringCreateWithUTF8CString("CustomClass");
    JSObjectSetProperty(context, globalObj, str, classObj, kJSPropertyAttributeNone, NULL);
}

/* Destroy callback */
static void destroy(GtkWidget *widget,
                    gpointer   data )
{
    gtk_main_quit();
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


    /* Place the WebKitWebView in the GtkScrolledWindow */
    gtk_container_add (GTK_CONTAINER (scrolled_window), web_view);
    gtk_container_add (GTK_CONTAINER (main_window), scrolled_window);

    /* Connect the destroy window event with destroy function */
    g_signal_connect (G_OBJECT (main_window), "destroy", G_CALLBACK (destroy), NULL);
    
    /* Open webpage */
    webkit_web_view_load_uri (WEBKIT_WEB_VIEW (web_view), "file:///home/cos/Public/ccode/c/WebKit-JavaScriptCore-Extensions/webkit-02.html");

    /* Create the main window */
    gtk_window_set_default_size (GTK_WINDOW (main_window), 800, 600);
    
    /* Show the application window */
    gtk_widget_show_all (main_window);

    /* Enter the main event loop, and wait for user interaction */
    gtk_main ();
    
    /* The user lost interest */
    return 0;
}
