#include <gtk/gtk.h>
#include <webkit/webkit.h>
int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    /* Create the widgets */
    GtkWidget *main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    GtkWidget *scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    GtkWidget *web_view = webkit_web_view_new ();
    
    /* Place the WebKitWebView in the GtkScrolledWindow */
    gtk_container_add (GTK_CONTAINER (scrolled_window), web_view);
    gtk_container_add (GTK_CONTAINER (main_window), scrolled_window);
    
    /* Open a webpage */
    webkit_web_view_load_uri (WEBKIT_WEB_VIEW (web_view), "http://www.gnome.org");
    
    /* Show the result */
    gtk_window_set_default_size (GTK_WINDOW (main_window), 800, 600);
    gtk_widget_show_all (main_window);
    gtk_main();
    return 0;
}
