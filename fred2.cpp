#include "iostream"
#include "utils.h"
#include <vector>
#include "lootPoint.h"
using namespace std;


int main()
{
	vector<lootPoint> lootPoints;
	lootPoint p = lootPoint( );
	for ( int i = 0; i < 1; i++ ) {
	p.add( 1, i );
	lootPoints.push_back( p );

	}
	int numero = 7;
	for ( int i = 0; i < lootPoints.size(); i++ ) {
		if ( lootPoints[i].y == numero ) {
			lootPoints.erase(lootPoints.begin()+i);
		}
	}
	console.log( lootPoints.size() );
	console.log( lootPoints[1].x, lootPoints[1].y );


}