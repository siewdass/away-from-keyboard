#include "inputs.h"
#include "status.h"
#include "viewport.h"
#include "actions.h"
#include "path.h"
#include "script.h"
#include "utils.h"
#include "character.h"
#include "licence.h"
#include "gui.h"

#include <thread>

Client client;
Values values;
Status status;
Viewport viewport;
Time time;
Actions actions;
Mode mode;
Path path;
Script script;
Inputs inputs;
Character character;
Licence licence;

string id;
string user;

void updateThread( ) {

	while ( true ) {

		if ( Pause ) {

			auto [
				box,
				monsters,
				visibleMonsters,
				side,
				isTargeting,
				targetPosition,
				boxTarget
			] = viewport.check( );
			
			auto [
				health,
				mana,
				hungry,
				paralyze,
				poison,
				haste,
				x, y, z,
				minimap,
				flasks,
				alarm,
				weapon,
				necklace,
				ring
			] = status.update( script.positions );

			values.box = box;
			values.monsters = monsters;
			values.visibleMonsters = visibleMonsters;
			values.side = side;
			values.isTargeting = isTargeting;
			values.targetPosition = targetPosition;
			values.health = health;
			values.mana = mana;
			values.hungry = hungry;
			values.paralyze = paralyze;
			values.poison = poison;
			values.haste = haste;
			values.minimap = minimap;
			values.flasks = flasks;
			values.x = x;
			values.y = y;
			values.z = z;
			values.boxTarget = boxTarget;
			values.alarm = alarm;
			values.weapon = weapon;
			values.necklace = necklace;
			values.ring = ring;

			//console.log( x, y );

			time.sleep( 200 );

		} else {
  		time.sleep( 2000 );
		}

	}
}

void healThread( ) {

	double global = 1050;
	double potionCooldown = 0,
		healCooldown = 0,
		healCooldown1 = 0,
		healCooldown2 = 0,
		healCooldown3 = 0,
		recoveryCooldown = 0;

	time.sleep( 3000 );

	while ( running ) {

		if ( Pause ) {
			
			int hppercent = ( double ) values.health / 267 * 100;
			int mppercent = ( double ) values.mana / 267 * 100;

			try {

				if ( time.clock( ) > potionCooldown ) {
					if ( character.healthpotion != "" && hppercent <= character.healthpotionpercent ) {
						inputs.keyboard( character.healthpotion );
						potionCooldown = time.clock( ) + global;
					} else if ( character.manapotion != "" && mppercent <= character.manapotionpercent ) {
						inputs.keyboard( character.manapotion );
						potionCooldown = time.clock( ) + global;
					}
				}

				if ( time.clock( ) > healCooldown && values.mana >= 5 ) {
					if ( time.clock( ) > healCooldown3 && character.healing3 != "" && hppercent <= character.healing3percent ) {
						inputs.keyboard( character.healing3 );
						healCooldown = time.clock( ) + global;
						healCooldown3 = time.clock( ) + global;
					} else if ( time.clock( ) > healCooldown2 && character.healing2 != "" && hppercent <= character.healing2percent ) {
						inputs.keyboard( character.healing2 );
						healCooldown = time.clock( ) + global;
						healCooldown2 = time.clock( ) + global;
					} else if ( time.clock( ) > healCooldown1 && character.healing1 != "" && hppercent <= character.healing1percent ) {
						inputs.keyboard( character.healing1 );
						healCooldown = time.clock( ) + global;
						healCooldown1 = time.clock( ) + global;
					} else if ( values.paralyze && character.paralyze != "" ) {
						inputs.keyboard( character.paralyze );
						healCooldown = time.clock( ) + global;
					} else if ( values.poison && character.curepoison != "" ) {
						inputs.keyboard( character.curepoison );
						healCooldown = time.clock( ) + global;
					} else if ( time.clock( ) > recoveryCooldown && character.recovery != "" && hppercent <= 95 ) {
						inputs.keyboard( character.recovery );
						healCooldown = time.clock( ) + global;
						recoveryCooldown = time.clock( ) + 63000;
					}
				}

			} catch ( const std::exception& e ) {
				console.log( "ERROR:", e.what( ) );
			}

			time.sleep( 300 );

		} else {
  		time.sleep( 2000 );
		}
	}
}

void ammoThread( ) {

	double quiver = 0, throwing = 0;
	time.sleep( 4000 );

	while ( running ) {
		
		if ( Pause ) {

			try {

				if ( time.clock( ) > quiver && character.quiver != "" ) {
					inputs.keyboard( character.quiver );
					inputs.keyboard( character.quiver );
					inputs.keyboard( character.quiver );
					quiver = time.clock( ) + 180000;
				} else if ( values.weapon && character.throwing != "" ) {
					inputs.keyboard( character.throwing );
				}

			} catch ( const std::exception& e ) {
				console.log( "ERROR:", e.what( ) );
			}

			time.sleep( 2000 );
		} else {
  		time.sleep( 2000 );
		}
	}
}

void toolsThread( ) {

	double eat = 0,
		amulet,
		ring = 0,
		f = 0,
		haste = 0;

	time.sleep( 4000 );

	while ( running ) {
		
		if ( Pause ) {

			try {
				if ( mode.walking ) {
					if ( time.clock( ) > haste && character.haste != "" && !values.haste ) {
						inputs.keyboard( character.haste );
						haste = time.clock( ) + 33000;
					} else if ( time.clock( ) > f && values.flasks.size( ) > 0 ) {
						Point flask = values.flasks[ values.flasks.size( ) -1 ];
						inputs.mouseDrag( 836 + flask.x, 290 + flask.y );
						//console.log( "Throw flasks!" );
					} else if ( character.food != "" && values.hungry ) {
						inputs.keyboard( character.food );
						inputs.keyboard( character.food );
						inputs.keyboard( character.food );
					} else if ( character.ring != "" && values.ring == false ) {
						inputs.keyboard( character.ring );
						inputs.keyboard( character.ring );
						inputs.keyboard( character.ring );
					} else if ( character.necklace != "" && values.necklace == false ) {
						inputs.keyboard( character.necklace );
						inputs.keyboard( character.necklace );
						inputs.keyboard( character.necklace );
					}
				}
			} catch ( const std::exception& e ) {
				console.log( "ERROR:", e.what( ) );
			}
			time.sleep( 2000 );

		} else {
  		time.sleep( 2000 );
		}
	}
}

void keepDistance( ) {

	time.sleep( 1000 );

	while ( running ) {
		
		if ( Pause ) {
			try {

				if ( mode.attacking ) {
					if ( values.monsters.size( ) > 0 ) {
						if ( values.isTargeting ) {

							if ( values.targetPosition.x <= 3 && values.targetPosition.x >= 0 ||
							     values.targetPosition.x >= -3 && values.targetPosition.x <= 0 ) {
								if ( values.targetPosition.y <= 3 && values.targetPosition.y >= 0 ||
								     values.targetPosition.y >= -3 && values.targetPosition.y <= 0 ) {
									actions.runaway( values, script );
								}
							}

						}
					}
				}

			} catch ( const exception &err ) {
				console.log( "ERROR:", err.what( ) );
			}

			time.sleep( 300 );
		} else {
			time.sleep( 2000 );
		}

	}
}

void autoattackTargetThread( ) {

	vector <lootPoint> lootPoints;
	bool valuableLoot;
	bool keepAttacking;
	double attackCooldown = 0;
	double autoAttackCooldown = 0;
	int count2 = 0;
	bool lootSpot;

	time.sleep( 3000 );

	while ( running ) {
		
		if ( Pause ) {
			try {

				if ( mode.walking ) { // WALK

					if ( values.x == 0 && values.y == 0 ) {
						count2 += 1;
						if ( count2 >= 8 ) {
							count2 = 0;
							inputs.keyboard( "s" );
						}
					}

					if ( values.monsters.size( ) == 0 ) {
						
						if ( lootPoints.size( ) >= 1 ) {
							lootSpot = actions.lootway( values, script, lootPoints[0] );
							if ( lootSpot ) {
								actions.looter( );
								lootPoints.erase(lootPoints.begin()+0);			
							}
						} else { actions.walker( values, script ); }
						
					} else {
						mode.attacking = true;
						mode.walking = false;
					}

				} else if ( mode.attacking ) { // ATTACK

					if ( values.monsters.size( ) > 0 ) {
						if ( !values.isTargeting ) {
							if ( time.clock( ) > autoAttackCooldown ) {
								time.sleep( 50 );
								inputs.keyboard( "space" );
								autoAttackCooldown = time.clock( ) + 1000;
							}
						} else if ( time.clock( ) > attackCooldown ) {
							if ( character.divinemissile != "" ) {
								inputs.keyboard( character.divinemissile );
								attackCooldown = time.clock( ) + 2100;
							}
						}
					} else {
						valuableLoot = status.valuableLoot( );
						if ( valuableLoot ) {
							for ( int i = 0; i < script.positions.size(); i++ ) {
								if ( values.x + values.targetPosition.x == script.positions[i].x &&
								values.y + values.targetPosition.y == script.positions[i].y ) {	
									lootPoint p = lootPoint( );							
									p.add( values.x + values.targetPosition.x, values.y + values.targetPosition.y );
									lootPoints.push_back( p );
									console.log( values.x + values.targetPosition.x, values.y + values.targetPosition.y );
									break;
								}
							}
						}
						mode.walking = true;
						mode.attacking = false;
					}

				} else if ( mode.looting ) { 
				}

			} catch ( const exception &err ) {
				console.log( "ERROR:", err.what( ) );
			}

			time.sleep( 300 );
		} else {
			time.sleep( 2000 );
		}

	}
}


void alarmThread( ) {

	time.sleep( 3000 );

	string mp3 = "mpg123 /home/" + user + "/afk/alarma.mp3 > /dev/null 2>&1";

	while ( running ) {
		
		if ( Pause ) {
			if ( values.alarm ) {
				
				system( mp3.c_str( ) );

				time.sleep( 60000 );
				if ( character.hunt == "target" ) {
					inputs.keyboard( character.healing1 );
					time.sleep( 1100 );
					inputs.keyboard( character.healing1 );
					time.sleep( 1100 );
					inputs.keyboard( character.healing1 );
					time.sleep( 1100 );
					inputs.keyboard( character.healing1 );
					time.sleep( 1100 );
				}
			}
			time.sleep( 2000 );
		}
		
	}
}

void run( ) {

	int id = client.get( );
	status.id = id;
	viewport.id = id;

	string file = "/home/" + user + "/afk/character.json";

	character.load( file.c_str( ) );
	script.load( "/opt/afk/maps/" + character.script );

	if ( id != 0 ) {
		thread( updateThread ).detach( );
		thread( alarmThread ).detach( );
		thread( toolsThread ).detach( );
		thread( healThread ).detach( );
		if ( character.vocation == "knight" && character.hunt == "target" ) {
			thread( autoattackTargetThread ).detach( );
		} else if ( character.vocation == "paladin" && character.hunt == "target" ) {
			thread( autoattackTargetThread ).detach( );
			thread( keepDistance ).detach( );
			thread( ammoThread ).detach( );
		}
	}

}

int main( int argc, char** argv ) {

	system( "rm -f /tmp/licence.json" );
	system( "wget -O /tmp/licence.json https://siewdass.github.io/licence.json > /dev/null 2>&1" );
	
	licence.load( "/tmp/licence.json" );

	id = argv[ 1 ];
	user = argv[ 2 ];

	for ( auto entry : licence.accepted ) {
		if ( entry == id ) {
			Interface( argc, argv, run );
			break;
		}
	}

	return 0;
}

// cambiar Point por un nuevo struct y 
// detectar cantidad de vida segun pixeles
// internos de la barra de vida
// autoattack siempre al que tenga mas vida
// exori min = minimo 2