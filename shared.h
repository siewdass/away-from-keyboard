#ifndef SHARED_H
#define SHARED_H

#pragma once

#include <string>
#include <iostream>
#include <queue>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

struct PathNode {
    Point pos;
    Point parent;
    int g;  // Cost from start

    PathNode(Point p, Point par, int cost) 
        : pos(p), parent(par), g(cost) {}
};

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

class Player {
public:
	Player( vector < Waypoint > waypoints ) {
		x = waypoints.size() > 0 ? waypoints[0].x : 0;
		y = waypoints.size() > 0 ? waypoints[0].y : 0;
	}
	int validSqm;
	int health = 267, mana = 267;
	bool hungry, paralyze, poison, haste;
	bool isTargeting, isAttacking, isWalking, isLooting, visibleMonsters;
	int x = 0, y = 0, z = 0;
	string minimap;
	int box;
	vector < Point > monsters;
	string side;
	vector < Point > flasks;
	Point targetPosition;
	bool target;
	bool alarm;
	bool weapon, necklace, ring;
	bool lowcapacity = false;
	bool canWalk;
	bool nearPlayer;
};

class Mode {
public:
	bool walking = true, attacking = false, looting = false;
};

#endif // SHARED_H
