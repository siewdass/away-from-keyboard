#include "screenshot.h"
#include "inputs.h"

#include "loops.h"

vector<shared_ptr<atomic<bool>>> runnings(6, make_shared<atomic<bool>>(true));

void execute(int client, string CHARACTER, string waypoint) {
  string tibia = ":" + to_string(client);

  console.log("Executing " + tibia + " " + CHARACTER + " " + waypoint);

  Character character(CHARACTER, waypoint);

  auto screenshot = make_shared<Screenshot>(tibia);
  auto inputs = make_shared<Inputs>(tibia);
  auto values = make_shared<Values>(character.waypoints);
  auto& running = runnings[client];

  thread([running, screenshot, inputs, values]() {
    try {
      updateLocation(*running, *screenshot, *inputs, *values);
		} catch ( const std::exception& e ) {
			cerr << "Error: " << e.what() << endl;
		}
  }).detach();

  thread([running, screenshot, inputs, values]() {
    try {
      updateThread(*running, *screenshot, *inputs, *values);
		} catch ( const std::exception& e ) {
			cerr << "Error: " << e.what() << endl;
		}
  }).detach();

  //timer.sleep( 10000 );
  //running->store(false);

	/*thread( healThread ).detach( );
	thread( potionsThread ).detach( );
	thread( toolsThread ).detach( );

	thread( networkThread ).detach( );

	if ( character.waypoints.size( ) > 0 ) {
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

#include "gui.h"

int main(int argc, char *argv[]) {
 
  thread( [&]( ) { location.load( ); }).detach( );
 
  interface(argc, argv);

  return 0;
}