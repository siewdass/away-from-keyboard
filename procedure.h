#pragma once
#include "util.h"
#include "shared.h"
#include "character.h"
#include "acknow.h"
#include "pathfinder.h"

Acknow acknow;
Pathfinder pathfinder;

class {
public:

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

        vector<Pixel> monsters = acknow.inspect( Monsters );

        bool target = false;

        for ( Pixel pixel : monsters ) {
            if ( pixel.r == 255 && pixel.g == 0 && pixel.b == 0 ) {
                target = true;
                break;   
            }
        }

        vector<Pixel> capacity = acknow.inspect( CAPACITY );

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

        vector<Pixel> weapon = acknow.inspect( WEAPON );

        for ( Pixel pixel : weapon ) {
            int r = pixel.r, g = pixel.g, b = pixel.b;
            if ( r == 94 && g == 96 && b == 98 ) {
                weaponEquipped = true;
                break;
            }
        }

        bool amuletEquipped = true;

        vector<Pixel> amulet = acknow.inspect( AMULET );

        for ( Pixel pixel : amulet ) {
            int r = pixel.r, g = pixel.g, b = pixel.b;
            if ( r == 71 && g == 73 && b == 76 ) {
                amuletEquipped = false;
                break;
            }
        }

        bool ringEquipped = true;

        vector<Pixel> ring = acknow.inspect( RING );

        for ( Pixel pixel : ring ) {
            int r = pixel.r, g = pixel.g, b = pixel.b;
            if ( r == 88 && g == 91 && b == 94 ) {
                ringEquipped = false;
                break;
            }
        }

        bool near = false;
        bool alarm = false;

        vector<Pixel> battle = acknow.inspect( BATTLE );

        for ( Pixel pixel : battle ) {
            int r = pixel.r, g = pixel.g, b = pixel.b;
            if ( r == 0 && g == 0 && b == 0 ) {   
                //std::cout << r << " " << g << " " << b << std::endl;         
                near = true;
            }
        }

        if ( AlarmCount >= 4 ) { AlarmCount = 0; alarm = true; }

        vector<Pixel> chat = acknow.inspect( CHAT );

        for ( Pixel pixel : chat ) {
            int r = pixel.r, g = pixel.g, b = pixel.b;
            if ( r == 96 && g == 248 && b == 248 ) {
                alarm = true;
                break;
            }
        }

        vector<Pixel> HealthPoints = acknow.inspect( HP );
        int health = (1012 / 2)-188-40;

        for ( Pixel pixel : HealthPoints ) {
            int r = pixel.r, g = pixel.g, b = pixel.b;
            if ( r >= 32 && r <= 46 && g >= 32 && g <= 46 && b >= 32 && b <= 46 ) {
                health -= 1;
            }
        }

        vector<Pixel> ManaPoints = acknow.inspect( MP );
        int mana = 0;

        for ( Pixel pixel : ManaPoints ) {
            int r = pixel.r, g = pixel.g, b = pixel.b;
            if ( r == 0 & g == 41 & b == 91 ) {
                mana += 1;
            } else if ( r == 0 & g == 44 & b == 91 ) {
                mana += 1;
            }
        }

        vector<Pixel> BenefitsAndDamages = acknow.inspect( BD );
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

        vector < Point > Flask = acknow.flask( FK );

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
        
        auto [ old, GameWindow ] = acknow.detect( GW );
        
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

} procedure;
