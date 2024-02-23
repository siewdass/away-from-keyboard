#ifndef VIEWPORT_H
#define VIEWPORT_H

#pragma once

#include "types.h"
#include <opencv2/opencv.hpp> 

using namespace std;
using namespace cv;

class Viewport {
public:
	int id;
	int sqm = 41; 
	Point xy( int, int );
	tuple < int, vector < Point >, bool, string, bool, Point, bool > check( Mat );
};

#endif