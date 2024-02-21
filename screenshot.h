#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

class Screenshot {
public:
    Mat tmp;
    Mat get( int tibia, int x, int y, int width, int height, bool minimap );
};

#endif // SCREENSHOT_H