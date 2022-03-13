#include "status.h"
#include "utils.h"
#include "screenshot.h"
#include "acknow.h"
#include "converter.h"
#include <vector>
#include <iostream>

using namespace std;

Screenshot screenshot;
Converter converter;
Acknow acknow;
Client client;

Mat LOOTCHAT;
vector < Pixel > lootchat;


void test( ) {	
	bool valuableLoot = false;

	LOOTCHAT = screenshot.get( id, 509, 575, 305, 42 );
    lootchat = acknow.inspect( LOOTCHAT );

    for ( Pixel pixel : lootchat ) {
        int r = pixel.r, g = pixel.g, b = pixel.b;
		console.log( r, g, b);
        if ( r == 0 && g == 240 && b == 0 || r == 32 && g == 160 && b == 255 || r == 255 && g == 104 && b == 255 || r == 240 && g == 240 && 0 ) {
            valuableLoot = true;
            break;
        }
    }

}

int main( ) {

	// cambiar Point por un nuevo struct y 
	// detectar cantidad de vida segun pixeles
	// internos de la barra de vida
	// autoattack siempre al que tenga mas vida
	// si queda trap que mate
	// if ( !x ) { return } 
	// exori min = minimo 2

	int id = client.get( );

	if ( id != 0 ) {
		thread t1 ( test );
		t1.join( );
	}

	return 0;
}