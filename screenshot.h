#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#pragma once

#include <opencv2/opencv.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string>
#include <memory>
#include <mutex>

using namespace cv;
using namespace std;

class Screenshot {
private:
    Display* display;
    Window root;
    string display_name;
    static mutex x11_mutex;  // Mutex for X11 operations
    
public:
    Screenshot(const string& displayName = "") : display_name(displayName) {
        lock_guard<mutex> lock(x11_mutex);
        display = XOpenDisplay(display_name.empty() ? nullptr : display_name.c_str());
        if (!display) throw runtime_error("Cannot open X display " + display_name);
        root = DefaultRootWindow(display);
    }
    
    ~Screenshot() {
        lock_guard<mutex> lock(x11_mutex);
        if (display) XCloseDisplay(display);
    }
    
    Mat get(int x, int y, int width, int height) {
        lock_guard<mutex> lock(x11_mutex);
        XImage* img = XGetImage(display, root, x, y, width, height, AllPlanes, ZPixmap);
        if (!img) throw runtime_error("Failed to capture screen from display " + display_name);
        Mat tmp = Mat(height, width, CV_8UC4, img->data);
        cvtColor(tmp, tmp, COLOR_BGRA2BGR);
        XDestroyImage(img);
        return tmp.clone();
    }
    
    // Disable copying
    Screenshot(const Screenshot&) = delete;
    Screenshot& operator=(const Screenshot&) = delete;
};

int init = XInitThreads();
mutex Screenshot::x11_mutex;

#endif