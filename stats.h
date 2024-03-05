#ifndef STATS_H
#define STATS_H

#pragma once

#include "types.h"
#include "shared.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

class Stats {
public:
	int health, mana, middle = ( 1012 / 2 );
	bool hungry, paralyze, poison, isTargeting, haste;
	tuple < int, int, bool, bool, bool, bool, vector < Point >, bool, bool, bool, bool, bool, bool > update( Mat, Mat, Mat, Mat, Mat, Mat, Mat, Mat, Mat, Mat, Mat );
	bool valuableLoot( Mat );
};

#endif
