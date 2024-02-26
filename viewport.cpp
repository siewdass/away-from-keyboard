#include "acknow.h"
#include "viewport.h"

Acknow acknowv;

using namespace std;

vector < Point > upV{ Point( -1,-1 ), Point( 0,-1 ), Point( 1,-1 ) };
vector < Point > rightV{ Point( 1,-1 ), Point( 1,0 ), Point( 1,1 ) };
vector < Point > downV{ Point( -1,1 ), Point( 0,1 ), Point( 1,1 ) };
vector < Point > leftV{ Point( -1,-1 ), Point( -1,0 ), Point( -1,1 ) };

Point Viewport::xy( int x, int y ) {

    vector < int > arr;

    int count = 0;

    for ( int i = 0; i <= 15; i++ ) {
        arr.push_back( count );
        count += Viewport::sqm;
    }

    int X = 0, Y = 0;

    for ( int i = 1; i < arr.size( ); i++ ) {
        if ( x <= arr[ i ] ) {
            if ( i == 1 ) {
                X = -7;
            } else if ( i == 2 ) {
                X = -6;
            } else if ( i == 3 ) {
                X = -5;
            } else if ( i == 4 ) {
                X = -4;
            } else if ( i == 5 ) {
                X = -3;
            } else if ( i == 6 ) {
                X = -2;
            } else if ( i == 7 ) {
                X = -1;
            } else if ( i == 8 ) {
                X = 0;
            } else if ( i == 9 ) {
                X = 1;
            } else if ( i == 10 ) {
                X = 2;
            } else if ( i == 11 ) {
                X = 3;
            } else if ( i == 12 ) {
                X = 4;
            } else if ( i == 13 ) {
                X = 5;
            } else if ( i == 14 ) {
                X = 6;
            } else if ( i == 15 ) {
                X = 7;
            }
            break;
        }
    }
    
    for ( int i = 1; i < arr.size( ); i++ ) {
        if ( y <= arr[ i ] ) {
            if ( i == 1 ) {
                Y = -5;
            } else if ( i == 2 ) {
                Y = -4;
            } else if ( i == 3 ) {
                Y = -3;
            } else if ( i == 4 ) {
                Y = -2;
            } else if ( i == 5 ) {
                Y = -1;
            } else if ( i == 6 ) {
                Y = 0;
            } else if ( i == 7 ) {
                Y = 1;
            } else if ( i == 8 ) {
                Y = 2;
            } else if ( i == 9 ) {
                Y = 3;
            } else if ( i == 10 ) {
                Y = 4;
            } else if ( i == 11 ) {
                Y = 5;
            }
            break;
        }
    }

    return Point( X, Y );
}

tuple < int, vector < Point >, string, bool, Point, bool > Viewport::check( Mat GW ) {
    int box = 0;
    vector < Point > arr;
    string side = "";
    bool target;
    Point TARGET;
    bool oldTarget;

    int width = sqm * 15;
    int height = sqm * 11;
    
    auto [ old, GameWindow ] = acknowv.detect( GW );

    oldTarget = old;

    for ( auto [ point, targeting ] : GameWindow ) {
        Point p = Viewport::xy( point.x + 23, point.y + 35 );

        if ( targeting ) {
            target = true;
            TARGET.x = p.x;
            TARGET.y = p.y;
        }

        if ( p.x == 0 && p.y == 0 ) continue;
        
        arr.push_back( p );
        if ( p.x >= -1 & p.x <= 1 & p.y >= -1 & p.y <= 1 ) box++;
    }

    bool topLeft = false,
         topCenter = false,   
         topRight = false,
         midleLeft = false,
         midleRight = false,
         bottomLeft = false, 
         bottomCenter = false,
         bottomRight = false;

    if ( arr.size( ) > 0 ) {
        
        int upCount = 0;
        int rightCount = 0;
        int downCount = 0;
        int leftCount = 0;

        for ( Point point : arr ) {
            for ( Point p : upV ) if ( point.x == p.x && point.y == p.y ) upCount++;
            for ( Point p : rightV ) if ( point.x == p.x && point.y == p.y ) rightCount++;
            for ( Point p : downV ) if ( point.x == p.x && point.y == p.y ) downCount++;
            for ( Point p : leftV ) if ( point.x == p.x && point.y == p.y ) leftCount++;
        }       

        if ( upCount == 3 ) {
            side = "Up";
        } else if ( rightCount == 3 ) {
            side = "Right";
        } else if ( downCount == 3 ) {
            side = "Down";
        } else if ( leftCount == 3 ) {
            side = "Left";
        } else if ( upCount == 2 ) {
            side = "Up";
        } else if ( rightCount == 2 ) {
            side = "Right";
        } else if ( downCount == 2 ) {
            side = "Down";
        } else if ( leftCount == 2 ) {
            side = "Left";
        }
    }

    //imwrite( "screenshot.png", GW );

    return make_tuple( box, arr, side, target, TARGET, oldTarget );
}

   