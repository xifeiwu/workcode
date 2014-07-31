import os
import threading
import gtk

class FileBrowser:      
    def __init__(self):
        self.window = gtk.Window()
        self.window.show_all()
        self.window.connect("destroy", gtk.main_quit)

        box = gtk.VBox()
        box.show()

        self.scrolled_w = gtk.ScrolledWindow()
        self.scrolled_w.show()
        self.window.add(box)

        button = gtk.Button("start")
        button.show()
        button.connect("clicked", self.start_scanning)
        button.set_size_request(30, 50)
        box.pack_start(button, False, False, 4)

        self.model=gtk.TreeStore(str)
        self.treeview = gtk.TreeView(self.model)
        self.treeview.show()

        col = gtk.TreeViewColumn("FileName")
        cell = gtk.CellRendererText()

        self.treeview.append_column(col)
        col.pack_start(cell, 0)
        col.set_attributes(cell, text=0)        

        box.pack_start(self.scrolled_w, 10)
        self.scrolled_w.add(self.treeview)
        self.window.set_size_request(600, 300)

    def start_scanning(self, w):
        self.model.clear()
        main_dir = "/"
        list_dir = os.listdir(main_dir)
        no_of_threads = len(list_dir)
        threads = []

        for i in range(no_of_threads):
            t = threading.Thread(target=self.thread_scanning,
                                 args=(main_dir,list_dir[i],))
            threads.append(t)
            t.start()   
            for t in threads:
                t.join()

    def thread_scanning(self, main_d, list_d):
        path = main_d+""+list_d
        if os.path.isdir(path):
            list_subd = os.listdir(path)
            par = self.model.append(None, [list_d])
            for sub in list_subd:
                self.model.append(par, [sub])

    def main(self):
        gtk.main()


if __name__=="__main__":
    fb = FileBrowser()
    fb.main()
