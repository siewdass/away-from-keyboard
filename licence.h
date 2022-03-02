#ifndef LICENCE_H
#define LICENCE_H

#include "types.h"
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;

class Licence {
public:
	vector < string > accepted;
	void load( string );
};

#endif // LICENCE_H