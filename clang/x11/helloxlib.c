#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
 
int main(void)
{
  Display *d;
  Window w;
  XEvent e;
  char *msg = "Hello, World!";
  int s;
 
  bool done = false;
 
  /* open connection with the server */
  d = XOpenDisplay(NULL);
  if (d == NULL) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }
 
  s = DefaultScreen(d);
 
  /* create window */
  w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 640, 480, 0,
			  BlackPixel(d, s), WhitePixel(d, s));
 
  /* register interest in the delete window message */
  Atom wmDeleteMessage = XInternAtom(d, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(d, w, &wmDeleteMessage, 1);
 
  /* select kind of events we are interested in */
  XSelectInput(d, w, ExposureMask | KeyPressMask | StructureNotifyMask);
 
  /* map (show) the window */
  XMapWindow(d, w);
 
  /* event loop */
  while (!done) {
    XNextEvent(d, &e);
    /* draw or redraw the window */
    if (e.type == Expose) {
      XFillRectangle(d, w, DefaultGC(d, s), 20, 20, 10, 10);
      XDrawString(d, w, DefaultGC(d, s), 50, 50, msg, strlen(msg));
    }
 
    /* exit on key press */
    switch(e.type){
 
    case KeyPress:
      XDestroyWindow(d, w);
      break;
 
    case DestroyNotify:
      done = true;
      break;
 
    case ClientMessage:
      if (e.xclient.data.l[0] == wmDeleteMessage){
	done = true;
      }
      break;      
    }
  }
 
  /* close connection to server */
  XCloseDisplay(d);
 
  return 0;
}
