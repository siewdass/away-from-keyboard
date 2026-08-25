#include "screenshot.h"
#include "inputs.h"
#include "procedure.h"

Control control;
bool found = false;

#define PARAMS int client, Screenshot& screenshot, Inputs& inputs, Player& player, Character& character, Actions& actions

void locationThread(PARAMS) {

    Mat FloorIndex, Minimap;
	double cooldown = 0;
	int oldX, oldY, count = 0;

	procedure.setWalkable( character.walkables );
	int gen = control.getGeneration( client );

	while ( control.getRunning( client ) && control.getGeneration( client ) == gen ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );
		
		auto [ XPOS, YPOS ] = inputs.mouseLocation( );		
		if ( XPOS < 190 || XPOS > 824 || YPOS < 42 || YPOS > 507 ) { inputs.mouseMove( 742, 596 ); }

		FloorIndex = screenshot.get( 1001, 54, 1, 61 );
		
		Minimap = screenshot.get( 898 - 16, 59 - 16, 34, 34 );

		auto [ x, y, z, validSqm ] = procedure.location( FloorIndex, Minimap, player.x, player.y );

		count = ( oldX == x && oldY == y ) ? count + 1 : 0;
		oldX = x;
		oldY = y;

		player.canWalk = count >= 2; 
		player.x = x;
		player.y = y;
		player.z = z;
		player.validSqm = validSqm;
		
		//console.log("X: " + to_string(x) + " Y: " + to_string(y) + " Z: " + to_string(z));

		timer.sleep( 300 );
	}
}

void statsThread(PARAMS) {

	int width = 1012, middle = ( 1012 / 2 );
	int AlarmCount = 0;
	Mat HP, MP, BD, FK, Player, CHAT, LOOTCHAT, CAPACITY, GameWindow, Monsters,
	WEAPON, RING, AMULET, OFFHAND, HELMET, ARMOR;

	int gen = control.getGeneration( client );

	while ( control.getRunning( client ) && control.getGeneration( client ) == gen ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );

		GameWindow = screenshot.get( 189, 40, 615, 451 );
		
		auto [ box, monsters, side, isTargeting, targetPosition, _ ] = procedure.viewport( GameWindow );

		player.box = box;
		player.monsters = monsters;
		player.side = side;
		player.isTargeting = isTargeting;
		player.targetPosition = targetPosition;

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
		OFFHAND = screenshot.get( 935, 186, 1, 1 );
		ARMOR = screenshot.get( 900, 170, 1, 1 );
		HELMET = screenshot.get( 900, 133, 1, 1 );

		//imwrite( "screenshot.png", HELMET );

		auto [
			health, mana, hungry, paralyze, poison, haste, flasks, alarm, near, weapon, necklace, ring, lowcapacity, target, offhand
		] = procedure.stats(
			HELMET, ARMOR, OFFHAND, CAPACITY, WEAPON, AMULET, RING, Player, CHAT, HP, MP, BD, FK, Monsters, AlarmCount
		);

		player.health = health;
		player.mana = mana;
		player.hungry = hungry;
		player.paralyze = paralyze;
		player.poison = poison;
		player.haste = haste;
		player.flasks = flasks;
		player.alarm = alarm;
		player.weapon = weapon;
		player.offhand = offhand;
		player.necklace = necklace;
		player.ring = ring;
		player.lowcapacity = lowcapacity;
		player.target = target;
		player.nearPlayer = near;

		//console.log( "hp:", to_string( health ), "mana:", to_string( mana ) );

		timer.sleep( 300 );
	}
}

void alternatorThread(PARAMS) {
    
	int count = 0;
	bool avoid = false;
	int gen = control.getGeneration( client );

	while ( control.getRunning( client ) && control.getGeneration( client ) == gen ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );

		if ( player.walking ) {

			if ( player.canWalk ) {
				if ( player.box == player.validSqm || player.box == player.validSqm - 1 ) {
					player.position = ( player.position + 1 ) % character.waypoints.size();
					player.attacking = true;
					player.walking = false;
				} else {
					auto [ attacking, X, Y, door ] = procedure.walker( player, character, procedure.paths );
					if ( door ) {
						inputs.mouse( X, Y, 3 );
						avoid = true;
					} else if ( !attacking && X != 0 && Y != 0 ) {
						inputs.mouse( X, Y, 1 );
					} else if ( attacking && player.monsters.size() != 0 ) {
						player.attacking = true;
						player.walking = false;
					}
					if ( !avoid ) timer.sleep( 1500 );
					avoid = false;
					// si hay mas de 1 monstruo 2000 si hay 1 o menos entonces 500
					// con esto tengo para hacer que camine mas rapido en ocaciones especificas
				}
			}

		} else if ( player.attacking ) {		

			if ( player.box == 0 ) count++;
			else count = 0;

			// agregue para que looteara mas rapido
			if ( count == 5 ) {
				player.looting = true;
				player.attacking = false;
			}

		} else if ( player.looting ) {

			for ( auto [ X, Y ] : procedure.looter( ) ) {
				inputs.shiftClick( X, Y, 3 );
				timer.sleep( 100 );
			}

			player.looting = false;
			player.walking = true;

			for ( auto [ flaskX, flaskY ] : procedure.throwFlask( player ) ) inputs.mouseDrag( flaskX, flaskY );
			procedure.walk = true;
		}

		timer.sleep( 250 );
	}
}

void healThread(PARAMS) {

	int percent = ( ( 1012 / 2 ) - 188 ) - 32;
	double global = 1050, healCooldown = 0, healCooldown1 = 0, recoveryCooldown = 0;
	int gen = control.getGeneration( client );

	while ( control.getRunning( client ) && control.getGeneration( client ) == gen ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );

		int hppercent = ( double ) player.health / percent * 100;
		int mppercent = ( double ) player.mana / percent * 100;

		if ( timer.clock( ) > healCooldown && player.mana >= 5 ) {
			if ( timer.clock( ) > healCooldown1 && character.healing != "" && hppercent <= character.healingpercent ) {
				//inputs.keyboard( character.healing );
				actions.add( { 3, character.healing } );
				healCooldown = timer.clock( ) + global;
				healCooldown1 = timer.clock( ) + global;
			} else if ( player.paralyze && character.paralyze != "" && player.walking ) {
				//inputs.keyboard( character.paralyze );
				actions.add( { 3, character.paralyze } );
				healCooldown = timer.clock( ) + global;
			} else if ( player.poison && character.curepoison != "" ) {
				//inputs.keyboard( character.curepoison );
				actions.add( { 3, character.curepoison } );
				healCooldown = timer.clock( ) + global;
			} else if ( timer.clock( ) > recoveryCooldown && character.recovery != "" && hppercent < 100 ) {
				//inputs.keyboard( character.recovery );
				actions.add( { 3, character.recovery } );
				healCooldown = timer.clock( ) + global;
				recoveryCooldown = timer.clock( ) + 63000;
			}
		}

		timer.sleep( 300 );
	}
}

void potionsThread(PARAMS) {

	int percent = ( ( 1012 / 2 ) - 188 ) - 32;
	double global = 1050;
	double potionCooldown = 0;
	int gen = control.getGeneration( client );

	while ( control.getRunning( client ) && control.getGeneration( client ) == gen ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );

		int hppercent = ( double ) player.health / percent * 100;
		int mppercent = ( double ) player.mana / percent * 100;

		if ( timer.clock( ) > potionCooldown ) {
			if ( character.healthpotion != "" && hppercent <= character.healthpotionpercent ) {
				//inputs.keyboard( character.healthpotion );
				actions.add( { 3, character.healthpotion } );
				potionCooldown = timer.clock( ) + global;
			} else if ( character.manapotion != "" && mppercent <= character.manapotionpercent ) {
				//inputs.keyboard( character.manapotion );
				actions.add( { 3, character.manapotion } );
				potionCooldown = timer.clock( ) + global;
			}
		}

		timer.sleep( 300 );
	}
}

void toolsThread(PARAMS) {

	double necklaceCooldown = 0, ringCooldown = 0, hasteCooldown = 0, ammo = 0;
	int gen = control.getGeneration( client );

	while ( control.getRunning( client ) && control.getGeneration( client ) == gen ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );

		if ( player.walking ) {
			if ( timer.clock( ) > hasteCooldown && character.haste != "" && !player.haste ) {
				inputs.keyboard( character.haste );
				hasteCooldown = timer.clock( ) + 33000;
			} else if ( character.food != "" && player.hungry ) {
				for ( int i = 0; i < 3; i++ ) inputs.keyboard( character.food );
			} else if ( character.ring != "" && player.ring == false ) {
				for ( int i = 0; i < 3; i++ ) inputs.keyboard( character.ring );
			} else if ( character.necklace != "" && player.necklace == false ) {
				for ( int i = 0; i < 3; i++ ) inputs.keyboard( character.necklace );
			}
			
			if ( timer.clock() > ammo && character.vocation == "paladin" && player.offhand ) {
				for ( int i = 0; i < 3; i++ ) inputs.keyboard( character.ammunition1 );
				ammo = timer.clock( ) + 30000;
			}
		}

		timer.sleep( 2000 );
	}
}

void knightThread(PARAMS) {

	bool keepAttacking;
	double global = 2100, challengeCooldown = 0, bloodrageCooldown = 0, attackCooldown = 0,
						  attackCooldown1 = 0, attackCooldown2 = 0, attackCooldown3 = 0,
						  attackCooldown4 = 0, attackCooldown5 = 0, attackCooldown6 = 0, 
						  attackCooldown7 = 0;
	int gen = control.getGeneration( client );

	while ( control.getRunning( client ) && control.getGeneration( client ) == gen ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );

		if ( player.attacking ) {

			if ( player.mana > 10 ) {
				if ( player.box >= 2 || keepAttacking ) {
					keepAttacking = true;
					if ( timer.clock( ) > attackCooldown ) {
						if ( character.bloodrage != "" && timer.clock( ) > bloodrageCooldown && player.box > 2 ) {
							inputs.keyboard( character.bloodrage );
							bloodrageCooldown = timer.clock( ) + 10500;
							attackCooldown = timer.clock( ) + 100;
						} else if ( character.challenge != "" && timer.clock( ) > challengeCooldown && player.box > 0 ) {
							inputs.keyboard( character.challenge );
							challengeCooldown = timer.clock( ) + 3150;
							attackCooldown = timer.clock( ) + 100;
						} else if ( character.annihilation != "" && timer.clock( ) > attackCooldown1 && player.target && player.box == 1 ) {
							inputs.keyboard( character.annihilation );
							attackCooldown = timer.clock( ) + global*2;
							attackCooldown1 = timer.clock( ) + 31500;
						} else if ( character.brutalstrike != "" && timer.clock( ) > attackCooldown2 && player.target && player.box == 1 ) {
							inputs.keyboard( character.brutalstrike );
							attackCooldown = timer.clock( ) + global;
							attackCooldown2 = timer.clock( ) + 6300;
						} else if ( character.whirlwindthrow != "" && timer.clock( ) > attackCooldown3 && player.target && player.box == 1 ) {
							inputs.keyboard( character.whirlwindthrow );
							attackCooldown = timer.clock( ) + global;
							attackCooldown3 = timer.clock( ) + 6300;
						} else if ( character.groundshaker != "" && timer.clock( ) > attackCooldown4 && player.box <= 3 && player.monsters.size( ) >= 5 ) {
							inputs.keyboard( character.groundshaker );
							attackCooldown = timer.clock( ) + global;
							attackCooldown4 = timer.clock( ) + 8400;
						} else if ( character.berserker != "" && timer.clock( ) > attackCooldown5 && player.box >= 2 ) {
							inputs.keyboard( character.berserker );
							attackCooldown = timer.clock( ) + global;
							attackCooldown5 = timer.clock( ) + 4200;
						} else if ( character.fierceberserker != "" && timer.clock( ) > attackCooldown6 && player.box >= 2 ) {
							inputs.keyboard( character.fierceberserker );
							attackCooldown = timer.clock( ) + global;
							attackCooldown6 = timer.clock( ) + 6300;
						} else if ( character.frontsweep != "" && player.side != "" && timer.clock( ) > attackCooldown7 && player.box >= 2 ) {
							string modifier = "control+";
							modifier.append( player.side );
							inputs.keywith( modifier.c_str( ) );
							timer.sleep( 100 );
							inputs.keyboard( character.frontsweep );
							attackCooldown = timer.clock( ) + global;
							attackCooldown7 = timer.clock( ) + 6300;
						}
					}
				}
			}
			timer.sleep( 100 );
		} else {
			keepAttacking = false;
			timer.sleep( 1000 );
		}

	}
}

void paladinThread(PARAMS) {

	bool keepAttacking, quiver = false;
	double global = 2100;
	double attackCooldown = 0, attackCooldown1 = 0, attackCooldown2 = 0,
		   attackCooldown3 = 0, attackCooldown4 = 0, attackCooldown5 = 0,
		   attackCooldown6 = 0, attackCooldown7 = 0;
	int gen = control.getGeneration( client );

	while ( control.getRunning( client ) && control.getGeneration( client ) == gen ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );

		if ( player.attacking ) {
			if ( player.box >= 3 || keepAttacking ) {

				keepAttacking = true;
				if ( timer.clock( ) > attackCooldown && player.mana > 10 ) {
					if ( character.sharpshooter != "" && timer.clock( ) > attackCooldown1 && player.box > 2 ) {
						inputs.keyboard( character.sharpshooter );
						attackCooldown = timer.clock( ) + 1000;
						attackCooldown1 = timer.clock( ) + 8400;
					} else if ( character.strongetherealspear != "" && timer.clock( ) > attackCooldown2 && player.target && player.box == 1 ) {
						inputs.keyboard( character.strongetherealspear );
						attackCooldown = timer.clock( ) + global;
						attackCooldown2 = timer.clock( ) + 8400;
					} else if ( character.etherealspear != "" && timer.clock( ) > attackCooldown3 && player.target && player.box == 1 ) {
						inputs.keyboard( character.etherealspear );
						attackCooldown = timer.clock( ) + global;
						attackCooldown3 = timer.clock( ) + 2100;
					} else if ( character.divinecaldera != "" && timer.clock( ) > attackCooldown4 && ( player.box >= 2 || player.box <= 3 && player.monsters.size( ) >= 5 ) ) {
						inputs.keyboard( character.divinecaldera );
						attackCooldown = timer.clock( ) + global;
						attackCooldown4 = timer.clock( ) + 4200;
					} else if ( character.arearune != "" && timer.clock( ) > attackCooldown5 && player.box >= 2 ) {
						inputs.keyboard( character.arearune );
						attackCooldown = timer.clock( ) + global;
						attackCooldown5 = timer.clock( ) + 2100;
					}
				}
			}
			timer.sleep( 100 );
		} else {
			keepAttacking = false;
			timer.sleep( 1000 );
		}

	}
}

void targetThread(PARAMS) {

	double autoAttackCooldown = 0;
	int gen = control.getGeneration( client );

	while ( control.getRunning( client ) && control.getGeneration( client ) == gen ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );

		if ( player.attacking ) {

			if ( timer.clock( ) > autoAttackCooldown && player.box >= 1 && !player.target ) {
				inputs.keyboard( "space" );
				autoAttackCooldown = timer.clock( ) + 1000;
			}
			
			timer.sleep( 200 );
		} else {
			timer.sleep( 1000 );
		}

	}
}

void alarmThread(PARAMS) {

	double criticalCooldown = 0, safeCooldown = 0;

	timer.sleep( 5000 );
	int gen = control.getGeneration( client );

	while ( control.getRunning( client ) && control.getGeneration( client ) == gen ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );

		if ( timer.clock( ) > criticalCooldown && ( player.alarm || player.mana <= 20 || player.health <= 20 ) )  {
			player.alarm = false;
			//control.setRunning( client, false );
			system( "mpg123 /opt/afk/critical.mp3 > /dev/null 2>&1" );
			criticalCooldown = timer.clock( ) + 60000;
		} else if ( timer.clock( ) > safeCooldown && player.lowcapacity ) {
			player.lowcapacity = false;
			system( "mpg123 /opt/afk/safe.mp3 > /dev/null 2>&1" );
			safeCooldown = timer.clock( ) + 60000;
		} else if ( player.walking && player.nearPlayer ) {
			int index = player.position;

			while ( true ) {
				if ( index == character.waypoints.size( ) ) index = 0;

				Waypoint waypoint = character.waypoints[ index ];
				if ( waypoint.action == "attack" ) {
					player.position = ( index + 1 ) % character.waypoints.size();
					break;
				}
				
				index++;
			}

			player.nearPlayer = false;
			system( "mpg123 /opt/afk/player.mp3 > /dev/null 2>&1" );
		}
		timer.sleep( 2000 );
	}

	player.lowcapacity = false;
	player.alarm = false;
}

void inputThread(PARAMS) {

    int gen = control.getGeneration( client );

    while ( control.getRunning( client ) && control.getGeneration( client ) == gen ) {

        while ( control.getPause( client ) ) timer.sleep( 2000 );

        try {
			if (!actions.isEmpty()) {
				Hotkey hotkey = actions.retrieve();
				if (!hotkey.combination.empty()) {
					inputs.keywith( hotkey.combination.c_str( ) );
					timer.sleep( 100 );	
				}
				inputs.keyboard( hotkey.key );
			}
        } catch (...) {
            console.log("Unknown exception caught in inputThread");
        }

        timer.sleep( 250 );
    }
}

void execute(int client, string spells, string waypoint) {
	if ( !found ) return;

	control.nextGeneration( client );
	auto tibia = ":" + to_string(client);

	console.log("Executing " + tibia + " " + spells + " " + waypoint);

    auto character = make_shared<Character>(spells, waypoint);
	auto screenshot = make_shared<Screenshot>(tibia);
	auto inputs = make_shared<Inputs>(tibia);
	auto player = make_shared<Player>(character->waypoints);
	auto actions = make_shared<Actions>();

	// detectar cantidad de vida segun pixeles internos de la barra de vida
	// si casi llega al spot de atacar y no hay monstruos que vaya al siguiente

	vector<function<void(int, Screenshot&, Inputs&, Player&, Character&, Actions&)>> functions = {
		locationThread,
		statsThread,
		healThread,
		potionsThread,
		toolsThread,
		inputThread
	};

	if ( character->waypoints.size( ) > 0 ) {
		functions.push_back(alarmThread);
		functions.push_back(alternatorThread);
		functions.push_back(targetThread);

		if ( character->vocation == "knight" && character->hunt == "box" ) {
			functions.push_back( knightThread );
		} else if ( character->vocation == "paladin" && character->hunt == "box" ) {
			functions.push_back( paladinThread );
		}
	}

	for ( auto& f : functions ) {
		int gen = control.getGeneration( client );
		thread([=]() {
			try {
				if ( control.getGeneration( client ) != gen ) return;
				f(client, *screenshot, *inputs, *player, *character, *actions);
			} catch (exception& e) {
				console.log("Error:", e.what());
			}
		}).detach();
	}
}

#include "interface.h"

int main(int argc, char *argv[]) {
 
	thread([&](){ procedure.load( ); }).detach();

	string serial = client.process( "lsblk --nodeps -no serial /dev/$(lsblk -ndo pkname $(df -h / | sed 1d | cut -d ' ' -f 1))" );
	console.log( serial );

	thread([=]() {
		try {
			while ( true ) {
				system( "rm -f /tmp/licence.json" );
				system( "wget -O /tmp/licence.json https://siewdass.github.io/licence.json > /dev/null 2>&1" );

				Json config;
				config.open( "/tmp/licence.json" );

				found = false;

				for ( Json entry: config.readArray( "accepted" ) ) {
					if ( entry.readString( "id" ) == serial ) {
						found = true;
						break;
					}
				}

				timer.sleep( 60000 * 60 * 6 );
			}
		} catch (exception& e) {
			console.log("Error:", e.what());
		}
	}).detach();

	interface(argc, argv);

	return 0;
}
