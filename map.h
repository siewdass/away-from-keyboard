#ifndef MAP_H
#define MAP_H

#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Map {
public:
	vector < Mat > maps;
	vector < Mat > paths;
	int width = 26, height = 27;
	Point3i locate( Point3i, Mat );
	bool compare( Mat, Mat, Point3i );
	bool walkable( Point3i );
	Map( );
};

#endif // Map_H