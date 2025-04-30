#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#pragma once

#include <opencv2/opencv.hpp>
#include <X11/Xlib.h>
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
    Screenshot(const string& displayName = "");
    ~Screenshot();
    Mat get(int x, int y, int width, int height);
    
    // Disable copying
    Screenshot(const Screenshot&) = delete;
    Screenshot& operator=(const Screenshot&) = delete;
};

#endif