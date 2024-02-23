#ifndef TYPES_H
#define TYPES_H

#include <string>
#pragma once
using namespace std;

struct Waypoint {
	int x, y, z;
	string action = "attack"; // walk, rope, stairs
	int direction = 1;
};

struct Position {
	int x, y, z;
	string img;
};

struct Pixel {
	int r, g, b;
};


#endif // TYPES_H