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
string name;

void updateThread( ) {

	while ( true ) {

		if ( Pause ) {

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
				ring,
				lowcapacity
			] = status.update( script.positions );

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
			values.alarm = alarm;
			values.weapon = weapon;
			values.necklace = necklace;
			values.ring = ring;
			values.lowcapacity = lowcapacity;

			time.sleep( 200 );

		} else {
			time.sleep( 2000 );
		}

	}
}

void healThread( ) {

	double global = 1050;
	double healCooldown = 0,
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

		} else {
  		time.sleep( 2000 );
		}
	}
}

void potionsThread( ) {

	double global = 1050;
	double potionCooldown = 0;

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

			} catch ( const std::exception& e ) {
				console.log( "ERROR:", e.what( ) );
			}

			time.sleep( 300 );

		} else {
  		time.sleep( 2000 );
		}
	}
}

void run( ) {

	int id = client.get( );
	status.id = id;
	viewport.id = id;

	string file = "/home/" +user+ "/afk/" +name+ ".json";

	console.log( file );

  bool exists = std::filesystem::exists( file.c_str( ) );

	if ( !exists ) { return; }

	character.load( file.c_str( ) );

	if ( id != 0 ) {
		thread( updateThread ).detach( );
		//thread( healThread ).detach( );
		//thread( potionsThread ).detach( );
	}

}

int main( int argc, char** argv ) {

	system( "rm -f /tmp/licence.json" );
	system( "wget -O /tmp/licence.json https://siewdass.github.io/licence.json > /dev/null 2>&1" );
	
	licence.load( "/tmp/licence.json" );

	id = argv[ 1 ];
	user = argv[ 2 ];
	name = argv[ 3 ];

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