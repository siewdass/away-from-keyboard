#include "utils.h"

using namespace std::chrono;

void Time::sleep( int ms ) {
	std::this_thread::sleep_for( milliseconds( ms ) );
}

double Time::clock( ) {
	return duration_cast < milliseconds > ( 
		time_point_cast < milliseconds > ( 
			high_resolution_clock::now( ) 
		).time_since_epoch( ) 
	).count( );
}