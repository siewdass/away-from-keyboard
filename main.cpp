#include "screenshot.h"
#include "inputs.h"
#include "procedure.h"

Control control;

#define PARAMS int client, Screenshot& screenshot, Inputs& inputs, Player& player, Character& character

void locationThread(PARAMS) {

    Mat FloorIndex, Minimap;
	double cooldown = 0;
	int oldX, oldY, count = 0;

	while ( control.getRunning( client ) ) {

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
	Mat HP, MP, BD, FK, Player, CHAT, LOOTCHAT, WEAPON, RING, AMULET, OFFHAND, CAPACITY, GameWindow, Monsters;

	while ( control.getRunning( client ) ) {

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

		//imwrite( "screenshot.png", OFFHAND );

		auto [ health, mana, hungry, paralyze, poison, haste, flasks, alarm, near, weapon, necklace, ring, lowcapacity, target, offhand ] = procedure.stats( OFFHAND, CAPACITY, WEAPON, AMULET, RING, Player, CHAT, HP, MP, BD, FK, Monsters, AlarmCount);

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

	while ( control.getRunning( client ) ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );

		if ( player.walking ) {

			if ( player.canWalk ) {
				if ( player.box == player.validSqm || player.box == player.validSqm - 1 ) {
					player.position = (player.position == character.waypoints.size()) ? 0 : player.position + 1;
					player.attacking = true;
					player.walking = false;
				} else {
					auto [ attacking, X, Y ] = procedure.walker( player, character, procedure.paths );
					if ( !attacking && X != 0 && Y != 0 ) {
						inputs.mouse( X, Y, 1 );
					} else if ( attacking ) {
						player.attacking = true;
						player.walking = false;
					}
					timer.sleep( 2000 );
				}
			}

		} else if ( player.attacking ) {		

			if ( player.box == 0 ) count++;
			else count = 0;

			player.looting = count == 2;
			if ( player.looting ) player.attacking = false;

		} else if ( player.looting ) {

			inputs.shiftDown( );
			for ( auto [ X, Y ] : procedure.looter( ) ) {
				inputs.mouse( X, Y, 3 );
				timer.sleep( 130 );
			}
			inputs.shiftUp( );

			player.looting = false;
			player.walking = true;

			for ( auto [ flaskX, flaskY ] : procedure.throwFlask( player ) ) inputs.mouseDrag( flaskX, flaskY );

		}

		timer.sleep( 250 );
	}
}

void healThread(PARAMS) {

	int percent = ( ( 1012 / 2 ) - 188 ) - 32;
	double global = 1050, healCooldown = 0, healCooldown1 = 0, recoveryCooldown = 0;

	while ( control.getRunning( client ) ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );

		int hppercent = ( double ) player.health / percent * 100;
		int mppercent = ( double ) player.mana / percent * 100;

		if ( timer.clock( ) > healCooldown && player.mana >= 5 ) {
			if ( timer.clock( ) > healCooldown1 && character.healing != "" && hppercent <= character.healingpercent ) {
				inputs.keyboard( character.healing );
				healCooldown = timer.clock( ) + global;
				healCooldown1 = timer.clock( ) + global;
			} else if ( player.paralyze && character.paralyze != "" && player.walking ) {
				inputs.keyboard( character.paralyze );
				healCooldown = timer.clock( ) + global;
			} else if ( player.poison && character.curepoison != "" ) {
				inputs.keyboard( character.curepoison );
				healCooldown = timer.clock( ) + global;
			} else if ( timer.clock( ) > recoveryCooldown && character.recovery != "" && hppercent < 100 ) {
				inputs.keyboard( character.recovery );
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

	while ( control.getRunning( client ) ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );

		int hppercent = ( double ) player.health / percent * 100;
		int mppercent = ( double ) player.mana / percent * 100;

		if ( timer.clock( ) > potionCooldown ) {
			if ( character.healthpotion != "" && hppercent <= character.healthpotionpercent ) {
				inputs.keyboard( character.healthpotion );
				potionCooldown = timer.clock( ) + global;
			} else if ( character.manapotion != "" && mppercent <= character.manapotionpercent ) {
				inputs.keyboard( character.manapotion );
				potionCooldown = timer.clock( ) + global;
			}
		}

		timer.sleep( 300 );
	}
}

void ammoThread(PARAMS) {
	bool quiver = false;
	double ammo = 0;

	while ( control.getRunning( client ) ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );

		if ( character.quiver1 != "" && character.quiver2 != "" && character.ammunition1 != "" && character.ammunition2 != "" && character.hunt == "box" ) {
			if ( player.offhand == false ) {
				inputs.keyboard( character.quiver1 );
				timer.sleep( 1000 );
			}

			if ( player.attacking && player.box >= 1 && player.box <= 3 && player.monsters.size() <= 3 ) {
				console.log( "quiver2" );
				inputs.keyboard( character.quiver2 );
				timer.sleep( 1000 );
				while ( player.monsters.size() != 0 ) timer.sleep( 500 );
				console.log( "quiver1" );
				inputs.keyboard( character.quiver1 );
				timer.sleep( 1000 );
			}
		}

		if ( timer.clock() > ammo && (player.walking || player.looting) && player.offhand ) {
			console.log( "ammunition1"	);
			inputs.keyboard( character.ammunition1 );
			inputs.keyboard( character.ammunition1 );
			inputs.keyboard( character.ammunition1 );
			ammo = timer.clock( ) + 60000;
		}


		timer.sleep( 500 );
	}
}

void toolsThread(PARAMS) {

	double necklaceCooldown = 0, ringCooldown = 0, foodCooldown = 0, hasteCooldown = 0;

	while ( control.getRunning( client ) ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );

		if ( player.walking ) {
			if ( timer.clock( ) > hasteCooldown && character.haste != "" && !player.haste ) {
				inputs.keyboard( character.haste );
				hasteCooldown = timer.clock( ) + 33000;
			} else if ( timer.clock( ) > foodCooldown && character.food != "" && player.hungry ) {
				inputs.keyboard( character.food );
				inputs.keyboard( character.food );
				inputs.keyboard( character.food );
				foodCooldown = timer.clock( ) + 180000;
			} else if ( character.ring != "" && player.ring == false ) {
				inputs.keyboard( character.ring );
				inputs.keyboard( character.ring );
				inputs.keyboard( character.ring );
			} else if ( character.necklace != "" && player.necklace == false ) {
				inputs.keyboard( character.necklace );
				inputs.keyboard( character.necklace );
				inputs.keyboard( character.necklace );
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

	while ( control.getRunning( client ) ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );

		if ( player.attacking ) {

			if ( player.mana > 10 ) {
				if ( player.box >= 2 || keepAttacking ) {
					keepAttacking = true;
					if ( timer.clock( ) > attackCooldown ) {
						if ( character.bloodrage != "" && timer.clock( ) > bloodrageCooldown && player.box > 0 ) {
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

	while ( control.getRunning( client ) ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );

		if ( player.attacking ) {
			if ( player.box >= 3 || keepAttacking ) {

				keepAttacking = true;
				if ( timer.clock( ) > attackCooldown && player.mana > 10 ) {
					if ( character.sharpshooter != "" && timer.clock( ) > attackCooldown1 && player.box >= 2 ) {
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

	while ( control.getRunning( client ) ) {

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

	while ( control.getRunning( client ) ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );

		if ( timer.clock( ) > criticalCooldown && ( player.alarm || player.mana <= 20 || player.health <= 20 ) )  {
			player.alarm = false;
			control.setRunning( client, false );
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
					player.position = index + 1;
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

void execute(int client, string spells, string waypoint) {
	auto tibia = ":" + to_string(client);

	console.log("Executing " + tibia + " " + spells + " " + waypoint);

    auto character = make_shared<Character>(spells, waypoint);
	auto screenshot = make_shared<Screenshot>(tibia);
	auto inputs = make_shared<Inputs>(tibia);
	auto player = make_shared<Player>(character->waypoints);

	// verificar si el spot donde va a atacar tiene para hacer box si no que elija otro hasta que ya consiga uno
	// detectar cantidad de vida segun pixeles internos de la barra de vida
	// si casi llega al spot de atacar y no hay monstruos que vaya al siguiente

	vector<function<void(int, Screenshot&, Inputs&, Player&, Character&)>> functions = {
		locationThread,
		statsThread,
		healThread,
		potionsThread,
		toolsThread
	};

	if ( character->waypoints.size( ) > 0 ) {
		functions.push_back(alarmThread);
		functions.push_back(alternatorThread);
		functions.push_back(targetThread);

		if ( character->vocation == "knight" && character->hunt == "box" ) {
			functions.push_back( knightThread );
		} else if ( character->vocation == "paladin" && character->hunt == "box" ) {
			functions.push_back( paladinThread );
			functions.push_back( ammoThread );
		}
	}

	for ( auto& f : functions ) {
		thread([=]() { // Knight
			try {
				f(client, *screenshot, *inputs, *player, *character);
			} catch (exception& e) {
				console.log("Error:", e.what());
			}
		}).detach();
	}
}

#include "interface.h"

int main(int argc, char *argv[]) {
 
  thread([&](){ procedure.load( ); }).detach();
 
  interface(argc, argv);

  return 0;
}