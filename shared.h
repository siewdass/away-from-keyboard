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
		tuple < int, int, int, string, string > get( ) {
			int id, width, height;
			string user, name;
			try {
				id = stoi( process( "DISPLAY=:1 xdotool search --onlyvisible --class tibia" ) );
				width = stoi( process( "DISPLAY=:1 xdotool search --onlyvisible --class tibia getwindowgeometry | grep Geometry | cut -d ' ' -f 4 | cut -d 'x' -f 1" ) );
				height = stoi( process( "DISPLAY=:1 xdotool search --onlyvisible --class tibia getwindowgeometry | grep Geometry | cut -d ' ' -f 4 | cut -d 'x' -f 2" ) );
				user = process( "whoami" );
				user.pop_back( );
				name = process( "DISPLAY=:1 xdotool search --onlyvisible --class tibia getwindowname | cut -d ' ' -f 3" );
				name.pop_back( );
			} catch ( exception & error ) {
			}
			return make_tuple( id, width, height, user, name);
		}
		string process( string cmd ) {
			const int MAX_BUFFER = 2048;
			char buffer[ MAX_BUFFER ];
			FILE *stream = popen( cmd.c_str( ) , "r" );
			if ( stream ) {
				while ( !feof( stream ) ) {
					if ( fgets( buffer, MAX_BUFFER, stream ) != NULL) { }
				}
				pclose( stream );
			}
			string v{ buffer };
			return v;
		}
};

#endif // SHARED_H