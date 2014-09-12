#include <gtk/gtk.h>
void print_cb()
{
    printf("in callback function");
}
int main( int argc, char *argv[])
{
GtkWidget *window;

gtk_init(&argc, &argv);

window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_widget_show(window);

gtk_main();

return 0;
}
