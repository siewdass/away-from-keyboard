#ifndef LOCATION_H
#define LOCATION_H

#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Location {
public:
	vector < Mat > maps;
	vector < vector < vector < int > > > paths;
	Point3i find( Point3i, Mat );
	tuple < int, int, int, int > update( Mat, Mat, int, int );
	bool compare( Point3i, Mat );
	void load( );
	Location( ) {}
	Point3i old;
};

#endif // LOCATION_H