#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>

static void do_drawing(cairo_t *);

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
    gpointer user_data)
{  
  do_drawing(cr);  

  return FALSE;
}
//line caps
/*
static void do_drawing(cairo_t *cr)
{
  cairo_set_line_width(cr, 10);

  cairo_set_line_cap(cr, CAIRO_LINE_CAP_BUTT); 
  cairo_move_to(cr, 30, 50); 
  cairo_line_to(cr, 150, 50);
  cairo_stroke(cr);

  cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND); 
  cairo_move_to(cr, 30, 90); 
  cairo_line_to(cr, 150, 90);
  cairo_stroke(cr);

  cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE); 
  cairo_move_to(cr, 30, 130); 
  cairo_line_to(cr, 150, 130);
  cairo_stroke(cr);

  cairo_set_line_width(cr, 1.5);

  cairo_move_to(cr, 30, 40);  
  cairo_line_to(cr, 30, 140);
  cairo_stroke(cr);

  cairo_move_to(cr, 150, 40);  
  cairo_line_to(cr, 150, 140);
  cairo_stroke(cr);

  cairo_move_to(cr, 155, 40);  
  cairo_line_to(cr, 155, 140);
  cairo_stroke(cr);    
}
*/

//line jions
/*
static void do_drawing(cairo_t *cr)
{
  cairo_set_source_rgb(cr, 0.1, 0, 0);

  cairo_rectangle(cr, 30, 30, 100, 100);
  cairo_set_line_width(cr, 14);
  cairo_set_line_join(cr, CAIRO_LINE_JOIN_MITER); 
  cairo_stroke(cr);

  cairo_rectangle(cr, 160, 30, 100, 100);
  cairo_set_line_width(cr, 14);
  cairo_set_line_join(cr, CAIRO_LINE_JOIN_BEVEL); 
  cairo_stroke(cr);

  cairo_rectangle(cr, 100, 160, 100, 100);
  cairo_set_line_width(cr, 14);
  cairo_set_line_join(cr, CAIRO_LINE_JOIN_ROUND); 
  cairo_stroke(cr);    
}
*/

//pen dash
static void do_drawing(cairo_t *cr)
{
  cairo_set_source_rgba(cr, 0, 0, 0, 1);

  static const double dashed1[] = {4.0, 21.0, 2.0};
  static int len1  = sizeof(dashed1) / sizeof(dashed1[0]);

  static const double dashed2[] = {14.0, 6.0};
  static int len2  = sizeof(dashed2) / sizeof(dashed2[0]);

  static const double dashed3[] = {1.0};

  cairo_set_line_width(cr, 1.5);

  cairo_set_dash(cr, dashed1, len1, 0);

  cairo_move_to(cr, 40, 30);  
  cairo_line_to(cr, 200, 30);
  cairo_stroke(cr);

  cairo_set_dash(cr, dashed2, len2, 1);

  cairo_move_to(cr, 40, 50);  
  cairo_line_to(cr, 200, 50);
  cairo_stroke(cr);

  cairo_set_dash(cr, dashed3, 1, 0);

  cairo_move_to(cr, 40, 70);  
  cairo_line_to(cr, 200, 70);
  cairo_stroke(cr);  
}

int main (int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *darea;
  
  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  darea = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), darea);

  g_signal_connect(G_OBJECT(darea), "draw", 
      G_CALLBACK(on_draw_event), NULL);
  g_signal_connect(G_OBJECT(window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200); 
  gtk_window_set_title(GTK_WINDOW(window), "Fill & stroke");

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
