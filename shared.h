#ifndef SHARED_H
#define SHARED_H

#pragma once

#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

class Values {
public:
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
	bool boxTarget;
	bool alarm;
	bool weapon, necklace, ring;
	bool lowcapacity = false;
};

class Mode {
public:
	bool walking = true, 
		attacking = false,
		looting = false;
};

class Client {
	public:
		int get( ) {
			const int MAX_BUFFER = 2048;
			
			string cmd = "xdotool search --onlyvisible --class tibia";
			
			char buffer[ MAX_BUFFER ];
			FILE *stream = popen( cmd.c_str( ) , "r" );
			if ( stream ){
			while ( !feof( stream ) ) {
					if ( fgets( buffer, MAX_BUFFER, stream ) != NULL) {
					}
			}
			pclose( stream );
			}

			string s1{ buffer };

			try {
				int d = stoi( s1 );
				return d;
			} catch ( exception & error ) {
				return 0;
			}
		}
};

#endif // SHARED_H