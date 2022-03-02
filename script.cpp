#include "script.h"
#include "utils.h"

void Script::load( string file ) {
	Json config;
	config.open( file.c_str( ) );
	for ( Json Z : config.readArray( "map" ) ) {
		vector < vector < int > > mapZ;
		for ( Json Y : Z.readArray( ) ) {
			vector < int > mapY;
			for ( Json X : Y.readArray( ) ) {
				mapY.push_back( X.readInteger( ) );
			}
			mapZ.push_back( mapY );
		}
		Script::map.push_back( mapZ );
	}
	
	for ( Json waypoint: config.readArray( "waypoints" ) ) {
		Waypoint wp;
		wp.x = waypoint.readInteger( "x" );
		wp.y = waypoint.readInteger( "y" );
		wp.z = waypoint.readInteger( "z" );
		wp.action = waypoint.readString( "action" );
		Script::waypoints.push_back( wp );
	}

	for ( Json position: config.readArray( "positions" ) ) {
		Position pos;
		pos.x = position.readInteger( "x" );
		pos.y = position.readInteger( "y" );
		pos.z = position.readInteger( "z" );
		pos.img = position.readString( "img" );
		Script::positions.push_back( pos );
	}
}
