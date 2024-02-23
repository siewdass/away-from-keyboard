#include "location.h"
#include "screenshot.h"

#include <string>
#include <fstream>

Screenshot s;
Mat img;
Point3i point;

void Location::load( ) {
	for ( int i = 0; i <= 15; i++ ) {
		img = imread( "maps/" + to_string( i ) + "-map.png" );
		maps.push_back( img );
		img = imread( "maps/" + to_string( i ) + "-path.png" );

		vector < vector < int > > tmp;
		for (int x = 0; x < img.rows; x++) {
			vector < int > row;
			for ( int y = 0; y < img.cols; y++ ) {
				row.push_back( img.at < Vec3b > ( x, y ) == Vec3b( 255, 0, 255 ) ? 0 : 1 );
			}
			tmp.push_back( row );
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

	img = s.get( id, 1001, 54, 1, 61, false );

	for ( int i = 0; i < img.rows; i+=4 ) {
		if ( img.at < Vec3b > ( i, 0 ) == Vec3b( 38, 38, 37 ) ) {
			break;
		}
		point.z++;
	}

	img = s.get( id, 898 - 16, 59 - 16, 34, 34, false );
	imwrite( "minimap.png", img );

	point = Location::find( point, img );

	return make_tuple( point.x, point.y, point.z );
}

bool xx= true;
Point3i Location::find( Point3i point, Mat minimap ) {
	z = maps[ point.z ];
	
	old.x = point.x;
	old.y = point.y;
	old.z = point.z;

	if ( xx ) {
		part = Mat( maps[ point.z ], Rect( point.x - 16, point.y - 16, 34, 34 ) );
		imwrite( "xyz.png", part );
		xx = false;
	}


	if ( compare( point, minimap ) ) return point;

	int x = 0, y = 0, grow = 0;
	bool xmin = true, xmax = true, ymin = true, ymax = true;

	while ( grow < 140 ) {
		grow = grow + 2;
		point.x--;
		point.y--;

		x = point.x;
		while ( xmax && point.x < x + grow ) {
			if ( walkable( point ) && compare( point, minimap ) ) return point;
			point.x++;
			if ( point.x > z.rows - 1 ) xmax = false;
		}

		y = point.y;
		while ( ymax && point.y < y + grow ) {
			if ( walkable( point ) && compare( point, minimap ) ) return point;
			point.y++;
			if ( point.y > z.cols - 1 ) ymax = false;
		}

		x = point.x;
		while ( xmin && point.x > x - grow ) {
			if ( walkable( point ) && compare( point, minimap ) ) return point;
			point.x--;
			if ( point.x < 0 ) xmin = false;
		}

		y = point.y;
		while ( ymin && point.y > y - grow ) {
			if ( walkable( point ) && compare( point, minimap ) ) return point;
			point.y--;
			if ( point.y < 0 ) ymin = false;
		}
	}
	return old;
}

bool Location::compare( Point3i point, Mat minimap ) {
	part = Mat( maps[ point.z ], Rect( point.x - 16, point.y - 16, 34, 34 ) );

	imwrite( "321.png", part );
	threshold = 0;

	for ( int x = 0; x < minimap.rows - 1; x++ ) {
		for ( int y = 0; y < minimap.cols - 1; y++ ) {
			if ( minimap.at < Vec3b > ( x, y ) == Vec3b( 255, 255, 255 ) ) continue; // CANT
			if ( threshold > 10 ) return false;
			if ( minimap.at < Vec3b > ( x, y ) != part.at < Vec3b > ( x, y ) ) threshold++;
		}
	}

	return true;
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