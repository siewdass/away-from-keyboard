#include "inputs.h"
#include "status.h"
#include "viewport.h"
#include "actions.h"
#include "path.h"
#include "script.h"
#include "utils.h"
#include <thread>

Client client;
Character character;
Status status;
Viewport viewport;
Time time;
Actions actions;
Mode mode;
Path path;
Script script;
Inputs inputs;

bool running = true;
vector < tuple < string, string, string, string, int, int, int > > QUEUE;

void updateThread( ) {

	script.load( "maps/amazon.json" );

	while ( running ) {

		auto [ box, monsters, visibleMonsters, side, isTargeting ] = viewport.check( );
		character.box = box;
		character.monsters = monsters;
		character.visibleMonsters = visibleMonsters;
		character.side = side;
		character.isTargeting = isTargeting;

		auto [ health, mana, hungry, paralyze, poison, haste, x, y, z, minimap, flasks ] = status.update( script.positions );
		character.health = health;
		character.mana = mana;
		character.hungry = hungry;
		character.paralyze = paralyze;
		character.poison = poison;
		character.haste = haste;
		character.minimap = minimap;
		character.flasks = flasks;
		character.x = x;
		character.y = y;
		character.z = z;

		console.log( "x:", character.x, "y:", character.y );
		console.log( "targeting:", character.isTargeting );
		console.log( "flask:", flasks.size( ) );

		time.sleep( 200 );
	}
}

void alternatorThread( ) {
	int lastX = 0, lastY = 0;
	time.sleep( 4000 );

   	while ( running ) {
		
			if ( mode.walking ) {
				mode.attacking = actions.walker( character, script );
				if ( !mode.attacking && !character.haste ) {
					inputs.keyboard( "b" );
					time.sleep( 1000 );
				}
				if ( mode.attacking && character.visibleMonsters ) {
					mode.walking = false;
				}
				if ( !mode.attacking && character.x == lastX && character.y == lastY && character.box >= 4 ) {
					console.log( "TTTTTTTTRRRRRRRRRRAAAAAAAAAPPPPPPPP" );
					mode.attacking = true;
					mode.walking = false;
				} else {
					lastX = character.x;
					lastY = character.y;
				}
			} else if ( mode.attacking ) {
				auto [ res, type, key, modifier, spell, x, y, button ] = actions.attacker( character );
				if ( type != "" ) {
					QUEUE.push_back( make_tuple( type, key, modifier, spell, x, y, button ) );
				}
				mode.looting = res;
				if ( mode.looting ) {
					mode.attacking = false;
				}
			} else if ( mode.looting ) {
				mode.walking = actions.looter( );
				if ( mode.walking ) {
					mode.looting = false;
				}
			}

    	time.sleep( 200 );
	}
}

void healThread( ) {

	double global = 1050;
	double potionCooldown = 0,
		healCooldown = 0,
		healCooldown1 = 0,
		healCooldown2 = 0,
		healCooldown3 = 0;

	time.sleep( 1000 );

	while ( running ) {

		try {
			if ( time.clock( ) > potionCooldown ) {
				if ( character.mana <= 10 ) {
					inputs.keyboard( "F2" );
					potionCooldown = time.clock( ) + global;
				}
			}
			if ( time.clock( ) > healCooldown && character.mana >= 10 ) {
				if ( time.clock( ) > healCooldown1 && character.health <= 200 ) {
					inputs.keyboard( "F1" );
					healCooldown = time.clock( ) + global;
					healCooldown1 = time.clock( ) + global;
				}
			}
		} catch ( const std::exception& e ) {
			console.log( "ERROR:", e.what( ) );
		}

		time.sleep( 500 );
	}
}

void toolsThread( ) {

	double eat = 0, ring = 0, f = 0;
	time.sleep( 3000 );

	while ( running ) {
		
		try {
			if ( mode.walking ) {
				if ( character.hungry ) {
					inputs.keyboard( "F5" );
					inputs.keyboard( "F5" );
					inputs.keyboard( "F5" );
				} else if ( time.clock( ) > f && character.flasks.size( ) > 0 ) {
					if ( character.flasks.size( ) == 1 ) {
						f = time.clock( ) + 180000;
					}
					Point flask = character.flasks[ 0 ];
					inputs.mouseDrag( 623 + flask.x, 290 + flask.y );
					console.log( "test" );
				} else if ( time.clock( ) > ring ) {
					inputs.keyboard( "F4" );
					inputs.keyboard( "F4" );
					inputs.keyboard( "F4" );
					ring = time.clock( ) + 180000;
				}
			}
		} catch ( const std::exception& e ) {
			console.log( "ERROR:", e.what( ) );
		}
		
		time.sleep( 5000 );
	}
}

void autoattackThread( ) {

	bool keepAttacking;
	double autoAttackCooldown = 0;

	time.sleep( 3000 );

	while ( running ) {

		if ( mode.walking ) {
			if ( character.monsters.size( ) == 0 ) {
				mode.attacking = actions.walker( character, script );
				if ( mode.attacking ) {
					mode.walking = false;
				}
			} else {
				mode.attacking = true;
				mode.walking = false;
			}
		} else if ( mode.attacking ) {
			if ( character.monsters.size( ) > 0 ) {
				if ( time.clock( ) > autoAttackCooldown && !character.isTargeting ) {
					inputs.keyboard( "space" );
					autoAttackCooldown = time.clock( ) + 1000;
					mode.looting = true;
					mode.attacking = false;
				}
			} else {
				mode.walking = true;
				mode.attacking = false;
			}
		} else if ( mode.looting ) {
			if ( !character.isTargeting ) {
				mode.walking = actions.looter( );
				if ( mode.walking && character.monsters.size( ) > 0 ) {
					mode.looting = false;
					mode.attacking = true;
					mode.walking = false;
				} else if ( mode.walking && character.monsters.size( ) == 0 ) {
					mode.looting = false;
					mode.attacking = false;
					mode.walking = true;
				}
			}
		}
		
		time.sleep( 500 );
		
	}
}

void attackThread( ) {

	bool keepAttacking;
	double global = 2100;
	double attackCooldown = 0,
		attackCooldown1 = 0,
		attackCooldown2 = 0,
		attackCooldown3 = 0,
		attackCooldown4 = 0,
		attackCooldown5 = 0,
		attackCooldown6 = 0,
		attackCooldown7 = 0;

	time.sleep( 3000 );

	while ( running ) {

		try {
			if ( mode.attacking ) {

				if ( character.mana > 50 ) {
					if ( character.box >= 3 || keepAttacking ) {
						keepAttacking = true;
						if ( time.clock( ) > attackCooldown ) {
							if ( time.clock( ) > attackCooldown1 && character.isTargeting && character.box == 1 ) {
								inputs.keyboard( "0" );
								attackCooldown = time.clock( ) + global*2;
								attackCooldown1 = time.clock( ) + 31500;
							} else if ( time.clock( ) > attackCooldown2 && character.isTargeting && character.box == 1 ) {
								inputs.keyboard( "F2" );
								attackCooldown = time.clock( ) + global;
								attackCooldown2 = time.clock( ) + 6300;
							} else if ( time.clock( ) > attackCooldown3 && character.isTargeting && character.box == 1 ) {
								inputs.keyboard( "F3" );
								attackCooldown = time.clock( ) + global;
								attackCooldown3 = time.clock( ) + 6300;
							} else if ( time.clock( ) > attackCooldown4 && character.box <= 3 && character.monsters.size( ) >= 5 ) {
								inputs.keyboard( "8" );
								attackCooldown = time.clock( ) + global;
								attackCooldown4 = time.clock( ) + 8400;
							} else if ( time.clock( ) > attackCooldown5 && character.box >= 2 ) {
								inputs.keyboard( "5" );
								attackCooldown = time.clock( ) + global;
								attackCooldown5 = time.clock( ) + 4200;
							} else if ( time.clock( ) > attackCooldown6 && character.box >= 2 ) {
								inputs.keyboard( "6" );
								attackCooldown = time.clock( ) + global;
								attackCooldown6 = time.clock( ) + 6300;
							} else if ( time.clock( ) > attackCooldown7 && character.box >= 2 ) {
								string modifier = "shift+";
								modifier.append( character.side );
								inputs.keywith( modifier.c_str( ) );
								time.sleep( 100 );
								inputs.keyboard( "7" );
								attackCooldown = time.clock( ) + global;
								attackCooldown7 = time.clock( ) + 6300;
							}
						}
					}
				}
				time.sleep( 100 );
			} else {
				keepAttacking = false;
				time.sleep( 1000 );
			}
		} catch ( const std::exception& e ) {
			console.log( "ERROR:", e.what( ) );
		}
	}
}

int main( ) {

	// cambiar Point por un nuevo struct y 
	// detectar cantidad de vida segun pixeles
	// internos de la barra de vida
	// autoattack siempre al que tenga mas vida
	// si queda trap que mate
	// if ( !x ) { return } 
	// exori min = minimo 2

	int id = client.get( );
	status.id = id;
	viewport.id = id;

	if ( id != 0 ) {
		thread t1 ( updateThread );
		//thread t2 ( alternatorThread );
		thread t3 ( healThread );
		//thread t4 ( attackThread );
		//thread t5 ( toolsThread );
		thread t6 ( autoattackThread );
		t1.join( );
		//t2.join( );
		t3.join( );
		//t4.join( );
		//t5.join( );
		t6.join( );
	}

	return 0;
}
