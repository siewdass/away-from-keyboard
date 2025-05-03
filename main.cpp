#include "screenshot.h"
#include "inputs.h"
#include "procedure.h"

vector<shared_ptr<atomic<bool>>> runnings(6, make_shared<atomic<bool>>(false));
vector<shared_ptr<atomic<bool>>> pauses(6, make_shared<atomic<bool>>(false));

void locationThread(atomic<bool>& running, atomic<bool>& pause, Screenshot& screenshot, Inputs& inputs, Player& player) {

    Mat FloorIndex, Minimap;
	double cooldown = 0;
	int oldX, oldY, count = 0;

	while ( running ) {

		while ( pause ) timer.sleep( 2000 );
		
		//auto [ XPOS, YPOS ] = inputs.mouseLocation( );		
		//if ( XPOS < 190 || XPOS > 824 || YPOS < 42 || YPOS > 507 ) { inputs.mouseMove( 742, 596 ); }

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

void statsThread(atomic<bool>& running, atomic<bool>& pause, Screenshot& screenshot, Inputs& inputs, Player& player) {

	int width = 1012, middle = ( 1012 / 2 );
	int AlarmCount = 0;
	Mat HP, MP, BD, FK, Player, CHAT, LOOTCHAT, WEAPON, RING, AMULET, CAPACITY, GameWindow, Monsters;

	while ( running ) {

		while ( pause ) timer.sleep( 2000 );

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
		//imwrite( "screenshot.png", GameWindow );

		auto [ health, mana, hungry, paralyze, poison, haste, flasks, alarm, near, weapon, necklace, ring, lowcapacity, target ] = procedure.stats( CAPACITY, WEAPON, AMULET, RING, Player, CHAT, HP, MP, BD, FK, Monsters, AlarmCount);

		player.health = health;
		player.mana = mana;
		player.hungry = hungry;
		player.paralyze = paralyze;
		player.poison = poison;
		player.haste = haste;
		player.flasks = flasks;
		player.alarm = alarm;
		player.weapon = weapon;
		player.necklace = necklace;
		player.ring = ring;
		player.lowcapacity = lowcapacity;
		player.target = target;
		player.nearPlayer = near;

		//console.log( "hp:", to_string( health ), "mana:", to_string( mana ) );

		timer.sleep( 300 );
	}
}

void alternatorThread(atomic<bool>& running, atomic<bool>& pause, Mode& mode, Inputs& inputs, Player& player, Character& character, atomic<int>& position) {
    
	int count = 0;

	while ( running ) {

		while ( pause ) timer.sleep( 2000 );

		if ( mode.walking ) {

			if ( player.canWalk ) {
				if ( player.box == player.validSqm || player.box == player.validSqm - 1 ) {
					position = (position == character.waypoints.size()) ? 0 : position + 1;
					mode.attacking = true;
					mode.walking = false;
				} else {
					auto [ attacking, X, Y ] = procedure.walker( player, character, procedure.paths, position );
					if ( !attacking && X != 0 && Y != 0 ) {
						inputs.mouse( X, Y, 1 );
					} else if ( attacking ) {
						mode.attacking = true;
						mode.walking = false;
					}
					timer.sleep( 2000 );
				}
			}

		} else if ( mode.attacking ) {		

			if ( player.box == 0 ) count++;
			else count = 0;

			mode.looting = count == 3;
			if ( mode.looting ) mode.attacking = false;

		} else if ( mode.looting ) {

			inputs.shiftDown( );
			for ( auto [ X, Y ] : procedure.looter( ) ) {
				inputs.mouse( X, Y, 3 );
				timer.sleep( 130 );
			}
			inputs.shiftUp( );

			mode.looting = false;
			mode.walking = true;

			for ( auto [ flaskX, flaskY ] : procedure.throwFlask( player ) ) inputs.mouseDrag( flaskX, flaskY );

		}

		timer.sleep( 250 );
	}
}

void healThread(atomic<bool>& running, atomic<bool>& pause, Mode& mode, Inputs& inputs, Player& player, Character& character) {

	int percent = ( ( 1012 / 2 ) - 188 ) - 32;
	double global = 1050, healCooldown = 0, healCooldown1 = 0, recoveryCooldown = 0;

	while ( running ) {

		while ( pause ) timer.sleep( 2000 );

		int hppercent = ( double ) player.health / percent * 100;
		int mppercent = ( double ) player.mana / percent * 100;

		if ( timer.clock( ) > healCooldown && player.mana >= 5 ) {
			if ( timer.clock( ) > healCooldown1 && character.healing != "" && hppercent <= character.healingpercent ) {
				inputs.keyboard( character.healing );
				healCooldown = timer.clock( ) + global;
				healCooldown1 = timer.clock( ) + global;
			} else if ( player.paralyze && character.paralyze != "" && mode.walking ) {
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

void potionsThread(atomic<bool>& running, atomic<bool>& pause, Inputs& inputs, Player& player, Character& character) {

	int percent = ( ( 1012 / 2 ) - 188 ) - 32;
	double global = 1050;
	double potionCooldown = 0;

	while ( running ) {
		
		while ( pause ) timer.sleep( 2000 );

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

void ammoThread(atomic<bool>& running, atomic<bool>& pause, Mode& mode, Inputs& inputs, Player& player, Character& character) {

	double quiver = 0, throwing = 0, change = 0;
	bool changed = false;

	while ( running ) {

		while ( pause ) timer.sleep( 2000 );

		if ( false && timer.clock( ) > change && mode.attacking && character.quiver1 != "" && character.quiver2 != "" && character.hunt == "box" && player.monsters.size() >= 4 ) {
			changed = false;
			while ( mode.attacking ) {
				if ( player.monsters.size() <= 3 ) {
					changed = true;
					inputs.keyboard( character.quiver2 );
				}
				timer.sleep( 250 );
			}
			inputs.keyboard( character.quiver1 );
		}

		if ( timer.clock( ) > quiver && mode.walking && character.ammunition != "" ) {
			inputs.keyboard( character.ammunition );
			inputs.keyboard( character.ammunition );
			inputs.keyboard( character.ammunition );
			quiver = timer.clock( ) + 60000;
		} else if ( player.weapon && character.throwing != "" ) {
			inputs.keyboard( character.throwing );
		}


		timer.sleep( 2000 );
	}
}

void toolsThread(atomic<bool>& running, atomic<bool>& pause, Mode& mode, Inputs& inputs, Player& player, Character& character) {

	double necklaceCooldown = 0, ringCooldown = 0, foodCooldown = 0, hasteCooldown = 0;

	while ( running ) {

		while ( pause ) timer.sleep( 2000 );

		if ( mode.walking ) {
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

void knightThread(atomic<bool>& running, atomic<bool>& pause, Mode& mode, Inputs& inputs, Player& player, Character& character) {

	bool keepAttacking;
	double global = 2100, challengeCooldown = 0, bloodrageCooldown = 0, attackCooldown = 0,
						  attackCooldown1 = 0, attackCooldown2 = 0, attackCooldown3 = 0,
						  attackCooldown4 = 0, attackCooldown5 = 0, attackCooldown6 = 0, 
						  attackCooldown7 = 0;

	while ( running ) {

		while ( pause ) timer.sleep( 2000 );

		if ( mode.attacking ) {

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

void paladinThread(atomic<bool>& running, atomic<bool>& pause, Mode& mode, Inputs& inputs, Player& player, Character& character) {

	bool keepAttacking;
	double global = 2100;
	double attackCooldown = 0, attackCooldown1 = 0, attackCooldown2 = 0,
		   attackCooldown3 = 0, attackCooldown4 = 0, attackCooldown5 = 0,
		   attackCooldown6 = 0, attackCooldown7 = 0;

	while ( running ) {

		while ( pause ) timer.sleep( 2000 );

		if ( mode.attacking ) {
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

void targetThread(atomic<bool>& running, atomic<bool>& pause, Mode& mode, Inputs& inputs, Player& player) {

	double autoAttackCooldown = 0;

	while ( running ) {

		while ( pause ) timer.sleep( 2000 );

		if ( mode.attacking ) {

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

void alarmThread(atomic<bool>& running, atomic<bool>& pause, Mode& mode, Inputs& inputs, Player& player, Character& character, atomic<int>& position) {

	double criticalCooldown = 0, safeCooldown = 0;

	while ( running ) {

		while ( pause ) timer.sleep( 2000 );

		if ( timer.clock( ) > criticalCooldown && ( player.alarm || player.mana <= 20 || player.health <= 20 ) )  {
			player.alarm = false;
			running = false;
			system( "mpg123 /opt/afk/critical.mp3 > /dev/null 2>&1" );
			criticalCooldown = timer.clock( ) + 60000;
		} else if ( timer.clock( ) > safeCooldown && player.lowcapacity ) {
			player.lowcapacity = false;
			system( "mpg123 /opt/afk/safe.mp3 > /dev/null 2>&1" );
			safeCooldown = timer.clock( ) + 60000;
		} else if ( mode.walking && player.nearPlayer ) {
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

			player.nearPlayer = false;
			system( "mpg123 /opt/afk/player.mp3 > /dev/null 2>&1" );
		}
		timer.sleep( 2000 );
	}

	player.lowcapacity = false;
	player.alarm = false;
}

void execute(int client, string CHARACTER, string waypoint) {
	string tibia = ":" + to_string(client);

	console.log("Executing " + tibia + " " + CHARACTER + " " + waypoint);

    auto character = make_shared<Character>(CHARACTER, waypoint);
    auto mode = make_shared<Mode>();
	auto screenshot = make_shared<Screenshot>(tibia);
	auto inputs = make_shared<Inputs>(tibia);
	auto player = make_shared<Player>(character->waypoints);
	auto& running = runnings[client];
	auto& pause = pauses[client];
	auto position = make_shared<atomic<int>>(0);
	
	// global CDS aqui en esta funcion?
	// verificar si el spot donde va a atacar tiene para hacer box si no que elija otro hasta que ya consiga uno
	// crear una clase para knight otra para rp y asi
	// detectar cantidad de vida segun pixeles internos de la barra de vida
	// knight pegarle al que tiene mas vida
	// paladin pegarle a los de la cruz
	// mode entra dentro de player
	// agregar flowcontrol comom running stop open pause

	thread([=]() { // Location
		try {
			locationThread(*running, *pause, *screenshot, *inputs, *player);
		} catch (exception& e) {
			console.log("Location error:", e.what());
		}
	}).detach();
	
	thread([=]() { // Stats
		try {
			statsThread(*running, *pause, *screenshot, *inputs, *player);
		} catch (exception& e) {
			console.log("Stats error:", e.what());
		}
	}).detach();
	
    thread([=]() { // Heal
        try {
            healThread(*running, *pause, *mode, *inputs, *player, *character);
        } catch (exception& e) {
            console.log("Heal error:", e.what());
        }
    }).detach();

    thread([=]() { // Potion
        try {
            potionsThread(*running, *pause, *inputs, *player, *character);
        } catch (exception& e) {
            console.log("Potion error:", e.what());
        }
    }).detach();
	
   	thread([=]() { // Tool
        try {
            toolsThread(*running, *pause, *mode, *inputs, *player, *character);
        } catch (exception& e) {
            cerr << "Tool Error: " << e.what() << endl;
        }
    }).detach();

	if ( character->waypoints.size( ) > 0 ) {

		thread([=]() { // Alarm
			try {
				alarmThread(*running, *pause, *mode, *inputs, *player, *character, *position);
			} catch (exception& e) {
				console.log("Alarm error:", e.what());
			}
		}).detach();

		thread([=]() { // Alternator
			try {
				alternatorThread(*running, *pause, *mode, *inputs, *player, *character, *position);
			} catch (exception& e) {
				console.log("Alternator error:", e.what());
			}
		}).detach();

		thread([=]() { // Target
			try {
				targetThread(*running, *pause, *mode, *inputs, *player);
			} catch (exception& e) {
				console.log("Target error:", e.what());
			}
		}).detach();
		
		if ( character->vocation == "knight" && character->hunt == "box" ) {

			thread([=]() { // Knight
				try {
					knightThread(*running, *pause, *mode, *inputs, *player, *character);
				} catch (exception& e) {
					console.log("Knight error:", e.what());
				}
			}).detach();

		} else if ( character->vocation == "paladin" && character->hunt == "box" ) {
			
			thread([=]() { // Paladin
				try {
					paladinThread(*running, *pause, *mode, *inputs, *player, *character);
				} catch (exception& e) {
					console.log("Paladin error:", e.what());
				}
			}).detach();

			thread([=]() { // Ammo
				try {
					ammoThread(*running, *pause, *mode, *inputs, *player, *character);
				} catch (exception& e) {
					console.log("Ammo error:", e.what());
				}
			}).detach();
		}
	}
}

#include "interface.h"

int main(int argc, char *argv[]) {
 
  thread([&](){ procedure.load( ); }).detach();
 
  interface(argc, argv);

  return 0;
}