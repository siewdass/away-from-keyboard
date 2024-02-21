#ifndef LOCATION_H
#define LOCATION_H

#pragma once

#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Location {
public:
	int id;
	int threshold;
	Mat z;
	Mat part;
	Point3i old;
	vector < Mat > maps;
	//vector < Mat > xdxd;
	vector < vector < vector < int > > > paths;
	int width = 26, height = 27;
	Point3i find( Point3i, Mat );
	tuple < int, int, int > update( int, int );
	bool compare( Mat, Mat, Point3i );
	bool walkable( Point3i );
	Location( );
};

#endif // LOCATION_H