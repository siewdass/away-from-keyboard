#include "screenshot.h"
#include <X11/Xutil.h>

int init = XInitThreads();
mutex Screenshot::x11_mutex;

Screenshot::Screenshot(const string& displayName) : display_name(displayName) {
    lock_guard<mutex> lock(x11_mutex);
    display = XOpenDisplay(display_name.empty() ? nullptr : display_name.c_str());
    if (!display) throw runtime_error("Cannot open X display " + display_name);
    root = DefaultRootWindow(display);
}

Screenshot::~Screenshot() {
    lock_guard<mutex> lock(x11_mutex);
    if (display) XCloseDisplay(display);
}

Mat Screenshot::get(int x, int y, int width, int height) {
    lock_guard<mutex> lock(x11_mutex);
    XImage* img = XGetImage(display, root, x, y, width, height, AllPlanes, ZPixmap);
    if (!img) throw runtime_error("Failed to capture screen from display " + display_name);
    Mat tmp = Mat(height, width, CV_8UC4, img->data);
    cvtColor(tmp, tmp, COLOR_BGRA2BGR);
    XDestroyImage(img);
    return tmp.clone();
}