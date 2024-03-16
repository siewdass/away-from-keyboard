#include <cmath>
#include "actions.h"
#include <iostream>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(-1, 1);

tuple < int, int > Actions::gameWindow( int x, int y ) {
    int sqm = 41;
    int width = sqm * 15, height = sqm * 11;
    int left = 210, top = 55;
    int X = x * sqm + ( width / 2 ) + left - ( sqm / 2 );
    int Y = y * sqm + ( height / 2 ) + top - ( sqm / 2 );
    return make_tuple( X, Y );
}

tuple < int, int > Actions::mapWindow( int x, int y ) {
    int left = 898, top = 59;
    int X = left + x;
    int Y = top + y;
    return make_tuple( X, Y );
}

tuple < bool, string, string, string, string, int, int, int > Actions::potions( int health, int mana ) {
    
    if ( time.clock( ) > potionCooldown ) {
        if ( health <= 135 ) {
            potionCooldown = time.clock( ) + 1050;
            return make_tuple( true, "keyboard", "2", "", "Health Potion", 0, 0, 0 );
        } else if ( mana <= 220 ) {
            potionCooldown = time.clock( ) + 1050;
            return make_tuple( true, "keyboard", "3", "", "Mana Potion", 0, 0, 0 );
        }
    }
    return make_tuple( false, "", "", "", "", 0, 0, 0 );
}

tuple < bool, string, string, string, string, int, int, int > Actions::supporter( Values values ) {
    
    if ( time.clock( ) > healCooldown ) {
        if ( time.clock( ) > healCooldown1 ) {
            if ( values.health <= 236 ) {
                if ( values.mana >= 10 ) {
                    healCooldown = time.clock( ) + 1050;
                    healCooldown1 = time.clock( ) + 63000;
                    return make_tuple( true, "keyboard", "F1", "", "Utura Gran", 0, 0, 0 );
                }
            }
        } else if ( time.clock( ) > healCooldown2 ) {
            if ( values.health <= 170 || values.paralyze ) {
                if ( values.mana >= 10 ) {
                    healCooldown = time.clock( ) + 1050;
                    healCooldown2 = time.clock( ) + 1050;
                    return make_tuple( true, "keyboard", "4", "", "Exura Mead Ico", 0, 0, 0 );
                }
            }
        } else if ( time.clock( ) > healCooldown3 ) {
            if ( values.health <= 53 ) {
                if ( values.mana >= 10 ) {
                    healCooldown = time.clock( ) + 1050;
                    healCooldown3 = time.clock( ) + 630000;
                    return make_tuple( true, "keyboard", "9", "", "Exura Gran Ico", 0, 0, 0 );
                }
            }
        }
    }

    return make_tuple( false, "", "", "", "", 0, 0, 0 );
}

bool Actions::attacker( Values values ) {

    if ( values.box == 0 ) count++;
    else count = 0;
    //count = values.box == 0 ? 0 : count++;

    return count == 5 ? true : false;
}

bool Actions::looter( ) {

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

    inputs.shiftDown( );
    for ( Point point : Around ) {

        auto [ x, y ] = Actions::gameWindow( point.x, point.y );
        inputs.mouse( x, y, 3 );
        time.sleep( 130 );

    }
    inputs.shiftUp( );

    return true;
}

bool Actions::walker( Values values, vector < Waypoint > waypoints, vector < vector < vector < int > > > paths ) {

    if ( abs( values.x - waypoints[ position ].x ) <= 1 && abs( values.y - waypoints[ position ].y ) <= 1 ) {
        console.log( "action", waypoints[ position ].action );
        bool attack = waypoints[ position ].action == "attack" ? true : false;
        position += 1;
        if ( attack && values.monsters.size( ) > 0 ) return true;
    }

    if ( position == waypoints.size( ) ) position = 0;

    auto ways = path.find(
        paths[ values.z ],
        Point( values.x, values.y ),
        Point( waypoints[ position ].x, waypoints[ position ].y )
    );

    if ( ways.size( ) > 0 ) {
        x = ways[ ways.size( ) >= 57 ? 56 : ways.size( ) - 1 ].x;
        y = ways[ ways.size( ) >= 57 ? 56 : ways.size( ) - 1 ].y;

        if ( waypoints[ position ].action == "walk" ) {
            x += dis( gen );
            y += dis( gen );
        }

        console.log( "from x:", values.x, "from y:", values.y, "to x:", x, "to y:", y );

        inputs.keyboard( "Escape" );
        time.sleep( 200 );
        inputs.mouse( left + ( pixel * ( x - values.x ) ), top + ( pixel * ( y - values.y ) ), 1 );
    }

    return false;
}

void Actions::run( Values values, Point to, vector < vector < vector < int > > > paths ) {
   
    auto ways = path.find( paths[ values.z ], Point( values.x, values.y ), to );
    if ( ways.size( ) > 0 ) {

        x = ways[ ways.size( ) >= 3 ? 3 : ways.size( ) - 1 ].x;
        y = ways[ ways.size( ) >= 3 ? 3 : ways.size( ) - 1 ].y;

        console.log( "from x:", values.x, "from y:", values.y, "to x:", x, "to y:", y );

        inputs.keyboard( "Escape" );
        time.sleep( 100 );
        inputs.mouse( left + ( pixel * ( x - values.x ) ), top + ( pixel * ( y - values.y ) ), 1 );
    }

}

bool Actions::lootway( Values values, vector < vector < vector < int > > > map, Point lootpoint ) {
    console.log( "ejecutado" );
    if ( values.minimap == old ) {
        count += 1;
    } else if ( values.minimap != old  ) {
        count = 0;
        old = values.minimap;
    } if ( count >= 3 ) {
        count = 0;
        if ( time.clock( ) > walkCoolDown ) {

            if ( values.x == lootpoint.x &&
                 values.y == lootpoint.y ) {
                console.log( "loot" );
                return true;
            }

            from.x = values.x;
            from.y = values.y;
            to.x = lootpoint.x;
            to.y = lootpoint.y;
            try {
            auto paths = path.find( map[ values.z ], from, to );
            console.log( position, runPosition );

            if ( paths.size( ) > 0 ) {

                if ( paths.size( ) >= 41 ) {
                    x = paths[ 40 ].x;
                    y = paths[ 40 ].y;
                } else {
                    x = paths[ paths.size( ) -1 ].x;
                    y = paths[ paths.size( ) -1 ].y;
                }

                console.log( "from x:", values.x, "from y:", values.y );
                console.log( "to x:", x, "to y:", y );

                int X = x - values.x; 
                int Y = y - values.y; 

                //inputs.keyboard( "Escape" );
                //time.sleep( 100 );
                inputs.mouse( left + ( pixel * X ), top + ( pixel * Y ), 1 );
                console.log( left + ( pixel * X ), top + ( pixel * Y ), 1 );
                walkCoolDown =  time.clock( ) + 1000;
                return false;
            }
            else {
                console.log( "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
                return true; 
            }

            } catch ( const std::exception& err ) {
                return true;
            }
        }
    }
    return false;
}

void Actions::adjustMap( ) {
}

void Actions::throwFlask( Values values ) {
    for ( int i = 0; i < values.flasks.size( ); i++ ) {
        Point flask = values.flasks[ values.flasks.size( ) -1 ];
        inputs.mouseDrag( 836 + flask.x, 290 + flask.y );
    }
}