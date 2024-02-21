#include "location.h"
#include "screenshot.h"
#include "utils.h"
#include <string>
#include <fstream>

Screenshot s;
Mat zImg;
Point3i point;
//vector < Mat > xdxd;

Location::Location( ) {
	for ( int i = 0; i <= 15; i++ ) {
		zImg = imread( "maps/" + to_string( i ) + "-map.png" );
		maps.push_back( zImg );
		zImg = imread( "maps/" + to_string( i ) + "-path.png" );
		//xdxd.push_back( zImg );
		
		//imwrite( to_string( i ) + ".png", zImg );
		vector < vector < int > > tmp;
		for (int y = 0; y < zImg.rows; y++) {
			vector < int > row;
			for ( int x = 0; x < zImg.cols; x++) {
				Vec3b pixel = zImg.at<Vec3b>(y, x);
				row.push_back( zImg.at<Vec3b>(y, x) == Vec3b( 255, 0, 255 ) ? 0 : 1 );
			}
			tmp.push_back(row);
		}
		paths.push_back( tmp );

		/*if ( i == 8 ) {
		ofstream outFile("output_matrix.txt");

		for (const auto& row : tmp) {
			for (const auto& value : row) {
				outFile << value << " ";
			}
			outFile << endl;
		}
		outFile.close();
		}*/

	}
}

tuple < int, int, int > Location::update( int x, int y ) {

	point.x = x;
	point.y = y;
	point.z = 0;

	zImg = s.get( id, 1001, 54, 1, 61, false );

	for ( int i = 0; i < zImg.rows; i+=4 ) {
		if ( zImg.at < Vec3b > ( i, 0 ) == Vec3b( 38, 38, 37 ) ) {
			break;
		}
		point.z++;
	}

	zImg = s.get( id, 845, 5, 104, 108, true );
	//imwrite( "minimap.png", zImg );

	point = Location::find( point, zImg );


	return make_tuple( point.x, point.y, point.z );
}

Point3i Location::find( Point3i point, Mat minimap ) {
	z = maps[ point.z ];
	
	old.x = point.x;
	old.y = point.y;
	old.z = point.z;
	
	if ( compare( z, minimap, point ) ) return point;

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
	return old;
}

bool Location::compare( Mat z, Mat minimap, Point3i point ) {
	part = Mat( z, Rect( point.x - width / 2 + 1, point.y - height / 2 - 1, width, height ) );

	//imwrite( "321.png", part );
	threshold = 0;

	for ( int x = 0; x < minimap.rows - 1; x++ ) {
		for ( int y = 0; y < minimap.cols - 1; y++ ) {
			if ( threshold > 10 ) return false;
			if ( minimap.at < Vec3b > ( x, y ) != part.at < Vec3b > ( x, y ) ) threshold++;
		}
	}

	return threshold > 10 ? false : true;
}

bool Location::walkable( Point3i point ) {
	//z = pathsx[ point.z ];
	//zImg = Mat( maps[ p.z ], Rect( p.x, p.y, width, height ) );

	/*console.log( paths[ point.z ][ point.x -1 ][ point.y -1 ], paths[ point.z ][ point.x ][ point.y -1 ], paths[ point.z ][ point.x +1 ][ point.y -1 ] );
	console.log( paths[ point.z ][ point.x -1 ][ point.y    ], paths[ point.z ][ point.x ][ point.y    ], paths[ point.z ][ point.x +1 ][ point.y    ] );
	console.log( paths[ point.z ][ point.x -1 ][ point.y +1 ], paths[ point.z ][ point.x ][ point.y +1 ], paths[ point.z ][ point.x +1 ][ point.y +1 ] );*/



	//imwrite( "123.png", z );
	return true;
	//return paths[ point.z ][ point.x ][ point.y ] == 0 ? false : true;
}