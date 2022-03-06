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

			auto [ box,monsters, visibleMonsters, side, isTargeting, targetPosition, boxTarget ] = viewport.check( );
			auto [ health, mana, hungry, paralyze, poison, haste, x, y, z, minimap, flasks, alarm ] = status.update( script.positions );

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

			//console.log( x, y );

			time.sleep( 200 );

		} else {
  		time.sleep( 2000 );
		}

	}
}

void alternatorThread( ) {

	int lastX = 0, lastY = 0;
	time.sleep( 2000 );

	while ( running ) {
		
		if ( Pause ) {

			if ( mode.walking ) {

				mode.attacking = actions.walker( values, script );
				if ( mode.attacking && values.visibleMonsters ) {
					mode.walking = false;
				}
				if ( !mode.attacking && values.x == lastX && values.y == lastY && values.box >= 4 ) {
					//console.log( "TTTTTTTTRRRRRRRRRRAAAAAAAAAPPPPPPPP" );
					mode.attacking = true;
					mode.walking = false;
				} else {
					lastX = values.x;
					lastY = values.y;
				}

			} else if ( mode.attacking ) {		

				mode.looting = actions.attacker( values );
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
					} else if ( character.ring != "" &&  time.clock( ) > ring ) {
						inputs.keyboard( character.ring );
						inputs.keyboard( character.ring );
						inputs.keyboard( character.ring );
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

void autoattackTargetThread( ) {

	bool keepAttacking;
	double autoAttackCooldown = 0;

	time.sleep( 3000 );

	while ( running ) {
		
		if ( Pause ) {
			try {

				if ( mode.walking ) {
					if ( values.monsters.size( ) == 0 ) {
						mode.attacking = actions.walker( values, script );
						if ( mode.attacking ) {
							mode.walking = false;
						}
					} else {
						mode.attacking = true;
						mode.walking = false;
					}
				} else if ( mode.attacking ) {
					if ( values.monsters.size( ) > 0 ) {
						if ( time.clock( ) > autoAttackCooldown && !values.isTargeting ) {
							inputs.keyboard( "Escape" );
							time.sleep( 1000 );
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

					if ( !values.isTargeting ) {

						if ( character.vocation == "knight" ) {
							mode.walking = actions.looter( );
							if ( mode.walking && values.monsters.size( ) > 0 ) {
								mode.looting = false;
								mode.attacking = true;
								mode.walking = false;
							} else if ( mode.walking && values.monsters.size( ) == 0 ) {
								mode.looting = false;
								mode.attacking = false;
								mode.walking = true;
							}
						} else {
							auto [ X, Y ] = actions.mapWindow( values.targetPosition.x, values.targetPosition.y );
							inputs.mouse( X, Y, 1 );
							time.sleep( 3000 );
							mode.walking = actions.looter( );
							auto [ XX, YY ] = actions.gameWindow( 0, 0 );
							inputs.shiftDown( );
							inputs.mouse( XX, YY, 3 );
							inputs.shiftUp( );
							/*inputs.shiftDown( );
							inputs.mouse( X, Y, 3 );
							inputs.shiftUp( );
							time.sleep( 3000 );*/
							mode.looting = false;
							mode.attacking = false;
						}

					}
				}

			} catch ( const exception &err ) {
				console.log( "ERROR:", err.what( ) );
			}

			time.sleep( 500 );
		} else {
			time.sleep( 2000 );
		}

	}
}

void autoAttackThread( ) {

	bool keepAttacking;
	double autoAttackCooldown = 0;

	time.sleep( 3000 );

	while ( running ) {
		
		if ( Pause ) {
			
			try {

				if ( mode.attacking ) {
					if ( values.box >= 3 || keepAttacking ) {
						keepAttacking = true;
						if ( time.clock( ) > autoAttackCooldown && !values.isTargeting ) {
							if ( values.box >= 1 ) {
								for ( Point point : values.monsters ) {
									if ( point.x >= -1 & point.x <= 1 & point.y >= -1 & point.y <= 1 ) {
										auto [ x, y ] = actions.gameWindow( point.x, point.y );
										inputs.mouse( x, y, 3 );
										autoAttackCooldown = time.clock( ) + 1000;
										break;
									}
								}
							}
						}
					}
					time.sleep( 500 );
				} else {
					keepAttacking = false;
					time.sleep( 2000 );
				}

			} catch ( const exception &err ) {
				console.log( "ERROR:", err.what( ) );
			}

		} else {
  		time.sleep( 2000 );
		}
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
		
		if ( Pause ) {
			try {
				if ( mode.attacking ) {

					if ( values.mana > 10 ) {
						if ( values.box >= 3 || keepAttacking ) {
							keepAttacking = true;
							if ( time.clock( ) > attackCooldown ) {
								if ( character.annihilation != "" && time.clock( ) > attackCooldown1 && values.isTargeting && values.box == 1 ) {
									inputs.keyboard( character.annihilation );
									attackCooldown = time.clock( ) + global*2;
									attackCooldown1 = time.clock( ) + 31500;
								} else if ( character.brutalstrike != "" && time.clock( ) > attackCooldown2 && values.isTargeting && values.box == 1 ) {
									inputs.keyboard( character.brutalstrike );
									attackCooldown = time.clock( ) + global;
									attackCooldown2 = time.clock( ) + 6300;
								} else if ( character.whirlwindthrow != "" && time.clock( ) > attackCooldown3 && values.isTargeting && values.box == 1 ) {
									inputs.keyboard( character.whirlwindthrow );
									attackCooldown = time.clock( ) + global;
									attackCooldown3 = time.clock( ) + 6300;
								} else if ( character.groundshaker != "" && time.clock( ) > attackCooldown4 && values.box <= 3 && values.monsters.size( ) >= 5 ) {
									inputs.keyboard( character.groundshaker );
									attackCooldown = time.clock( ) + global;
									attackCooldown4 = time.clock( ) + 8400;
								} else if ( character.berserker != "" && time.clock( ) > attackCooldown5 && values.box >= 2 ) {
									inputs.keyboard( character.berserker );
									attackCooldown = time.clock( ) + global;
									attackCooldown5 = time.clock( ) + 4200;
								} else if ( character.fierceberserker != "" && time.clock( ) > attackCooldown6 && values.box >= 2 ) {
									inputs.keyboard( character.fierceberserker );
									attackCooldown = time.clock( ) + global;
									attackCooldown6 = time.clock( ) + 6300;
								} else if ( character.frontsweep != "" && time.clock( ) > attackCooldown7 && values.box >= 2 ) {
									string modifier = "control+";
									modifier.append( values.side );
									inputs.keywith( modifier.c_str( ) );
									time.sleep( 100 );
									inputs.keyboard( character.frontsweep );
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
		if ( character.vocation == "knight" && character.hunt == "box" ) {
			thread( alternatorThread ).detach( );
			thread( autoAttackThread ).detach( );
			thread( attackThread ).detach( );
		} else if ( character.vocation == "knight" && character.hunt == "target" ) {
			thread( autoattackTargetThread ).detach( );
		} else if ( character.vocation == "paladin" && character.hunt == "target" ) {
			thread( autoattackTargetThread ).detach( );
			thread( ammoThread ).detach( );
		}
	}

}

int main( int argc, char** argv ) {

	auto [ x, y ] = actions.gameWindow( 0, 0 );
	console.log( x, y );

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


//auto [ x, y ] = actions.mapWindow( 0, -56 );
//console.log( x, y );
//inputs.mouse( x, y, 1 );
// cambiar Point por un nuevo struct y 
// detectar cantidad de vida segun pixeles
// internos de la barra de vida
// autoattack siempre al que tenga mas vida
// if ( !x ) { return } 
// exori min = minimo 2