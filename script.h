#ifndef SCRIPT_H
#define SCRIPT_H

#include "types.h"
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;

class Script {
	public:
		vector < vector < vector < int > > > map;
		vector < Waypoint > waypoints;
		vector < Position > positions;
		void load( string );
};

#endif // SCRIPT_H