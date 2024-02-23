#include <X11/Xlib.h>
#include <iostream>

int main() {
    Display* display = XOpenDisplay(NULL);

    Window root = DefaultRootWindow(display);

    XGrabButton(display, AnyButton, AnyModifier, root, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None);

    XEvent ev;
    while (true) {
        XNextEvent(display, &ev);
        if (ev.type == ButtonPress) {
            Window clickedWindow = ev.xbutton.subwindow;
            if (clickedWindow == None) {
                clickedWindow = ev.xbutton.window;
            }
            std::cout << clickedWindow << std::endl;
            break;
        }
    }

    XCloseDisplay(display);
    return 0;
}
