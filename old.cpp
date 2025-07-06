#include "screenshot.h"
#include "inputs.h"
#include "procedure.h"

Control control;

#define PARAMS int client, Screenshot& screenshot, Inputs& inputs, Player& player, Character& character, Actions& actions

void locationThread(PARAMS) {

    Mat FloorIndex, Minimap;
	double cooldown = 0;
	int oldX, oldY, count = 0;

	procedure.setWalkable( character.walkables );

	while ( control.getRunning( client ) ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );
		
		try {
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
			
			console.log("X: " + to_string(x) + " Y: " + to_string(y) + " Z: " + to_string(z));

		} catch (...) {
			console.log("Unknown exception caught in locationThread");
		}

		timer.sleep( 300 );
	}
}

void statsThread(PARAMS) {

	int width = 1012, middle = ( 1012 / 2 );
	int AlarmCount = 0;
	Mat HP, MP, BD, FK, Player, CHAT, LOOTCHAT, WEAPON, RING, AMULET, OFFHAND, CAPACITY, GameWindow, Monsters;

	while ( control.getRunning( client ) ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );

		try {
								
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

		} catch (...) {
			console.log("Unknown exception caught in statsThread");
		}

		timer.sleep( 300 );
	}
}

void alternatorThread(PARAMS) {
    int count = 0;
	bool avoid = false;

	while ( control.getRunning( client ) ) {

        while ( control.getPause( client ) ) timer.sleep( 2000 );

        try {
            if ( player.walking ) {

                if ( player.canWalk ) {
                    if ( player.box == player.validSqm || player.box == player.validSqm - 1 ) {
                        player.position = (player.position == character.waypoints.size()) ? 0 : player.position + 1;
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
						if ( !avoid ) timer.sleep( 2000 );
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

                //inputs.shiftDown( );
                for ( auto [ X, Y ] : procedure.looter( ) ) {
					if ( X > 0 && Y > 0 && X < 1012 && Y < 507 ) {
						//inputs.mouse( X, Y, 3 );
						inputs.shiftClick( X, Y, 3 );
						timer.sleep( 100 );
					}
                }
                //inputs.shiftUp( );

                player.looting = false;
                player.walking = true;

                for ( auto [ X, Y ] : procedure.throwFlask( player ) ) {
					if ( X > 0 && Y > 0 && X < 1012 && Y < 507 ) {
						inputs.mouseDrag( X, Y );
						timer.sleep( 100 );
					}
				}
            }
        } catch (...) {
            console.log("Unknown exception caught in alternatorThread");
        }

        timer.sleep( 250 );
    }
}

void healThread(PARAMS) {

    int percent = ( ( 1012 / 2 ) - 188 ) - 32;
    double global = 1050, healCooldown = 0, healCooldown1 = 0, recoveryCooldown = 0;

    while ( control.getRunning( client ) ) {

        while ( control.getPause( client ) ) timer.sleep( 2000 );

        try {
            int hppercent = ( double ) player.health / percent * 100;
            int mppercent = ( double ) player.mana / percent * 100;

            if ( timer.clock( ) > healCooldown && player.mana >= 5 ) {
                if ( timer.clock( ) > healCooldown1 && character.healing != "" && hppercent <= character.healingpercent ) {
					actions.add( { 3, character.healing } );
                    healCooldown = timer.clock( ) + global;
                    healCooldown1 = timer.clock( ) + global;
                } else if ( player.paralyze && character.paralyze != "" && player.walking ) {
					actions.add( { 3, character.paralyze } );
                    healCooldown = timer.clock( ) + global;
                } else if ( player.poison && character.curepoison != "" ) {
					actions.add( { 3, character.curepoison } );
                    healCooldown = timer.clock( ) + global;
                } else if ( timer.clock( ) > recoveryCooldown && character.recovery != "" && hppercent < 100 ) {
					actions.add( { 3, character.recovery } );
                    healCooldown = timer.clock( ) + global;
                    recoveryCooldown = timer.clock( ) + 63000;
                }
            }
        } catch (...) {
            console.log("Unknown exception caught in healThread");
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

        try {
            int hppercent = ( double ) player.health / percent * 100;
            int mppercent = ( double ) player.mana / percent * 100;

            if ( timer.clock( ) > potionCooldown ) {
                if ( character.healthpotion != "" && hppercent <= character.healthpotionpercent ) {
					actions.add( { 3, character.healthpotion } );
                    potionCooldown = timer.clock( ) + global;
                } else if ( character.manapotion != "" && mppercent <= character.manapotionpercent ) {
					actions.add( { 3, character.manapotion } );
                    potionCooldown = timer.clock( ) + global;
                }
            }
        } catch (...) {
            console.log("Unknown exception caught in potionsThread");
        }

        timer.sleep( 300 );
    }
}

void ammoThread(PARAMS) {
    bool quiver = false;
    double ammo = 0;

    while ( control.getRunning( client ) ) {

        while ( control.getPause( client ) ) timer.sleep( 2000 );

        try {
            // Uncomment and adjust as needed
            /*
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
            */

            if ( timer.clock() > ammo && (player.walking || player.looting) && player.offhand ) {
                console.log( "ammunition1" );
                inputs.keyboard( character.ammunition1 );
                inputs.keyboard( character.ammunition1 );
                inputs.keyboard( character.ammunition1 );
                ammo = timer.clock( ) + 30000;
            }
        } catch (...) {
            console.log("Unknown exception caught in ammoThread");
        }

        timer.sleep( 2000 );
    }
}

void toolsThread(PARAMS) {

    double necklaceCooldown = 0, ringCooldown = 0, foodCooldown = 0, hasteCooldown = 0;

    while ( control.getRunning( client ) ) {

        while ( control.getPause( client ) ) timer.sleep( 2000 );

        try {
            if ( player.walking ) {
                if ( timer.clock( ) > hasteCooldown && character.haste != "" && !player.haste ) {
                    inputs.keyboard( character.haste );
                    hasteCooldown = timer.clock( ) + 33000;
                } else if ( timer.clock( ) > foodCooldown && character.food != "" && player.hungry ) {
                    inputs.keyboard( character.food );
                    inputs.keyboard( character.food );
                    inputs.keyboard( character.food );
                    foodCooldown = timer.clock( ) + 60000;
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
        } catch (...) {
            console.log("Unknown exception caught in toolsThread");
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

		try {
			if ( player.attacking ) {

				if ( player.mana > 10 ) {
					if ( player.box >= 2 || keepAttacking ) {
						keepAttacking = true;
						if ( timer.clock( ) > attackCooldown ) {
							if ( character.bloodrage != "" && timer.clock( ) > bloodrageCooldown && player.box > 0 ) {
								actions.add( { 3, character.bloodrage } );
								bloodrageCooldown = timer.clock( ) + 10500;
								attackCooldown = timer.clock( ) + 100;
							} else if ( character.challenge != "" && timer.clock( ) > challengeCooldown && player.box > 0 ) {
								actions.add( { 3, character.challenge } );
								challengeCooldown = timer.clock( ) + 3150;
								attackCooldown = timer.clock( ) + 100;
							} else if ( character.annihilation != "" && timer.clock( ) > attackCooldown1 && player.target && player.box == 1 ) {
								actions.add( { 3, character.annihilation } );
								attackCooldown = timer.clock( ) + global*2;
								attackCooldown1 = timer.clock( ) + 31500;
							} else if ( character.brutalstrike != "" && timer.clock( ) > attackCooldown2 && player.target && player.box == 1 ) {
								actions.add( { 3, character.brutalstrike } );
								attackCooldown = timer.clock( ) + global;
								attackCooldown2 = timer.clock( ) + 6300;
							} else if ( character.whirlwindthrow != "" && timer.clock( ) > attackCooldown3 && player.target && player.box == 1 ) {
								actions.add( { 3, character.whirlwindthrow } );
								attackCooldown = timer.clock( ) + global;
								attackCooldown3 = timer.clock( ) + 6300;
							} else if ( character.groundshaker != "" && timer.clock( ) > attackCooldown4 && player.box <= 3 && player.monsters.size( ) >= 5 ) {
								actions.add( { 3, character.groundshaker } );
								attackCooldown = timer.clock( ) + global;
								attackCooldown4 = timer.clock( ) + 8400;
							} else if ( character.berserker != "" && timer.clock( ) > attackCooldown5 && player.box >= 2 ) {
								actions.add( { 3, character.berserker } );
								attackCooldown = timer.clock( ) + global;
								attackCooldown5 = timer.clock( ) + 4200;
							} else if ( character.fierceberserker != "" && timer.clock( ) > attackCooldown6 && player.box >= 2 ) {
								actions.add( { 3, character.fierceberserker } );
								attackCooldown = timer.clock( ) + global;
								attackCooldown6 = timer.clock( ) + 6300;
							} else if ( character.frontsweep != "" && player.side != "" && timer.clock( ) > attackCooldown7 && player.box >= 2 ) {
								string modifier = "control+";
								modifier.append( player.side );
								actions.add( { 3, character.frontsweep, modifier } );
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
		} catch (...) {
			console.log("Unknown exception caught in knightThread");
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

		try {
			if ( player.attacking ) {
				if ( player.box >= 3 || keepAttacking ) {

					keepAttacking = true;
					if ( timer.clock( ) > attackCooldown && player.mana > 10 ) {
						if ( character.sharpshooter != "" && timer.clock( ) > attackCooldown1 && player.box >= 2 ) {
							actions.add( { 3, character.sharpshooter } );
							attackCooldown = timer.clock( ) + 1000;
							attackCooldown1 = timer.clock( ) + 8400;
						} else if ( character.strongetherealspear != "" && timer.clock( ) > attackCooldown2 && player.target && player.box == 1 ) {
							actions.add( { 3, character.strongetherealspear } );
							attackCooldown = timer.clock( ) + global;
							attackCooldown2 = timer.clock( ) + 8400;
						} else if ( character.etherealspear != "" && timer.clock( ) > attackCooldown3 && player.target && player.box == 1 ) {
							actions.add( { 3, character.etherealspear } );
							attackCooldown = timer.clock( ) + global;
							attackCooldown3 = timer.clock( ) + 2100;
						} else if ( character.divinecaldera != "" && timer.clock( ) > attackCooldown4 && ( player.box >= 2 || player.box <= 3 && player.monsters.size( ) >= 5 ) ) {
							actions.add( { 3, character.divinecaldera } );
							attackCooldown = timer.clock( ) + global;
							attackCooldown4 = timer.clock( ) + 4200;
						} else if ( character.arearune != "" && timer.clock( ) > attackCooldown5 && player.box >= 2 ) {
							actions.add( { 3, character.arearune } );
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
		} catch (...) {
			console.log("Unknown exception caught in paladinThread");
		}
	}
}

void targetThread(PARAMS) {

	double autoAttackCooldown = 0;

	while ( control.getRunning( client ) ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );

		try {
			if ( player.attacking ) {

				if ( timer.clock( ) > autoAttackCooldown && player.box >= 1 && !player.target ) {
					inputs.keyboard( "space" );
					autoAttackCooldown = timer.clock( ) + 1000;
				}
				
				timer.sleep( 200 );
			} else {
				timer.sleep( 1000 );
			}
		} catch (...) {
			console.log("Unknown exception caught in targetThread");
		}
	}
}

void alarmThread(PARAMS) {

	double criticalCooldown = 0, safeCooldown = 0;

	timer.sleep( 5000 );

	while ( control.getRunning( client ) ) {

		while ( control.getPause( client ) ) timer.sleep( 2000 );

		try {
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
						player.position = index + 1;
						break;
					}
					
					index++;
				}

				player.nearPlayer = false;
				system( "mpg123 /opt/afk/player.mp3 > /dev/null 2>&1" );
			}
		} catch (...) {
			console.log("Unknown exception caught in alarmThread");
		}
		timer.sleep( 2000 );
	}

	player.lowcapacity = false;
	player.alarm = false;
}

void inputThread(PARAMS) {

    while ( control.getRunning( client ) ) {

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
	auto tibia = ":" + to_string(client);

	console.log("Executing " + tibia + " " + spells + " " + waypoint);

    auto character = make_shared<Character>(spells, waypoint);
	auto screenshot = make_shared<Screenshot>(tibia);
	auto inputs = make_shared<Inputs>(tibia);
	auto player = make_shared<Player>(character->waypoints);
	auto actions = make_shared<Actions>();

	// verificar si el spot donde va a atacar tiene para hacer box si no que elija otro hasta que ya consiga uno
	// detectar cantidad de vida segun pixeles internos de la barra de vida
	// si casi llega al spot de atacar y no hay monstruos que vaya al siguiente
	// corregir el pause lado alarma o quitarlo mientras

	vector<function<void(int, Screenshot&, Inputs&, Player&, Character&, Actions&)>> functions = {
		inputThread,
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

	system( "rm -f /tmp/licence.json" );
	system( "wget -O /tmp/licence.json https://siewdass.github.io/licence.json > /dev/null 2>&1" );

	Json config;
	config.open( "/tmp/licence.json" );

	for ( Json entry: config.readArray( "accepted" ) ) {
		if ( entry.readString( "id" ) == serial ) {
			interface(argc, argv);
			break;
		}
	}

	console.log( serial );

	//interface(argc, argv);

	return 0;
}