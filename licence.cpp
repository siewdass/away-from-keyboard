#include "licence.h"
#include "utils.h"

void Licence::load( string file ) {
	Json config;
	config.open( file.c_str( ) );

	for ( Json entry: config.readArray( "accepted" ) ) {
		accepted.push_back( entry.readString( "id" ) );
	}
}
