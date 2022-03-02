#include "viewport.h"

#include <vector>

using namespace std;

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

tuple < int, vector < Point >, bool, string, bool, Point, bool > Viewport::check( ) {
    bool target = false;
    vector < Point > arr;
    int box = 0;

    int width = sqm * 15;
    int height = sqm * 11;
    Point TARGET;
    bool oldTarget;

    Mat GW = screenshot.get( id, 189, 40, 615, 451 );

    auto [ old, GameWindow ] = acknow.detect( GW );

    oldTarget = old;

    for ( auto [ point, targeting ] : GameWindow ) {

        Point p = Viewport::xy( point.x + 23, point.y + 35 );

        cv::Rect rect( 
            p.x * sqm + ( width / 2 ) - sqm / 2,
            p.y * sqm + ( height / 2 )- sqm / 2 -1,
            sqm, sqm 
        );

        if ( !targeting ) {
            cv::rectangle( GW, rect, cv::Scalar( 0, 255, 255 ) );
        } else {
            target = true;
            TARGET.x = p.x;
            TARGET.y = p.y;
            cv::rectangle( GW, rect, cv::Scalar( 255, 0, 0 ) );
        }

        if ( p.x == 0 && p.y == 0 ) {
            continue;
        }

        arr.push_back( p );
        if ( p.x >= -1 & p.x <= 1 & p.y >= -1 & p.y <= 1 ) {
            box += 1;
        }  
    }

    bool visibleMonsters = arr.size( ) > 0 ? true : false;

    string side;

    bool topLeft = false,
         topCenter = false,   
         topRight = false,
         midleLeft = false,
         midleRight = false,
         bottomLeft = false, 
         bottomCenter = false,
         bottomRight = false;

    if ( arr.size( ) > 0 ) {
        
        for ( Point point : arr ) {
            if ( point.x == -1 && point.y == -1 ) {
                topLeft = true;
            } else if ( point.x == 0 && point.y == -1 ) {
                topCenter = true;
            } else if ( point.x == 1 && point.y == -1 ) {
                topRight = true;
            } else if ( point.x == -1 && point.y == 0 ) {
                midleLeft = true;
            } else if ( point.x == 1 && point.y == 0 ) {
                midleRight = true;
            } else if ( point.x == -1 && point.y == 1 ) {
                bottomLeft = true;
            } else if ( point.x == 0 && point.y == 1 ) {
                bottomCenter = true;
            } else if ( point.x == 1 && point.y == 1 ) {
                bottomRight = true;
            }
        }

        if ( topLeft && topCenter && topRight ) {
            side = "Up";
        } else if ( topRight && midleRight && bottomRight ) {
            side = "Right";
        } else if ( bottomRight && bottomCenter && bottomLeft ) {
            side = "Down";
        } else if ( bottomLeft && midleLeft && topLeft ) {
            side = "Left";
        } else if ( ( topLeft && topCenter ) || ( topCenter && topRight ) || ( topLeft && topRight ) ) {
            side = "Up";
        } else if ( ( topRight && midleRight ) || ( midleRight && bottomRight ) || ( topRight && bottomRight ) ) {
            side = "Right";
        } else if ( ( bottomRight && bottomCenter ) || ( bottomCenter && bottomLeft ) || ( bottomRight && bottomLeft ) ) {
            side = "Down";
        } else if ( ( bottomLeft && midleLeft ) || ( midleLeft && topLeft ) || ( bottomLeft && topLeft ) ) {
            side = "Left";
        } else if ( topLeft || topCenter || topRight ) {
            side = "Up";
        } else if ( topRight || midleRight || bottomRight ) {
            side = "Right";
        } else if ( bottomRight || bottomCenter || bottomLeft ) {
            side = "Down";
        } else if ( bottomLeft || midleLeft || topLeft ) {
            side = "Left";
        }
    }

    imwrite( "screenshot.png", GW );

    return make_tuple( box, arr, visibleMonsters, side, target, TARGET, oldTarget );
}