'''
Simple editor that load/upload patterns into POV circuitery using
a serial connection.
The format used for saving is

struct header {
    char magic[4], // "%POV"
    uint_8 version,// 1
    uint_16 width, // width of the pattern
    uint_16 height,// height of the pattern
    uint_8  depth  // # of channel 1 = BW, 3 = RGB
}

the byte order is the network one (big endian).
'''

import pygtk
pygtk.require('2.0')
import gtk
import math
import struct


class Board(gtk.DrawingArea):
    """This represents the grid used to draw the pattern.

    http://www.pygtk.org/pygtk2tutorial/sec-EventHandling.html#idp4150200
    """
    # Draw in response to an expose-event
    __gsignals__ = {
        "expose-event": "override",
        #"click": "override",
    }
    BOARD_SQUARE_SIZE = 50

    def __init__(self, width, height):
        gtk.DrawingArea.__init__(self)
        #self.width = width
        #self.height = height

        self.connect('button_press_event', self.on_clicked)
        self.connect("configure_event", self.on_configure_event)
        self.set_events(gtk.gdk.EXPOSURE_MASK
                               | gtk.gdk.LEAVE_NOTIFY_MASK
                               | gtk.gdk.BUTTON_PRESS_MASK
                               | gtk.gdk.POINTER_MOTION_MASK
                               | gtk.gdk.POINTER_MOTION_HINT_MASK)

    def do_expose_event(self, event):
        ctx = self.window.cairo_create()
        # Restrict Cairo to the exposed area; avoid extra work
        ctx.rectangle(event.area.x, event.area.y,
                event.area.width, event.area.height)
        ctx.clip()

        self.draw(ctx, *self.window.get_size())

    def on_configure_event(self, widget, event):
        x, y, self.width, self.height = widget.get_allocation()
        self.n_hor = self.width/self.BOARD_SQUARE_SIZE
        self.n_ver = self.height/self.BOARD_SQUARE_SIZE

        # create an empty array with values
        # FIXME: save in case of reconfigure during editing
        self.values = [0]*self.n_hor*self.n_ver

    def draw(self, ctx, width, height):
        # Fill the background with white
        ctx.set_source_rgb(1.0, 1.0, 1.0)
        ctx.rectangle(0, 0, width, height)
        ctx.fill()

        # draw the grid
        for x in range(self.n_hor):
            for y in range(self.n_ver):
                # TODO: change X, Y in names more human
                X, Y = x*self.BOARD_SQUARE_SIZE, y*self.BOARD_SQUARE_SIZE
                WIDTH, HEIGHT = self.BOARD_SQUARE_SIZE, self.BOARD_SQUARE_SIZE

                # background
                color = (1.0, 1.0, 1.0)
                if self.get_value_on(x, y):
                    color = (1.0, 0.0, 0.0)

                ctx.set_source_rgb(*color)
                ctx.rectangle(X, Y, WIDTH, HEIGHT)
                ctx.fill()

                # border
                ctx.set_source_rgb(0.0, 0.0, 0.0)
                ctx.move_to(X, Y)
                ctx.rel_line_to(WIDTH, 0)
                ctx.rel_line_to(0, HEIGHT)
                ctx.rel_line_to(-WIDTH, 0)
                ctx.rel_line_to(0, -HEIGHT)
                ctx.stroke()

    def on_clicked(self, widget, event):
        x, y = int(math.floor(event.x/self.BOARD_SQUARE_SIZE)), int(math.floor(event.y/self.BOARD_SQUARE_SIZE))
        self.set_value_on(x, y)
        self.queue_draw()

    def set_value_on(self, x, y):
        """Switch value in the selected cell"""
        value = self.get_value_on(x, y)
        self.values[(y * self.n_hor) + x] = 1 if not value else 0

    def get_value_on(self, x, y):
        return self.values[(y * self.n_hor) + x]

class POVEditor(gtk.Window):
    def __init__(self):
        gtk.Window.__init__(self)

        self.board = Board(0, 0)
        self.board.set_size_request(500, 400)
        self.button = gtk.Button()
        self.button.set_size_request(50,50)
        self.button.connect('clicked', self.on_button_clicked)
        vbox = gtk.VBox()

        self.add(vbox)
        vbox.add(self.board)
        vbox.add(self.button)

        self.connect("delete-event", gtk.main_quit)

    def on_button_clicked(self, event):
        # http://docs.python.org/2.7/library/struct.html?highlight=struct#byte-order-size-and-alignment
        with open('flash', 'w') as f:
            header = struct.pack('!4cBhhB', '%', 'P', 'O', 'V', 1, 500, 400, 1)
            f.write(header)
            for p in self.board.values:
                f.write(struct.pack('!B', p))

if __name__ == "__main__":
    editor = POVEditor()
    editor.show_all()

    gtk.main()


