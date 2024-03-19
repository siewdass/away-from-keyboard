#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <string>

using namespace cv;
using namespace std;

class Screenshot {
public:
	Screenshot( );
	~Screenshot( );
	void load( string );
	void unload( );
	Mat get( int, int, int, int );
};

#endif
