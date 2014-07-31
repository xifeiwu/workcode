import gtk

class DragImage(gtk.Image):
    def __init__(self,image,layout):
        gtk.Image.__init__(self)
        self.drag = False
        self.drag_x = 0
        self.drag_y = 0
        self.layout = layout
        self.x = 0
        self.y = 0
        self.set_from_file(image)
        self.event_box = gtk.EventBox()
        self.event_box.set_visible_window(False)
        self.event_box.add(self)
        self.event_box.add_events(gtk.gdk.POINTER_MOTION_MASK | gtk.gdk.BUTTON_PRESS_MASK | gtk.gdk.BUTTON_RELEASE_MASK)
        self.event_box.connect("button-press-event", self.click)
        self.event_box.connect("button-release-event", self.release)
        self.event_box.connect("motion-notify-event", self.mousemove)
        self.layout.put( self.event_box, 0, 0 )

    def click(self, widget, event):
        self.drag =  True
        self.drag_x =  event.x
        self.drag_y =  event.y
        print(self.drag_x, self.drag_y)

    def release(self, widget, event):
        self.drag =  False

    def mousemove(self,widget,event):
        if self.drag:
            self.layout.move(self.event_box,self.x+int(event.x-self.drag_x),self.y+int(event.y-self.drag_y))
            self.x, self.y = self.layout.child_get(self.event_box,'x','y')

class move_test(object):
    def __init__(self):
        window =  gtk.Window()
        layout =  gtk.Layout()
        img1 = DragImage('soccerball.gif',layout)
        img2 = DragImage('image2.jpg',layout)
        window.add(layout)
        window.show_all()

move_test()
gtk.main()
