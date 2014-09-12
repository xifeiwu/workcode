#include <gtk/gtk.h>

gboolean on_button_press (GtkWidget* widget,
  GdkEventButton * event, GdkWindowEdge edge)
{
  if (event->type == GDK_BUTTON_PRESS)
  {
    if (event->button == 1) {
      gtk_window_begin_move_drag(GTK_WINDOW(gtk_widget_get_toplevel(widget)),
          event->button,
	  event->x_root,
	  event->y_root,
	  event->time);
    }
  }

  return FALSE;
}

void button_clicked(GtkWidget *widget, gpointer data)
{
  g_print("clicked\n");
}

void enter_button(GtkWidget *widget, gpointer data) 
{ 
  g_print("in function enter_button\n");
  GdkRGBA color;
  color.red = 1.0;
  color.green = 0.0;
  color.blue = 0.0;
  color.alpha = 0.5;
  gtk_widget_override_background_color(widget, GTK_STATE_PRELIGHT, &color);
//  GdkColor color;
//  color.red = 27000;
//  color.green = 30325;
//  color.blue = 34181;
//  gtk_widget_modify_bg(widget, GTK_STATE_PRELIGHT, &color);
}

void frame_callback(GtkWindow *window, 
    GdkEvent *event, gpointer data)
{
  int x, y;
  char buf[10];
  x = event->configure.x;
  y = event->configure.y;
  sprintf(buf, "%d, %d", x, y);
  gtk_window_set_title(window, buf);
}

int main( int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *fixed;
  GtkWidget *button;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "GtkButton");
  gtk_window_set_default_size(GTK_WINDOW(window), 230, 150);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_widget_add_events(GTK_WIDGET(window), GDK_CONFIGURE);
  gtk_window_set_decorated(GTK_WINDOW (window), FALSE);
  fixed = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window), fixed);

  button = gtk_button_new_with_label("Click");
  gtk_fixed_put(GTK_FIXED(fixed), button, 50, 50);
  gtk_widget_set_size_request(button, 80, 35);
  g_signal_connect(G_OBJECT(button), "enter", 
    G_CALLBACK(enter_button), NULL);
  g_signal_connect(G_OBJECT(window), "button-press-event",
      G_CALLBACK(on_button_press), NULL);
  g_signal_connect(G_OBJECT(button), "clicked", 
      G_CALLBACK(button_clicked), NULL);
  g_signal_connect(G_OBJECT(window), "configure-event",
     G_CALLBACK(frame_callback), NULL);
  g_signal_connect(G_OBJECT(window), "destroy", 
      G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
