#include "screenshot.h"
#include "inputs.h"
#include "loops.h"

vector<shared_ptr<atomic<bool>>> runnings(6, make_shared<atomic<bool>>(false));

void execute(int client, string CHARACTER, string waypoint) {
	string tibia = ":" + to_string(client);

	console.log("Executing " + tibia + " " + CHARACTER + " " + waypoint);

    auto character = make_shared<Character>(CHARACTER, waypoint);
    auto mode = make_shared<Mode>();
	auto screenshot = make_shared<Screenshot>(tibia);
	auto inputs = make_shared<Inputs>(tibia);
	auto player = make_shared<Player>(character->waypoints);
	auto& running = runnings[client];
	auto position = make_shared<atomic<int>>(0);
	// global CDS
	
	// pasar todo a un solo utils
	// una sola clase que se llame retrieve que tenga todo lo que se procesa del
	// tibia y que retorne solamente valores para ejecutar
	// seria la que tiene opencv
	// targetThread que solo ataque cuando tenga al menos 3 en box para rp?
	// verificar si el spot donde va a atacar tiene para hacer box si no que elija otro hasta que ya consiga uno
	// astar pasa a ser pathfinder

	// crear una clase para knight otra para rp y asi
	// detectar cantidad de vida segun pixeles internos de la barra de vida
	// knight pegarle al que tiene mas vida
	// paladin pegarle a los de la cruz

	thread([=]() { // Location
		try {
			locationThread(*running, *screenshot, *inputs, *player);
		} catch (exception& e) {
			console.log("Location error:", e.what());
		}
	}).detach();
	
	thread([=]() { // Stats
		try {
			statsThread(*running, *screenshot, *inputs, *player);
		} catch (exception& e) {
			console.log("Stats error:", e.what());
		}
	}).detach();
	
    thread([=]() { // Heal
        try {
            healThread(*running, *mode, *inputs, *player, *character);
        } catch (exception& e) {
            console.log("Heal error:", e.what());
        }
    }).detach();

    thread([=]() { // Potion
        try {
            potionsThread(*running, *inputs, *player, *character);
        } catch (exception& e) {
            console.log("Potion error:", e.what());
        }
    }).detach();
	
   	thread([=]() { // Tool
        try {
            toolsThread(*running, *mode, *inputs, *player, *character);
        } catch (exception& e) {
            cerr << "Tool Error: " << e.what() << endl;
        }
    }).detach();

	if ( character->waypoints.size( ) > 0 ) {

		thread([=]() { // Alarm
			try {
				alarmThread(*running, *mode, *inputs, *player, *character, *position);
			} catch (exception& e) {
				console.log("Alarm error:", e.what());
			}
		}).detach();

		thread([=]() { // Alternator
			try {
				alternatorThread(*running, *mode, *inputs, *player, *character, *position);
			} catch (exception& e) {
				console.log("Alternator error:", e.what());
			}
		}).detach();

		thread([=]() { // Target
			try {
				targetThread(*running, *mode, *inputs, *player);
			} catch (exception& e) {
				console.log("Target error:", e.what());
			}
		}).detach();
		
		if ( character->vocation == "knight" && character->hunt == "box" ) {

			thread([=]() { // Knight
				try {
					knightThread(*running, *mode, *inputs, *player, *character);
				} catch (exception& e) {
					console.log("Knight error:", e.what());
				}
			}).detach();

		} else if ( character->vocation == "paladin" && character->hunt == "box" ) {
			
			thread([=]() { // Paladin
				try {
					paladinThread(*running, *mode, *inputs, *player, *character);
				} catch (exception& e) {
					console.log("Paladin error:", e.what());
				}
			}).detach();

			thread([=]() { // Ammo
				try {
					ammoThread(*running, *mode, *inputs, *player, *character);
				} catch (exception& e) {
					console.log("Ammo error:", e.what());
				}
			}).detach();
		}
	}
}

#include "gui.h"

int main(int argc, char *argv[]) {
 
  thread([&](){ location.load( ); }).detach();
 
  interface(argc, argv);

  return 0;
}