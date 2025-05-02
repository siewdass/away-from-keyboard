#include "new/util.h"
#include <cmath>
#include "actions.h"
#include <iostream>
#include <random>
#include <atomic>

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
    
    if ( timer.clock( ) > potionCooldown ) {
        if ( health <= 135 ) {
            potionCooldown = timer.clock( ) + 1050;
            return make_tuple( true, "keyboard", "2", "", "Health Potion", 0, 0, 0 );
        } else if ( mana <= 220 ) {
            potionCooldown = timer.clock( ) + 1050;
            return make_tuple( true, "keyboard", "3", "", "Mana Potion", 0, 0, 0 );
        }
    }
    return make_tuple( false, "", "", "", "", 0, 0, 0 );
}

tuple < bool, string, string, string, string, int, int, int > Actions::supporter( Player player ) {
    
    if ( timer.clock( ) > healCooldown ) {
        if ( timer.clock( ) > healCooldown1 ) {
            if ( player.health <= 236 ) {
                if ( player.mana >= 10 ) {
                    healCooldown = timer.clock( ) + 1050;
                    healCooldown1 = timer.clock( ) + 63000;
                    return make_tuple( true, "keyboard", "F1", "", "Utura Gran", 0, 0, 0 );
                }
            }
        } else if ( timer.clock( ) > healCooldown2 ) {
            if ( player.health <= 170 || player.paralyze ) {
                if ( player.mana >= 10 ) {
                    healCooldown = timer.clock( ) + 1050;
                    healCooldown2 = timer.clock( ) + 1050;
                    return make_tuple( true, "keyboard", "4", "", "Exura Mead Ico", 0, 0, 0 );
                }
            }
        } else if ( timer.clock( ) > healCooldown3 ) {
            if ( player.health <= 53 ) {
                if ( player.mana >= 10 ) {
                    healCooldown = timer.clock( ) + 1050;
                    healCooldown3 = timer.clock( ) + 630000;
                    return make_tuple( true, "keyboard", "9", "", "Exura Gran Ico", 0, 0, 0 );
                }
            }
        }
    }

    return make_tuple( false, "", "", "", "", 0, 0, 0 );
}

bool Actions::attacker( Player player ) {

    if ( player.box == 0 ) count++;
    else count = 0;
    //count = player.box == 0 ? 0 : count++;

    return count == 5 ? true : false;
}

vector < tuple < int, int > > Actions::looter( ) {

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
        auto [ x, y ] = Actions::gameWindow( point.x, point.y );
        All.push_back( { x, y } );
    }

    return All;
}

tuple<bool, int, int> Actions::walker( Player player, Character character, vector < vector < vector < int > > > paths, atomic<int>& position ) {

    int X = 0, Y = 0;

    if ( abs( player.x - character.waypoints[ position ].x ) <= 1 && abs( player.y - character.waypoints[ position ].y ) <= 1 ) {
        console.log( "action", character.waypoints[ position ].action );
        bool attack = character.waypoints[ position ].action == "attack" ? true : false;
        position += 1;
        if ( attack && player.monsters.size( ) > 0 ) return make_tuple( true, X, Y );
    }

    if ( position == character.waypoints.size( ) ) position = 0;

    auto ways = path.find(
        paths[ player.z ],
        Point( player.x, player.y ),
        Point( character.waypoints[ position ].x, character.waypoints[ position ].y )
    );

    if ( ways.size( ) > 0 ) {
        x = ways[ ways.size( ) >= 57 ? 56 : ways.size( ) - 1 ].x;
        y = ways[ ways.size( ) >= 57 ? 56 : ways.size( ) - 1 ].y;

        if ( character.waypoints[ position ].action == "walk" ) {
            x += dis( gen );
            y += dis( gen );
        }

        //console.log( "from x:", player.x, "from y:", player.y, "to x:", x, "to y:", y );
        //inputs.mouse( left + ( pixel * ( x - player.x ) ), top + ( pixel * ( y - player.y ) ), 1 );

        X = left + ( pixel * ( x - player.x ) );
        Y = top + ( pixel * ( y - player.y ) );
    }

    return make_tuple( false, X, Y );
}

void Actions::run( Player player, Point to, vector < vector < vector < int > > > paths ) {
   
    auto ways = path.find( paths[ player.z ], Point( player.x, player.y ), to );
    if ( ways.size( ) > 0 ) {

        x = ways[ ways.size( ) >= 3 ? 3 : ways.size( ) - 1 ].x;
        y = ways[ ways.size( ) >= 3 ? 3 : ways.size( ) - 1 ].y;

        console.log( "from x:", player.x, "from y:", player.y, "to x:", x, "to y:", y );

        //inputs.keyboard( "Escape" );
        timer.sleep( 100 );
        //inputs.mouse( left + ( pixel * ( x - player.x ) ), top + ( pixel * ( y - player.y ) ), 1 );
    }

}

bool Actions::lootway( Player player, vector < vector < vector < int > > > map, Point lootpoint ) {
    //REMOVE
    int position = 0;
    if ( player.minimap == old ) {
        count += 1;
    } else if ( player.minimap != old  ) {
        count = 0;
        old = player.minimap;
    } if ( count >= 3 ) {
        count = 0;
        if ( timer.clock( ) > walkCoolDown ) {

            if ( player.x == lootpoint.x &&
                 player.y == lootpoint.y ) {
                console.log( "loot" );
                return true;
            }

            from.x = player.x;
            from.y = player.y;
            to.x = lootpoint.x;
            to.y = lootpoint.y;
        try {
            auto paths = path.find( map[ player.z ], from, to );
            console.log( position, runPosition );

            if ( paths.size( ) > 0 ) {

                if ( paths.size( ) >= 41 ) {
                    x = paths[ 40 ].x;
                    y = paths[ 40 ].y;
                } else {
                    x = paths[ paths.size( ) -1 ].x;
                    y = paths[ paths.size( ) -1 ].y;
                }

                console.log( "from x:", player.x, "from y:", player.y );
                console.log( "to x:", x, "to y:", y );

                int X = x - player.x; 
                int Y = y - player.y; 

                //inputs.keyboard( "Escape" );
                //time.sleep( 100 );
                //inputs.mouse( left + ( pixel * X ), top + ( pixel * Y ), 1 );
                console.log( left + ( pixel * X ), top + ( pixel * Y ), 1 );
                walkCoolDown =  timer.clock( ) + 1000;
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

vector<tuple<int, int>> Actions::throwFlask( Player player ) {
    vector<tuple<int, int>> flasks;
    for ( int i = 0; i < player.flasks.size( ); i++ ) {
        Point flask = player.flasks[ player.flasks.size( ) -1 ];
        flasks.push_back( { 836 + flask.x, 290 + flask.y } );
    }
    return flasks;
}