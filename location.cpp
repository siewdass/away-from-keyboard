#include "utils.h"
#include "location.h"

void Location::load( ) {
	Mat img;

	for ( int i = 0; i <= 15; i++ ) {
		img = imread( "/opt/afk/maps/" + to_string( i ) + "-map.png" );
		maps.push_back( img );
		img = imread( "/opt/afk/maps/" + to_string( i ) + "-path.png" );

		vector < vector < int > > tmp;
		for (int x = 0; x < img.rows; x++) {
			vector < int > row;
			for ( int y = 0; y < img.cols; y++ ) {
				row.push_back( img.at < Vec3b > ( x, y ) == Vec3b( 255, 0, 255 ) ? 0 : 1 );
			}
			tmp.push_back( row );
		}
		paths.push_back( tmp );
	}
}

tuple < int, int, int > Location::update( Mat FloorIndex, Mat Minimap, int x, int y ) {
	Point3i point;

	point.x = x == 0 ? maps[ 0 ].cols / 2 : x;
	point.y = y == 0 ? maps[ 0 ].rows / 2 : y;
	point.z = 0;

	for ( int i = 0; i < FloorIndex.rows; i+=4 ) {
		if ( FloorIndex.at < Vec3b > ( i, 0 ) == Vec3b( 38, 38, 37 ) ) {
			break;
		}
		point.z++;
	}

	//imwrite( "minimap.png", Minimap );

	point = Location::find( point, Minimap );

	return make_tuple( point.x, point.y, point.z );
}

Point3i Location::find( Point3i point, Mat minimap ) {
	Point3i old = point;

	try {
		if ( compare( point, minimap ) ) return point;

		Mat z = maps[ point.z ];

		int x, y, grow = 0;

		while ( grow < max( z.cols, z.rows) ) {
			grow += 2;
			point.x--;
			point.y--;

			x = point.x;
			while ( point.x < z.rows && point.x < x + grow ) {
				if ( compare( point, minimap ) ) return point;
				point.x++;
			}

			y = point.y;
			while ( point.y < z.cols && point.y < y + grow ) {
				if ( compare( point, minimap ) ) return point;
				point.y++;
			}

			x = point.x;
			while ( point.x >= 0 && point.x > x - grow ) {
				if ( compare( point, minimap ) ) return point;
				point.x--;
			}

			y = point.y;
			while ( point.y >= 0 && point.y > y - grow ) {
				if ( compare( point, minimap ) ) return point;
				point.y--;
			}
		}

		return old;
	} catch ( exception &e ) {
		return old;
    }

	return old;
}

bool Location::compare( Point3i point, Mat minimap ) {
	try {
		if ( paths[ point.z ][ point.y ][ point.x ] == 0 ) return false;

		Mat part = Mat( maps[ point.z ], Rect( point.x - 16, point.y - 16, 34, 34 ) );

		//imwrite( "part.png", part );
		int threshold = 0;

		for ( int x = 0; x < minimap.rows - 1; x++ ) {
			for ( int y = 0; y < minimap.cols - 1; y++ ) {
				if ( minimap.at < Vec3b > ( x, y ) == Vec3b( 255, 255, 255 ) ) continue; // CANT
				if ( threshold > 20 ) return false;
				if ( minimap.at < Vec3b > ( x, y ) != part.at < Vec3b > ( x, y ) ) threshold++;
			}
		}

		return true;
	} catch ( exception &e ) {
		return false;
    }
}