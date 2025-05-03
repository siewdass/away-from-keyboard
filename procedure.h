#pragma once
#include "util.h"
#include "character.h"
#include "pathfinder.h"

Pathfinder pathfinder;

class {
public:

    vector<Mat> maps;
    vector<vector<vector<int>>> paths;

    tuple<int, int> gameWindow( int x, int y ) {
        int sqm = 41;
        int width = sqm * 15, height = sqm * 11;
        int left = 210, top = 55;
        int X = x * sqm + ( width / 2 ) + left - ( sqm / 2 );
        int Y = y * sqm + ( height / 2 ) + top - ( sqm / 2 );
        return { X, Y };
    }

    Point xy( int x, int y ) {

        vector < int > arr;

        int count = 0;

        for ( int i = 0; i <= 15; i++ ) {
            arr.push_back( count );
            count += 41;
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

    tuple<bool, int, int> walker( Player player, Character character, vector < vector < vector < int > > > paths, atomic<int>& position ) {

        bool attack = false;
        int left = 898, top = 59, pixel = 1;
        int x = 0, y = 0;

        if ( abs( player.x - character.waypoints[ position ].x ) <= 1 && abs( player.y - character.waypoints[ position ].y ) <= 1 ) {
            console.log( "action", character.waypoints[ position ].action );
            attack = character.waypoints[ position ].action == "attack" ? true : false;
            position += 1;
            if ( attack && player.monsters.size( ) > 0 ) return { attack, x, y };
        }

        if ( position == character.waypoints.size( ) ) position = 0;

        pathfinder.loadMap(paths[ player.z ]);

        auto ways = pathfinder.findPath(
            Point(player.x, player.y),
            Point( character.waypoints[ position ].x, character.waypoints[ position ].y )
        );

        if ( ways.size( ) > 0 ) {
            x = ways[ ways.size( ) >= 57 ? 56 : ways.size( ) - 1 ].x;
            y = ways[ ways.size( ) >= 57 ? 56 : ways.size( ) - 1 ].y;

            if ( character.waypoints[ position ].action == "walk" ) {
                x += other.plusMinus();
                y += other.plusMinus();
            }

            x = left + ( pixel * ( x - player.x ) );
            y = top + ( pixel * ( y - player.y ) );
            
            console.log( "from x:", player.x, "from y:", player.y, "to x:", y, "to y:", y );
        }

        return { attack, x, y };
    }

    vector<tuple<int, int>> throwFlask( Player player ) {
        vector<tuple<int, int>> flasks;
        for ( int i = 0; i < player.flasks.size( ); i++ ) {
            Point flask = player.flasks[ player.flasks.size( ) -1 ];
            flasks.push_back( { 836 + flask.x, 290 + flask.y } );
        }
        return flasks;
    }

    vector<tuple<int, int>> looter( ) {

        vector < Point > Around = { 
            Point( -1, -1 ), 
            Point( 0, -1 ), 
            Point( 1, -1 ),
            Point( 1, 0 ),
            Point( 1, 1 ),
            Point( 0, 1 ),
            Point( -1, 1 ), 
            Point( -1, 0 ),
            Point( 0, 0 )
        };

        vector < tuple < int, int > > All;

        for ( Point point : Around ) {
            auto [ x, y ] = gameWindow( point.x, point.y );
            All.push_back( { x, y } );
        }

        return All;
    }

    tuple<int, int, bool, bool, bool, bool, vector<Point>, bool, bool, bool, bool, bool, bool, bool> stats( Mat CAPACITY, Mat WEAPON, Mat AMULET, Mat RING, Mat BATTLE, Mat CHAT, Mat HP, Mat MP, Mat BD, Mat FK, Mat Monsters, int AlarmCount ) {

        vector<Pixel> monsters = inspect( Monsters );

        bool target = false;

        for ( Pixel pixel : monsters ) {
            if ( pixel.r == 255 && pixel.g == 0 && pixel.b == 0 ) {
                target = true;
                break;   
            }
        }

        vector<Pixel> capacity = inspect( CAPACITY );

        int cap = 0;
        bool lowcap = false;

        for ( Pixel pixel : capacity ) {
            int r = pixel.r, g = pixel.g, b = pixel.b;
            if ( r <= 55 && g <= 55 && b <= 55 ) {
                cap += 1;   
            }
        }

        if ( cap >= 22 ) {
            lowcap = true;
        }

        bool weaponEquipped = false;

        vector<Pixel> weapon = inspect( WEAPON );

        for ( Pixel pixel : weapon ) {
            int r = pixel.r, g = pixel.g, b = pixel.b;
            if ( r == 94 && g == 96 && b == 98 ) {
                weaponEquipped = true;
                break;
            }
        }

        bool amuletEquipped = true;

        vector<Pixel> amulet = inspect( AMULET );

        for ( Pixel pixel : amulet ) {
            int r = pixel.r, g = pixel.g, b = pixel.b;
            if ( r == 71 && g == 73 && b == 76 ) {
                amuletEquipped = false;
                break;
            }
        }

        bool ringEquipped = true;

        vector<Pixel> ring = inspect( RING );

        for ( Pixel pixel : ring ) {
            int r = pixel.r, g = pixel.g, b = pixel.b;
            if ( r == 88 && g == 91 && b == 94 ) {
                ringEquipped = false;
                break;
            }
        }

        bool near = false;
        bool alarm = false;

        vector<Pixel> battle = inspect( BATTLE );

        for ( Pixel pixel : battle ) {
            int r = pixel.r, g = pixel.g, b = pixel.b;
            if ( r == 0 && g == 0 && b == 0 ) {   
                //std::cout << r << " " << g << " " << b << std::endl;         
                near = true;
            }
        }

        if ( AlarmCount >= 4 ) { AlarmCount = 0; alarm = true; }

        vector<Pixel> chat = inspect( CHAT );

        for ( Pixel pixel : chat ) {
            int r = pixel.r, g = pixel.g, b = pixel.b;
            if ( r == 96 && g == 248 && b == 248 ) {
                alarm = true;
                break;
            }
        }

        vector<Pixel> HealthPoints = inspect( HP );
        int health = (1012 / 2)-188-40;

        for ( Pixel pixel : HealthPoints ) {
            int r = pixel.r, g = pixel.g, b = pixel.b;
            if ( r >= 32 && r <= 46 && g >= 32 && g <= 46 && b >= 32 && b <= 46 ) {
                health -= 1;
            }
        }

        vector<Pixel> ManaPoints = inspect( MP );
        int mana = 0;

        for ( Pixel pixel : ManaPoints ) {
            int r = pixel.r, g = pixel.g, b = pixel.b;
            if ( r == 0 & g == 41 & b == 91 ) {
                mana += 1;
            } else if ( r == 0 & g == 44 & b == 91 ) {
                mana += 1;
            }
        }

        vector<Pixel> BenefitsAndDamages = inspect( BD );
        bool hungry = false;
        bool paralyze = false;
        bool poison = false;
        bool haste = false;

        for ( Pixel pixel : BenefitsAndDamages ) {
            int r = pixel.r, g = pixel.g, b = pixel.b;
            if ( r >= 220 & r <= 250 & g >= 200 & g <= 220 & b >= 141 & b <= 160 ) {
                hungry = true;
            } else if ( r == 255 & g == 0 & b == 0 ) {
                paralyze = true;
            } else if ( r >= 60 & r <= 100 & g >= 190 & g <= 255 & b >= 80 & b <= 165 ) {
                poison = true;
            } else if ( r >= 60 & r <= 100 & g >= 190 & g <= 255 & b >= 80 & b <= 165 ) {
                poison = true;
            } else if ( r == 176 && g == 139 && b == 80 ) {
                haste = true;
            }
        }

        vector < Point > Flask = findflask( FK );

        return {
            health,
            mana,
            hungry,
            paralyze,
            poison,
            haste,
            Flask,
            alarm,
            near,
            weaponEquipped,
            amuletEquipped,
            ringEquipped,
            lowcap,
            target
        };
    }

    tuple < int, vector < Point >, string, bool, Point, bool > viewport( Mat GW ) {
        int box = 0;
        vector < Point > arr;
        string side = "";
        bool target;
        Point TARGET;
        bool oldTarget;

        int width = 41 * 15;
        int height = 41 * 11;
        
        auto [ old, GameWindow ] = detect( GW );
        
        oldTarget = old;

        for ( auto [ point, targeting ] : GameWindow ) {
            Point p = xy( point.x + 23, point.y + 35 );

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
                for ( Point p : { Point( -1,-1 ), Point( 0,-1 ), Point( 1,-1 ) } ) if ( point.x == p.x && point.y == p.y ) upCount++;
                for ( Point p : { Point( 1,-1 ), Point( 1,0 ), Point( 1,1 ) } ) if ( point.x == p.x && point.y == p.y ) rightCount++;
                for ( Point p : { Point( -1,1 ), Point( 0,1 ), Point( 1,1 ) } ) if ( point.x == p.x && point.y == p.y ) downCount++;
                for ( Point p : { Point( -1,-1 ), Point( -1,0 ), Point( -1,1 ) } ) if ( point.x == p.x && point.y == p.y ) leftCount++;
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

    vector < Point > findflask( Mat img ) {

        vector < Point > arr;
        Pixel pixel;
        vector < tuple < int, int, int > > NORMAL {
            { 87, 87, 87 }, { 81, 81, 81 }, { 56, 57, 56 }, { 49, 49, 49 },
            { 44, 45, 45 }, { 40, 40, 40 }, { 40, 40, 40 }, { 36, 36, 36 },
            { 45, 46, 46 }, { 0, 0, 24 }, { 0, 0, 24 }, { 0, 0, 24 }, { 0, 0, 24 },
            { 39, 40, 40 }, { 39, 39, 40 }, { 44, 44, 44 }, { 45, 46, 46 }, { 32, 32, 32 },
            { 40, 40, 41 }, { 34, 34, 35 }, { 24, 25, 25 }, { 2, 2, 5 }, 
            { 38, 38, 39 }, { 36, 36, 37 }, { 44, 44, 44 }, { 44, 45, 45 },
            { 57, 57, 57 }, { 62, 63, 63 }, { 63, 64, 64 }, { 87, 87, 87 }
        };
        vector < tuple < int, int, int > > STRONG {
            { 87, 87, 87 }, { 81, 81, 81 }, { 56, 57, 56 }, { 49, 49, 49 },
            { 0, 0, 0 }, { 101, 100, 100 }, { 146, 146, 147 }, { 42, 42, 45 },
            { 45, 46, 46 }, { 7, 5, 3 }, { 40, 40, 41 }, { 40, 40, 41 }, { 36, 36, 36 },
            { 39, 40, 40 }, { 39, 39, 40 }, { 7, 5, 3 }, { 45, 46, 46 }, { 32, 32, 32 },
            { 40, 40, 41 }, { 34, 34, 35 }, { 24, 25, 25 }, { 66, 66, 66 },
            { 31, 26, 24 }, { 14, 14, 25 }, { 92, 91, 91 }, { 0, 0, 0 },
            { 57, 57, 57 }, { 62, 63, 63 }, { 63, 64, 64 }, { 87, 87, 87 }
        };
        vector < tuple < int, int, int > > GREAT {
            { 87, 87, 87 }, { 81, 81, 81 }, { 56, 57, 56 }, { 49, 49, 49 },
            { 44, 45, 45 }, { 0, 0, 0 }, { 0, 0, 0 }, { 63, 63, 63 }, { 45, 46, 46 },
            { 36, 36, 36 }, { 40, 40, 41 }, { 40, 40, 41 }, { 36, 36, 36 },
            { 39, 40, 40 }, { 39, 39, 40 }, { 44, 44, 44 }, { 52, 52, 52 },
            { 52, 52, 52 }, { 40, 40, 41 }, { 97, 97, 97 }, { 24, 25, 25 },
            { 30, 30, 31 }, { 0, 0, 0 }, { 154, 52, 0 }, { 191, 91, 0 },
            { 44, 45, 45 }, { 57, 57, 57 }, { 62, 63, 63 }, { 0, 0, 0 }, { 87, 87, 87 }
        };

        try {

            if ( !img.empty( ) ) {

                int width = img.cols, height = img.rows;
                int total = 30;

                for ( int y = 0; y <= height; y++ ) {
                    for ( int x = 0; x <= width; x++ ) {

                        int normal = 0, strong = 0, great = 0;

                        pixel = rgb( img, x, y );
                        Pixel pixel2 = rgb( img, x + 1, y );
                        Pixel pixel3 = rgb( img, x + 2, y );

                        if ( pixel.r == 87 & pixel.g == 87 & pixel.b == 87 &&
                            pixel2.r == 81 & pixel2.g == 81 & pixel2.b == 81 &&
                            pixel3.r == 56 & pixel3.g == 57 & pixel3.b == 56 ) {
                            
                            for ( int i = 0; i <= NORMAL.size( ); i++ ) {
                                pixel = rgb( img, x + i, y );
                                auto [ r, g, b ] = NORMAL[ i ];
                                if ( pixel.r == r && pixel.g == g && pixel.b == b ) {
                                    normal += 1;
                                } else {
                                    i == NORMAL.size( );
                                }
                            }

                            for ( int i = 0; i <= STRONG.size( ); i++ ) {
                                pixel = rgb( img, x + i, y );
                                auto [ r, g, b ] = STRONG[ i ];
                                if ( pixel.r == r && pixel.g == g && pixel.b == b ) {
                                    strong += 1;
                                } else {
                                    i == STRONG.size( );
                                }
                            }

                            for ( int i = 0; i <= GREAT.size( ); i++ ) {
                                pixel = rgb( img, x + i, y );
                                auto [ r, g, b ] = GREAT[ i ];
                                if ( pixel.r == r && pixel.g == g && pixel.b == b ) {
                                    great += 1;
                                } else {
                                    i == GREAT.size( );
                                }
                            }

                            if ( normal == total || strong == total || great == total ) {
                            
                                Point point;
                                point.x = x + 15;
                                point.y = y + 15; 

                                arr.push_back( point );

                                x += total;

                            }
                        }                     
                    }
                }
            }

        } catch ( exception &e ) {
        }
        
        return arr;
    }

    Pixel rgb( Mat img, int x, int y ) {

        Vec3b rgb = img.at < Vec3b > ( y, x );
        Pixel pixel;
        pixel.r = rgb[ 2 ];
        pixel.g = rgb[ 1 ];
        pixel.b = rgb[ 0 ];

        return pixel;
    }

    vector < Pixel > inspect( Mat img ) {
        vector < Pixel > arr; 
        if ( !img.empty( ) ) {
            
            for ( int i = 0; i < img.rows; i++ ) {
                for ( int j = 0; j < img.cols; j++ ) {
                    cv::Vec3b rgb = img.at < cv::Vec3b > ( i, j );
                    Pixel pixel;
                    pixel.r = rgb[ 2 ];
                    pixel.g = rgb[ 1 ];
                    pixel.b = rgb[ 0 ];

                    arr.push_back( pixel );
                }
            }

        }
        return arr;
    }

    tuple < bool, vector < tuple < Point, bool > > > detect( Mat img ) {

        vector < tuple < Point, bool > > arr;
        int max = 30;
        bool target;

        try {

            if ( !img.empty( ) ) {

                for ( int y = 0; y <= img.rows; y++ ) {
                    for ( int x = 0; x <= img.cols; x++ ) {

                        int top = 0, bottom = 0, left = 0, right = 0;

                        if ( img.at < Vec3b > ( y, x ) == Vec3b( 0, 0, 0 ) ) {

                            while ( top < max ) top = img.at < Vec3b > ( y, x + top ) != Vec3b( 0, 0, 0 ) ? 100 : top + 1;

                            while ( top == max && bottom < max ) bottom = img.at < Vec3b > ( y + 3, x + max ) != Vec3b( 0, 0, 0 ) ? 100 : bottom + 1;
                        
                            while ( bottom == max && left < 3 ) left = img.at < Vec3b > ( y + left, x ) != Vec3b( 0, 0, 0 ) ? 100 : left + 1;

                            while ( left == 3 && right < 3 ) right = img.at < Vec3b > ( y + right, x + max ) != Vec3b( 0, 0, 0 ) ? 100 : right + 1;
                            
                            if ( top == max & bottom == max & left == 3 & right == 3 ) {
                                //img.at < Vec3b > ( y, x ) = Vec3b( 255, 0, 0 );

                                int targeting = 0;

                                for ( int t = 1; t <= 20; t++ ) {
                                    Pixel pixel = rgb( img, x + 15 - t, y + 28 - t );
                                    if ( pixel.r <= 233 && pixel.r >= 127 && pixel.g < 66 && pixel.b < 66 && pixel.b > 12 ) targeting++;
                                }

                                target = targeting > 3;

                                arr.push_back( make_tuple( Point( x, y ), target ) );
                                x += max;
                            }
                        }

                    }
                }
            }

        } catch ( exception &e ) {
        }

        //imwrite( "viewport.png", img ); 
        
        return make_tuple( target, arr );
    }

    void load() {
        Mat img;

        for (int i = 0; i <= 15; i++) {
            img = imread("/opt/afk/maps/" + to_string(i) + "-map.png");
            maps.push_back(img);
            img = imread("/opt/afk/maps/" + to_string(i) + "-path.png");

            vector<vector<int>> tmp;
            for (int x = 0; x < img.rows; x++) {
                vector<int> row;
                for (int y = 0; y < img.cols; y++) {
                    row.push_back(img.at<Vec3b>(x, y) == Vec3b(255, 0, 255) ? 0 : 1);
                }
                tmp.push_back(row);
            }
            paths.push_back(tmp);
        }
    }

    void walkable(vector<Waypoint> walkable) {
        for (Waypoint wp : walkable) paths[wp.z][wp.y][wp.x] = 0;
    }

    tuple<int, int, int, int> location(Mat FloorIndex, Mat Minimap, int x, int y) {
        int z = 0, valid = -1;

        try {
            for (int i = 0; i < FloorIndex.rows; i+=4) {
                if (FloorIndex.at<Vec3b>(i, 0) == Vec3b(38, 38, 37)) break;
                z++;
            }

            if (x == 0 && y == 0) {
                for (int i = 16; i < maps[z].rows - 16; i++) {
                    for (int j = 16; j < maps[z].cols - 16; j++) {
                        if (compare(j, i, z, Minimap)) {
                            x = j, y = i;
                        }
                    }
                }
            } else {
                auto [X, Y] = find(x, y, z, Minimap);
                x = X;
                y = Y;
            }

            for (int i = y - 1; i < y + 2; ++i) {
                for (int j = x - 1; j < x + 2; ++j) {
                    if (paths[z][i][j] == 1) valid++;
                }
            }

        } catch (exception& e) {
            console.log(e.what());
        }
        
        return make_tuple(x, y, z, valid);
    }

    tuple<int, int> find(int x, int y, int z, Mat minimap) {
        int oldX = x, oldY = y; 
        try {
            if (compare(x, y, z, minimap)) return make_tuple(x, y);

            int X, Y, grow = 0;

            while (grow < 150) {
                grow += 2;
                x--;
                y--;

                X = x;
                while (x < maps[z].rows && x < X + grow) {
                    if (compare(x, y, z, minimap)) return make_tuple(x, y);
                    x++;
                }

                Y = y;
                while (y < maps[z].cols && y < Y + grow) {
                    if (compare(x, y, z, minimap)) return make_tuple(x, y);
                    y++;
                }

                X = x;
                while (x >= 0 && x > X - grow) {
                    if (compare(x, y, z, minimap)) return make_tuple(x, y);
                    x--;
                }

                Y = y;
                while (y >= 0 && y > Y - grow) {
                    if (compare(x, y, z, minimap)) return make_tuple(x, y);
                    y--;
                }
            }
        
        } catch (exception &e) {
        }

        return make_tuple(oldX, oldY);
    }

    bool compare(int x, int y, int z, Mat minimap) {
        try {
            if (paths[z][y][x] == 0 && x >= 16 && x <= (2559 - 16) && y >= 16 && y <= (2559 - 16)) return false;

            Mat part = Mat(maps[z], Rect(x - 16, y - 16, 34, 34));

            int threshold = 0;

            for (int i = 0; i < minimap.rows - 1; i++) {
                for (int j = 0; j < minimap.cols - 1; j++) {
                    if (minimap.at<Vec3b>(i, j) == Vec3b(255, 255, 255)) continue;
                    if (threshold > 20) return false;
                    if (minimap.at<Vec3b>(i, j) != part.at<Vec3b>(i, j)) threshold++;
                }
            }

            return true;
        } catch (exception &e) {
        }

        return false;
    }

} procedure;
