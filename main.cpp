#include "screenshot.h"
#include "utils.h"
#include "types.h"
#include "shared.h"
#include "stats.h"
#include "viewport.h"
#include "location.h"
#include "character.h"
#include "actions.h"
#include "path.h"
#include "utils.h"
#include "inputs.h"

string CHARACTER = "";
string WAYPOINTS = "";
string TIBIA = "";
bool loaded = false;
bool running = false;
function < void ( ) > run;
Values values;
Client client;

bool Pause = false;
bool Stop = false;

void Toggle( ) {}

Stats stats;
Viewport viewport;

Location location;
Character character;

Time time;
Mode mode;
Path path;

Actions actions;
Inputs inputs;

Screenshot screenshot;

int height = 642, width = 1012, middle = ( 1012 / 2 ), percent = ( ( width / 2 ) - 188 ) - 32, unknow = 0;
Mat HP, MP, BD, FK, minimap, Player, CHAT, LOOTCHAT, WEAPON, RING, AMULET, CAPACITY, GameWindow, FloorIndex, Minimap, Monsters;

void updateLocation( ) {

	double cooldown = 0;
	int oldX, oldY, count = 0;

	while ( running ) {

		try {

			auto [ XPOS, YPOS ] = inputs.mouseLocation( );
			if ( XPOS < 190 || XPOS > 824 || YPOS < 42 || YPOS > 507 ) { inputs.mouseMove( 742, 596 ); }

			FloorIndex = screenshot.get( 1001, 54, 1, 61 );
			Minimap = screenshot.get( 898 - 16, 59 - 16, 34, 34 );

			auto [ x, y, z, validSqm ] = location.update( FloorIndex, Minimap, values.x, values.y );

			count = ( oldX == x && oldY == y ) ? count + 1 : 0;
			oldX = x;
			oldY = y;

			values.canWalk = count >= 2; 
			values.x = x;
			values.y = y;
			values.z = z;
			values.validSqm = validSqm;
	
		} catch ( const std::exception& e ) {
		}

		time.sleep( 300 );
	}
}

void updateThread( ) {

	while ( running ) {

		try {

			GameWindow = screenshot.get( 189, 40, 615, 451 );
			
			auto [ box, monsters, side, isTargeting, targetPosition, _ ] = viewport.check( GameWindow );

			values.box = box;
			values.monsters = monsters;
			values.side = side;
			values.isTargeting = isTargeting;
			values.targetPosition = targetPosition;

			CAPACITY = screenshot.get( 919, 255, 30, 1 );
			WEAPON = screenshot.get( 861, 186, 1, 1 );
			AMULET = screenshot.get(  862, 158, 1, 1 );
			RING = screenshot.get( 861, 220, 1, 1 );
			Player = screenshot.get( 26, 30, 1, 1 );
			CHAT = screenshot.get( 189, 575, 1, 42 );
			HP = screenshot.get( 188, 5, middle-188-40, 1 );
			MP = screenshot.get( middle+40, 5, width-middle-188+40, 1 );
			BD = screenshot.get( 480, 4, 53, 27);
			FK = screenshot.get( 836, 304, 176, 296 );
			Monsters = screenshot.get( 23, 72, 1, 174 );
			//imwrite( "screenshot.png", CAPACITY );

			auto [ health, mana, hungry, paralyze, poison, haste, flasks, alarm, near, weapon, necklace, ring, lowcapacity, target ] = stats.update( CAPACITY, WEAPON, AMULET, RING, Player, CHAT, HP, MP, BD, FK, Monsters );

			values.health = health;
			values.mana = mana;
			values.hungry = hungry;
			values.paralyze = paralyze;
			values.poison = poison;
			values.haste = haste;
			values.flasks = flasks;
			values.alarm = alarm;
			values.weapon = weapon;
			values.necklace = necklace;
			values.ring = ring;
			values.lowcapacity = lowcapacity;
			values.target = target;
			values.nearPlayer = near;

			//console.log( "x:", values.x, "y:", values.y, "z:", values.z, "hp:", to_string( ( health / 278 ) * 100 ) + "%", "mana:", to_string( ( mana / 278 ) * 100 ) + "%" );

		} catch ( const std::exception& e ) {
		}

		time.sleep( 300 );
	}
}

void alternatorThread( ) {

	while ( running ) {

		try {

			if ( mode.walking ) {

				if ( values.canWalk ) {
					if ( values.box == values.validSqm || values.box == values.validSqm - 1 ) {
						position = position == character.waypoints.size( ) ? 0 : position + 1;
						mode.attacking = true;
						mode.walking = false;
					} else {
						mode.attacking = actions.walker( values, character, location.paths );
						if ( mode.attacking ) mode.walking = false;
						time.sleep( 2000 );
					}
				}

			} else if ( mode.attacking ) {		

				mode.looting = actions.attacker( values );
				if ( mode.looting ) mode.attacking = false;

			} else if ( mode.looting ) {

				mode.walking = actions.looter( );
				mode.looting = false;

				actions.throwFlask( values );
					
				if ( Stop == true ) Toggle( );
			}

		} catch ( const std::exception& e ) {
		}

		time.sleep( 250 );
	}
}

void healThread( ) {

	double global = 1050, healCooldown = 0, healCooldown1 = 0, healCooldown2 = 0, healCooldown3 = 0, recoveryCooldown = 0;

	while ( running ) {

		int hppercent = ( double ) values.health / percent * 100;
		int mppercent = ( double ) values.mana / percent * 100;

		try {

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
				} else if ( values.paralyze && character.paralyze != "" && mode.walking ) {
					inputs.keyboard( character.paralyze );
					healCooldown = time.clock( ) + global;
				} else if ( values.poison && character.curepoison != "" ) {
					inputs.keyboard( character.curepoison );
					healCooldown = time.clock( ) + global;
				} else if ( time.clock( ) > recoveryCooldown && character.recovery != "" && hppercent < 100 ) {
					inputs.keyboard( character.recovery );
					healCooldown = time.clock( ) + global;
					recoveryCooldown = time.clock( ) + 63000;
				}
			}

		} catch ( const std::exception& e ) {
			console.log( "ERROR:", e.what( ) );
		}

		time.sleep( 300 );
	}
}

void potionsThread( ) {

	double global = 1050;
	double potionCooldown = 0;

	while ( running ) {
		
		int hppercent = ( double ) values.health / percent * 100;
		int mppercent = ( double ) values.mana / percent * 100;

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

		} catch ( const std::exception& e ) {
			console.log( "ERROR:", e.what( ) );
		}

		time.sleep( 300 );
	}
}

void ammoThread( ) {

	double quiver = 0, throwing = 0;

	while ( running ) {

		try {

			if ( time.clock( ) > quiver && character.quiver != "" ) {
				inputs.keyboard( character.quiver );
				inputs.keyboard( character.quiver );
				inputs.keyboard( character.quiver );
				quiver = time.clock( ) + 60000;
			} else if ( values.weapon && character.throwing != "" ) {
				inputs.keyboard( character.throwing );
			}

		} catch ( const std::exception& e ) {
			console.log( "ERROR:", e.what( ) );
		}

		time.sleep( 2000 );
		
	}
}

void toolsThread( ) {

	double necklaceCooldown = 0, ringCooldown = 0, foodCooldown = 0, hasteCooldown = 0;

	while ( running ) {
		try {
			if ( mode.walking ) {
				if ( time.clock( ) > hasteCooldown && character.haste != "" && !values.haste ) {
					inputs.keyboard( character.haste );
					hasteCooldown = time.clock( ) + 33000;
				} else if ( time.clock( ) > foodCooldown && character.food != "" && values.hungry ) {
					inputs.keyboard( character.food );
					inputs.keyboard( character.food );
					inputs.keyboard( character.food );
					foodCooldown = time.clock( ) + 180000;
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
	}
}

void attackKnightBox( ) {

	bool keepAttacking;
	double global = 2100, challengeCooldown = 0, bloodrageCooldown = 0,
	attackCooldown = 0, attackCooldown1 = 0, attackCooldown2 = 0, attackCooldown3 = 0, attackCooldown4 = 0, attackCooldown5 = 0, attackCooldown6 = 0, attackCooldown7 = 0;

	while ( running ) {

		try {
			if ( mode.attacking ) {

				if ( values.mana > 10 ) {
					if ( values.box >= 2 || keepAttacking ) {
						keepAttacking = true;
						if ( time.clock( ) > attackCooldown ) {
							if ( character.bloodrage != "" && time.clock( ) > bloodrageCooldown && values.box > 0 ) {
								inputs.keyboard( character.bloodrage );
								bloodrageCooldown = time.clock( ) + 10500;
								attackCooldown = time.clock( ) + 100;
							} else if ( character.challenge != "" && time.clock( ) > challengeCooldown && values.box > 0 ) {
								inputs.keyboard( character.challenge );
								challengeCooldown = time.clock( ) + 3150;
								attackCooldown = time.clock( ) + 100;
							} else if ( character.annihilation != "" && time.clock( ) > attackCooldown1 && values.target && values.box == 1 ) {
								inputs.keyboard( character.annihilation );
								attackCooldown = time.clock( ) + global*2;
								attackCooldown1 = time.clock( ) + 31500;
							} else if ( character.brutalstrike != "" && time.clock( ) > attackCooldown2 && values.target && values.box == 1 ) {
								inputs.keyboard( character.brutalstrike );
								attackCooldown = time.clock( ) + global;
								attackCooldown2 = time.clock( ) + 6300;
							} else if ( character.whirlwindthrow != "" && time.clock( ) > attackCooldown3 && values.target && values.box == 1 ) {
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
							} else if ( character.frontsweep != "" && values.side != "" && time.clock( ) > attackCooldown7 && values.box >= 2 ) {
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
	}
}

void attackPaladinBox( ) {

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

	while ( running ) {

		try {
			if ( mode.attacking ) {
				if ( values.box >= 3 || keepAttacking ) {
					keepAttacking = true;
					if ( time.clock( ) > attackCooldown && values.mana > 10 ) {
						if ( character.sharpshooter != "" && time.clock( ) > attackCooldown1 && values.box >= 2 ) {
							inputs.keyboard( character.sharpshooter );
							attackCooldown = time.clock( ) + 1000;
							attackCooldown1 = time.clock( ) + 8400;
						} else if ( character.strongetherealspear != "" && time.clock( ) > attackCooldown2 && values.target && values.box == 1 ) {
							inputs.keyboard( character.strongetherealspear );
							attackCooldown = time.clock( ) + global;
							attackCooldown2 = time.clock( ) + 8400;
						} else if ( character.etherealspear != "" && time.clock( ) > attackCooldown3 && values.target && values.box == 1 ) {
							inputs.keyboard( character.etherealspear );
							attackCooldown = time.clock( ) + global;
							attackCooldown3 = time.clock( ) + 2100;
						} else if ( character.divinecaldera != "" && time.clock( ) > attackCooldown4 && ( values.box >= 2 || values.box <= 3 && values.monsters.size( ) >= 5 ) ) {
							inputs.keyboard( character.divinecaldera );
							attackCooldown = time.clock( ) + global;
							attackCooldown4 = time.clock( ) + 4200;
						} else if ( character.arearune != "" && time.clock( ) > attackCooldown5 && values.box >= 2 ) {
							inputs.keyboard( character.arearune );
							//time.sleep( 100 );
							//inputs.mouse( values.targetPosition.x, values.targetPosition.y, 0 );
							attackCooldown = time.clock( ) + global;
							attackCooldown5 = time.clock( ) + 2100;
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

void knightTargetThread( ) {

	double autoAttackCooldown = 0;
	int count;

	while ( running ) {

		try {

			if ( mode.walking ) {

				if ( values.canWalk && values.monsters.size( ) == 0 ) {
					actions.walker( values, character, location.paths );
				} else if ( values.monsters.size( ) > 0 ) {
					mode.attacking = true;
					mode.walking = false;
					inputs.keyboard( "Escape" );
				}

			} else if ( mode.attacking ) {

				if ( !values.isTargeting && values.monsters.size( ) > 0 ) {
					if ( time.clock( ) > autoAttackCooldown ) {
						auto [ x, y ] = actions.gameWindow( values.monsters[ 0 ].x, values.monsters[ 0 ].y );
						inputs.mouse( x, y, 3 );
						autoAttackCooldown = time.clock( ) + 1000;
						inputs.mouseMove( 742, 596 );
					}
				} else if ( values.isTargeting ) {
					mode.looting = true;
					mode.attacking = false;
				}

			} else if ( mode.looting ) {

				count = values.isTargeting ? 0 : count + 1;

				if ( count > 3 ) {
					actions.looter( );
					mode.looting = false;
					mode.walking = true;
				}
			}

		} catch ( const exception &err ) {
			console.log( "ERROR:", err.what( ) );
		}

		time.sleep( 300 );
	}
}

void keepDistance( ) {

	while ( running ) {

		try {

			if ( mode.attacking ) {
				if ( values.monsters.size( ) > 0 ) {
					if ( values.isTargeting ) {

						if ( abs( values.targetPosition.x - values.x ) > 4 || abs( values.targetPosition.y - values.y ) > 4 ) {
							// follow
							//actions.run( values, values.targetPosition, location.paths );
						} else if ( abs( values.targetPosition.x - values.x ) < 4 || abs( values.targetPosition.y - values.y ) < 4 ) {
							// distance
							// si ya llego a ese punto tiene que ir al anterior
							//int pos = position == 0 ? character.waypoints.size( ) - 1 : position;
							//actions.run( values, Point( character.waypoints[ pos ].x, character.waypoints[ pos ].y ), location.paths );
						}

					}
				}
			}

		} catch ( const exception &err ) {
			console.log( "ERROR:", err.what( ) );
		}

		time.sleep( 300 );

	}
}

void paladinTargetThread( ) {

	double autoAttackCooldown = 0;
	int count;

	while ( running ) {

		try {

			if ( mode.walking ) {

				if ( values.canWalk && values.monsters.size( ) == 0 ) {
					actions.walker( values, character, location.paths );
				} else if ( values.monsters.size( ) > 0 ) {
					mode.attacking = true;
					mode.walking = false;
					inputs.keyboard( "Escape" );
				}

			} else if ( mode.attacking ) {

				if ( !values.isTargeting && values.monsters.size( ) > 0 ) {
					if ( time.clock( ) > autoAttackCooldown ) {
						auto [ x, y ] = actions.gameWindow( values.monsters[ 0 ].x, values.monsters[ 0 ].y );
						inputs.mouse( x, y, 3 );
						autoAttackCooldown = time.clock( ) + 1000;
						inputs.mouseMove( 742, 596 );
					}
				} else if ( values.isTargeting ) {
					mode.looting = true;
					mode.attacking = false;
				}

			} else if ( mode.looting ) {

				count = values.isTargeting ? 0 : count + 1;

				if ( count > 3 ) {
					actions.looter( );
					mode.looting = false;
					mode.walking = true;
				}
			}

		} catch ( const exception &err ) {
			console.log( "ERROR:", err.what( ) );
		}

		time.sleep( 300 );
	}
}

void autoAttackThread( ) {

	double autoAttackCooldown = 0;

	while ( running ) {
		try {
			if ( mode.attacking ) {

				if ( time.clock( ) > autoAttackCooldown && values.box >= 1 && !values.target ) {
					inputs.keyboard( "space" );
					autoAttackCooldown = time.clock( ) + 1000;
				}
				
				time.sleep( 200 );
			} else {
				time.sleep( 1000 );
			}
		} catch ( const exception &err ) {
			console.log( "ERROR:", err.what( ) );
		}
	}
}

void alarmThread( ) {

	double criticalCooldown = 0, safeCooldown = 0;

	while ( running ) {
		if ( time.clock( ) > criticalCooldown && ( values.alarm || values.mana <= 20 || values.health <= 20 ) )  {
			values.alarm = false;
			Stop = true;
			system( "mpg123 /opt/afk/critical.mp3 > /dev/null 2>&1" );
			criticalCooldown = time.clock( ) + 60000;
		} else if ( time.clock( ) > safeCooldown && values.lowcapacity ) {
			values.lowcapacity = false;
			system( "mpg123 /opt/afk/safe.mp3 > /dev/null 2>&1" );
			safeCooldown = time.clock( ) + 60000;
		} else if ( mode.walking && values.nearPlayer ) {
			int index = position;

			while ( true ) {
				if ( index == character.waypoints.size( ) ) index = 0;

				Waypoint waypoint = character.waypoints[ index ];
				if ( waypoint.action == "attack" ) {
					position = index + 1;
					break;
				}
				
				index++;
			}

			values.nearPlayer = false;
			system( "mpg123 /opt/afk/player.mp3 > /dev/null 2>&1" );
		}
		time.sleep( 2000 );
	}

	values.lowcapacity = false;
	values.alarm = false;
}

void networkThread( ) {
	string ping;
	string prefix = "time=";
	int number = 0;
	int count = 0;

	while ( running ) {
		try {
			ping = client.process( "ping -c 1 8.8.8.8 | awk '/time=/ {print $7}' | cut -d '/' -f1 | cut -d '.' -f1" );

			if ( ping.size() >= prefix.size() && ping.substr(0, prefix.size()) == prefix ) {
				size_t pos = ping.find(prefix);
				if (pos != string::npos) {
					number = std::stoi( ping.substr(pos + 5) );
					count = number > 100 ? count + 1 : 0;
					if ( count >= 3 ) system( "mpg123 /opt/afk/lag.mp3 > /dev/null 2>&1" );
				}
			}
		} catch ( exception &err ) { }

		time.sleep( 3000 );
	}
}

void execute( ) {
	//console.log( WAYPOINTS );
	screenshot.load( TIBIA );
	character.load( CHARACTER.c_str( ), WAYPOINTS.c_str( ) );
	inputs.load( TIBIA.c_str( ) );
	//location.walkable( character.walkable );
	values.x = character.waypoints.size( ) > 0 ? character.waypoints[ 0 ].x : 0;
	values.y = character.waypoints.size( ) > 0 ? character.waypoints[ 0 ].y : 0;

	/*thread( updateLocation ).detach( );
	thread( updateThread ).detach( );
	thread( healThread ).detach( );
	thread( potionsThread ).detach( );
	thread( toolsThread ).detach( );*/

	//thread( networkThread ).detach( );

	/*if ( character.waypoints.size( ) > 0 ) {
		thread( alarmThread ).detach( );
		
		if ( character.vocation == "knight" && character.hunt == "box" ) {
			thread( alternatorThread ).detach( );
			thread( autoAttackThread ).detach( );
			thread( attackKnightBox ).detach( );
		} else if ( character.vocation == "knight" && character.hunt == "target" ) {
			thread( knightTargetThread ).detach( );
		} else if ( character.vocation == "paladin" && character.hunt == "box" ) {
			thread( alternatorThread ).detach( );
			thread( autoAttackThread ).detach( );
			thread( attackPaladinBox ).detach( );
			thread( ammoThread ).detach( );
		} else if ( character.vocation == "paladin" && character.hunt == "target" ) {
			thread( paladinTargetThread ).detach( );
			//thread( keepDistance ).detach( );
			thread( attackPaladinBox ).detach( );
			thread( ammoThread ).detach( );
		}
	}*/
}

int main( int argc, char** argv ) {
	
	location.load( );
	loaded = true;
	//thread( [&]( ) { location.load( ); loaded = true; }).detach( );

	TIBIA = string( ":" + string(argv[ 1 ]) );
	WAYPOINTS = string( "/opt/afk/waypoints/" + string(argv[ 2 ]) );
	CHARACTER = string( "/opt/afk/characters/" + string(argv[ 3 ]) );

	console.log( TIBIA );
	console.log( CHARACTER );
	console.log( WAYPOINTS );

	execute( );
	//run = execute;
	/*string serial = client.process( "lsblk --nodeps -no serial /dev/$(lsblk -ndo pkname $(df -h / | sed 1d | cut -d ' ' -f 1))" );

	system( "rm -f /tmp/licence.json" );
	system( "wget -O /tmp/licence.json https://siewdass.github.io/licence.json > /dev/null 2>&1" );

	Json config;
	config.open( "/tmp/licence.json" );

	for ( Json entry: config.readArray( "accepted" ) ) {
		if ( entry.readString( "id" ) == serial ) {
			Interface( argc, argv );
			break;
		}
	}

	console.log( serial );*/

	//Interface( argc, argv );

	return 0;
}


