#include "actions.h"
#include <iostream>

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

    if ( values.monsters.size( ) >= 2 || values.box >= 1 ) {

        /*if ( time.clock( ) > autoAttackCooldown ) {
            if ( values.box >= 1 && !values.isTargeting ) {
                for ( Point point : values.monsters ) {
                    if ( point.x >= -1 & point.x <= 1 & point.y >= -1 & point.y <= 1 ) {
                        auto [ x, y ] = Actions::gameWindow( point.x, point.y );
                        inputs.mouse( x, y, 3 );
                        autoAttackCooldown = time.clock( ) + 2000;
                        break;
                        //return make_tuple( false, "mouse", "", x, y, 3 );
                    }
                }
            }
        }

        if ( time.clock( ) > supportCooldown ) {
            if ( values.mana > 10 ) {
                if ( keepAttacking ) {
                    supportCooldown = time.clock( ) + 5000;
                    return make_tuple( false, "keyboard", "1", "", "Exori", 0, 0, 0 );
                }
            }
        }

        if ( time.clock( ) > attackCooldown ) {
            if ( values.mana > 50 ) {
                if ( values.box >= 3 || keepAttacking ) {
                    count = 0;
                    single = true;
                    keepAttacking = true;
                    if ( time.clock( ) > attackCooldown1 && values.isTargeting && values.box == 1 ) {
                        attackCooldown = time.clock( ) + 2100;
                        attackCooldown1 = time.clock( ) + 33000;
                        return make_tuple( false, "keyboard", "0", "", "Exori Gran Ico", 0, 0, 0 );
                    } else if ( time.clock( ) > attackCooldown2 && values.isTargeting && values.box == 1 ) {
                        attackCooldown = time.clock( ) + 2100;
                        attackCooldown2 = time.clock( ) + 6300;
                        return make_tuple( false, "keyboard", "F2", "", "Exori Ico", 0, 0, 0 );
                    } else if ( time.clock( ) > attackCooldown3 && values.isTargeting && values.box == 1 ) {
                        attackCooldown = time.clock( ) + 2100;
                        attackCooldown3 = time.clock( ) + 6300;
                        return make_tuple( false, "keyboard", "F3", "", "Exori Hur", 0, 0, 0 );
                    } else if ( time.clock( ) > attackCooldown4 && values.box <= 3 && values.monsters.size( ) >= 5 ) {
                        attackCooldown = time.clock( ) + 2100;
                        attackCooldown4 = time.clock( ) + 8400;
                        return make_tuple( false, "keyboard", "8", "", "Exori Mas", 0, 0, 0 );
                    } else if ( time.clock( ) > attackCooldown5 ) {
                        attackCooldown = time.clock( ) + 2100;
                        attackCooldown5 = time.clock( ) + 4200;
                        return make_tuple( false, "keyboard", "5", "", "Exori", 0, 0, 0 );
                    } else if ( time.clock( ) > attackCooldown6 ) {
                        attackCooldown = time.clock( ) + 2100;
                        attackCooldown6 = time.clock( ) + 6300;
                        return make_tuple( false, "keyboard", "6", "", "Exori Gran", 0, 0, 0 );
                    } else if ( time.clock( ) > attackCooldown7 ) {
                        attackCooldown = time.clock( ) + 2100;
                        attackCooldown7 = time.clock( ) + 6300;
                        return make_tuple( false, "keyboard", "7", values.side, "Exori Min", 0, 0, 0 );
                    }
                }
            }
        }*/
    } else {
        if ( count == 3 ) {
            keepAttacking = false;
            single = false;
            return true;
        } else {
            count += 1;
        }
    }

    return false;
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
        time.sleep( 100 );

    }
    inputs.shiftUp( );

    return true;
}

bool Actions::walker( Values values, vector < Waypoint > waypoints, vector < vector < vector < int > > > paths ) {

    if ( values.minimap == old ) {
        count += 1;
    } else if ( values.minimap != old  ) {
        count = 0;
        old = values.minimap;
    } if ( count >= 4 ) {
        runPosition = position-1;
        count = 0;
        if ( time.clock( ) > walkCoolDown ) {

            if ( values.x == waypoints[ position ].x &&
                 values.y == waypoints[ position ].y && 
                 waypoints[ position ].action == "attack" ) {
                console.log( "action" );
                position += 1;
                return true;
            } else if ( ( values.x == waypoints[ position ].x -1 || 
                          values.x == waypoints[ position ].x +1 ) && 
                        ( values.y == waypoints[ position ].y -1 || 
                          values.y == waypoints[ position ].y +1 ||
                          values.y == waypoints[ position ].y ) && 
                          waypoints[ position ].action == "attack" ) {
                console.log( "action wrong x" );
                position += 1;
                return true;
            } else if ( ( values.y == waypoints[ position ].y -1 || 
                          values.y == waypoints[ position ].y +1 ) && 
                        ( values.x == waypoints[ position ].x -1 || 
                          values.x == waypoints[ position ].x +1 ||
                          values.x == waypoints[ position ].x ) && 
                          waypoints[ position ].action == "attack" ) {
                console.log( "action wrong y" );
                position += 1;
                return true;
            }  else if ( values.x == waypoints[ position ].x &&
                 values.y == waypoints[ position ].y && 
                 waypoints[ position ].action == "walk" ) {
                console.log( "walk" );
                position += 1;
            } else if ( ( values.x == waypoints[ position ].x -1 || 
                          values.x == waypoints[ position ].x +1 ) && 
                        ( values.y == waypoints[ position ].y -1 || 
                          values.y == waypoints[ position ].y +1 ||
                          values.y == waypoints[ position ].y ) && 
                          waypoints[ position ].action == "walk" ) {
                console.log( "walk wrong x" );
                position += 1;
            } else if ( ( values.y == waypoints[ position ].y -1 || 
                          values.y == waypoints[ position ].y +1 ) && 
                        ( values.x == waypoints[ position ].x -1 || 
                          values.x == waypoints[ position ].x +1 ||
                          values.x == waypoints[ position ].x ) && 
                          waypoints[ position ].action == "walk" ) {
                console.log( "walk wrong y" );
                position += 1;
            } if ( position == waypoints.size( ) ) {
                console.log( "reset positions" );
                position = 0;
            }

            from.x = values.x;
            from.y = values.y;
            to.x = waypoints[ position ].x;
            to.y = waypoints[ position ].y;

            /*vector < vector < int > > matrix;
            for ( int i = ( values.x - 100 ); i > ( values.x + 100 ); i++ ) {
                vector < int > row;
                for ( int j = ( values.y - 100 ); j < ( values.y + 100 ); j++ ) {
                    row.push_back( paths[ values.z ][ i ][ j ] );
                }
                matrix.push_back( row );
            }

            for ( vector < int > row : matrix ) { for ( int e : row ) { std::cout << e << " "; } std::cout << endl; }*/

            auto way = path.find( paths[ values.z ], from, to );

            if ( way.size( ) > 0 ) {
                x = way[ way.size( ) >= 56 ? 56 : way.size( ) - 1 ].x;
                y = way[ way.size( ) >= 56 ? 56 : way.size( ) - 1 ].y;//56

                int X = x - values.x; 
                int Y = y - values.y; 

                console.log( "from x:", values.x, "from y:", values.y, "to x:", x, "to y:", y );
                console.log( left + ( pixel * X ), top + ( pixel * Y ) );

                //inputs.keyboard( "Escape" );
                //time.sleep( 100 );
                inputs.mouse( left + ( pixel * X ), top + ( pixel * Y ), 1 );

                walkCoolDown =  time.clock( ) + 1000;
                return false;
            }
        }
    }
    return false;
}

bool Actions::runaway( Values values, vector < Waypoint > waypoints, vector < vector < vector < int > > > map ) {
   
    if ( values.minimap == old ) {
        count += 1;
    } else if ( values.minimap != old  ) {
        count = 0;
        old = values.minimap;
    } if ( count >= 2 ) {
        count = 0;
        if ( time.clock( ) > walkCoolDown ) {
            if ( values.x == waypoints[ runPosition ].x &&
                 values.y == waypoints[ runPosition ].y ) {
                console.log( "run" );
                if ( runPosition == 0 ) {
                    runPosition = 1;
                } else if ( runPosition == position - 1) { 
                    runPosition -= 1;
                } else if ( runPosition == position - 2 ) {
                    runPosition += 1;
                }
            } else if ( ( values.x == waypoints[ runPosition ].x -1 || 
                          values.x == waypoints[ runPosition ].x +1 ) && 
                        ( values.y == waypoints[ runPosition ].y -1 || 
                          values.y == waypoints[ runPosition ].y +1 ||
                          values.y == waypoints[ runPosition ].y )  ) {
                console.log( "run wrong x" );
                if ( runPosition == 0 ) {
                    runPosition = 1;
                } else if ( runPosition == position - 1) { 
                    runPosition -= 1;
                } else if ( runPosition == position - 2 ) {
                    runPosition += 1;
                }
            } else if ( ( values.y == waypoints[ runPosition ].y -1 || 
                          values.y == waypoints[ runPosition ].y +1 ) && 
                        ( values.x == waypoints[ runPosition ].x -1 || 
                          values.x == waypoints[ runPosition ].x +1 ||
                          values.x == waypoints[ runPosition ].x ) ) {
                console.log( "run wrong y" );
                if ( runPosition == 0 ) {
                    runPosition = 1;
                } else if ( runPosition == position - 1) { 
                    runPosition -= 1;
                } else if ( runPosition == position - 2 ) {
                    runPosition += 1;
                }
            }

            if ( runPosition < 0 ) {
                runPosition = 0;
            }
            from.x = values.x;
            from.y = values.y;
            to.x = waypoints[ runPosition ].x;
            to.y = waypoints[ runPosition ].y;
            console.log( "walk" );

            auto paths = path.find( map[ z ], from, to );
            if ( paths.size( ) > 0 ) {

                if ( paths.size( ) >= 1 ) {
                    x = paths[ 0 ].x;
                    y = paths[ 0 ].y;
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
        }
    }
    return false;
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
            auto paths = path.find( map[ z ], from, to );
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