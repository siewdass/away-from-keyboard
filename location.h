#ifndef LOCATION_H
#define LOCATION_H

#pragma once

#include <vector>
#include <opencv2/opencv.hpp>
#include "shared.h"
#include <vector>

using namespace std;
using namespace cv;

class Location {
public:
	vector < Mat > maps;
	vector < vector < vector < int > > > paths;
	tuple < int, int > find( int, int, int, Mat );
	tuple < int, int, int, int > update( Mat, Mat, int, int );
	bool compare( int, int, int, Mat );
	void load( );
	Location( ) {}
	void walkable( vector < Waypoint > );
};

#endif // LOCATION_H