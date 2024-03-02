#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main( ) {
	Mat z = imread( "minimap.png" );

	int x, y, grow = 0;
	//bool xmin = true, xmax = true, ymin = true, ymax = true;
    Point point( z.rows / 2, z.cols / 2 );

    z.at < Vec3b > ( point.y, point.x ) = Vec3b( 255, 0, 0 );
    
    grow += 2;

    point.x--;
    point.y--;

    x = point.x;
    if ( point.y - 1 > z ) {
        
    } 
    while ( point.x <= x + grow ) {
        point.x++;
        z.at < Vec3b > ( point.y, point.x ) = Vec3b( 0, 0, 0 );
        //if ( point.x > z.rows - 1 ) break;
    }



	//while ( true ) {
		//point.x--;
		//point.y--;

		/*x = point.x;
		while ( point.x >= 0 && point.x < z.rows && point.x < x + grow + 2 ) {

			point.x++;
			if ( point.x > z.rows - 1 ) xmax = false;
		}*/

		/*y = point.y;
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
		}*/
	//	grow += 2;
	//}

    namedWindow("test", WINDOW_NORMAL);
    resizeWindow("test", 800, 600); 
    imshow("test", z);
    waitKey(0);

    return 0;
}