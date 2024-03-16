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

void Location::walkable( vector < Waypoint > walkable ) {
	for ( Waypoint wp : walkable ) paths[ wp.z ][ wp.y ][ wp.x ] = 0;
}

tuple < int, int, int, int > Location::update( Mat FloorIndex, Mat Minimap, int x, int y ) {
	Point3i point;

	point.x = x;
	point.y = y;
	point.z = 0;

	for ( int i = 0; i < FloorIndex.rows; i+=4 ) {
		if ( FloorIndex.at < Vec3b > ( i, 0 ) == Vec3b( 38, 38, 37 ) ) break;
		point.z++;
	}

	if ( x == 0 && y == 0 ) {
		for ( int i = 16; i < maps[ 0 ].rows - 16; i++ ) {
			for ( int j = 16; j < maps[ 0 ].cols - 16; j++ ) {
				if ( compare( Point3i( j, i, point.z ), Minimap ) ) point = Point3i( j, i, point.z );
			}
		}
	} else {
		point = Location::find( point, Minimap );
	}

	int valid = -1;

	for ( int i = point.y - 1; i < point.y + 2; ++i ) {
		for ( int j = point.x - 1; j < point.x + 2; ++j ) {
			if ( paths[ point.z ][ i ][ j ] == 1 ) valid++;
		}
	}

	//imwrite( "minimap.png", Minimap );

	return make_tuple( point.x, point.y, point.z, valid );
}

Point3i Location::find( Point3i point, Mat minimap ) {
	Point3i old = point; 
	try {
		if ( compare( point, minimap ) ) return point;

		Mat z = maps[ point.z ];

		int x, y, grow = 0;

		while ( grow < 150 ) {
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
	
		/*int i, rows = 2559, cols = 2047;
		int startX = point.x, startY = point.y;
		int low_row = ( 0 > startX ) ? 0 : startX;
		int low_column = ( 0 > startY ) ? 0 : startY;
		int high_row = ( ( startX + 1 ) >= rows ) ? rows - 1 : startX + 1;
		int high_column = ( ( startY + 1 ) >= cols ) ? cols - 1 : startY + 1;

		while ( low_row > 0 - rows && low_column > 0 - cols ) {
			for ( i = low_column + 1; i <= high_column && i < cols && low_row >= 0; ++i ) if ( compare( Point3i( low_row, i, point.z ), minimap ) ) return Point3i( low_row, i, point.z );
			low_row -= 1;

			for ( i = low_row + 2; i <= high_row && i < rows && high_column < cols; ++i ) if ( compare( Point3i( i, high_column, point.z ), minimap ) ) return Point3i( i, high_column, point.z );
			high_column += 1;

			for ( i = high_column - 2; i >= low_column && i >= 0 && high_row < rows; --i ) if ( compare( Point3i( high_row, i, point.z ), minimap ) ) return Point3i( high_row, i, point.z );
			high_row += 1;

			for ( i = high_row - 2; i > low_row && i >= 0 && low_column >= 0; --i ) if ( compare( Point3i( i, low_column, point.z ), minimap ) ) return Point3i( i, low_column, point.z );
			low_column -= 1;
		}*/

	} catch ( exception &e ) {
    }

	return old;
}

bool Location::compare( Point3i point, Mat minimap ) {
	try {
		if ( paths[ point.z ][ point.y ][ point.x ] == 0 && point.x >= 16 && point.x <= ( 2559 - 16 ) && point.y >= 16 && point.y <= ( 2559 - 16 ) ) return false;

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