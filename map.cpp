#include "map.h"

Map::Map( ) {
	Mat img;
	for ( int i = 0; i <= 15; i++ ) {  
		img = imread( "maps/" + to_string( i ) + "-map.png" );
		maps.push_back( img );
		img = imread( "maps/" + to_string( i ) + "-path.png" );
		paths.push_back( img );
	}
}

Point3i Map::locate( Point3i point, Mat minimap ) {
	Mat z = maps[ point.z ];
	// check if not x13 y13
	int x = 0, y = 0, grow = 0;
	bool xmin = true, xmax = true, ymin = true, ymax = true;

	while ( grow < 140 ) {
		grow = grow + 2;
		point.x--;
		point.y--;

		x = point.x;
		while ( xmax && point.x < x + grow ) {
			if ( walkable( point ) && compare( z, minimap, point ) ) return point;
			point.x++;
			if ( point.x > z.rows - 1 ) xmax = false;
		}

		y = point.y;
		while ( ymax && point.y < y + grow ) {
			if ( walkable( point ) && compare( z, minimap, point ) ) return point;
			point.y++;
			if ( point.y > z.cols - 1 ) ymax = false;
		}

		x = point.x;
		while ( xmin && point.x > x - grow ) {
			if ( walkable( point ) && compare( z, minimap, point ) ) return point; 
			point.x--;
			if ( point.x < 0 ) xmin = false;
		}

		y = point.y;
		while ( ymin && point.y > y - grow ) {
			if ( walkable( point ) && compare( z, minimap, point ) ) return point;
			point.y--;
			if ( point.y < 0 ) ymin = false;
		}
	}
	return Point3i( 0, 0, 0 );
}


bool Map::compare( Mat z, Mat minimap, Point3i point ) {
	Mat part = Mat( z, Rect( point.x - width / 2, point.y - height / 2, width, height ) );

	int threshold = 0;
	// reuse vars

	for ( int x = 0; x < minimap.rows - 1; x++ ) {
		for ( int y = 0; y < minimap.cols - 1; y++ ) {
			if ( threshold > 10 ) return false;
			if ( minimap.at < Vec3b > ( x, y ) != part.at < Vec3b > ( x, y ) ) threshold++;
		}
	}

	return threshold > 10 ? false : true;
}

bool Map::walkable( Point3i point ) {
	Vec3b pixel = paths[ point.z ].at < Vec3b > ( point.y, point.x );
	return pixel == Vec3b( 255, 0, 255 ) ? false : true;
}