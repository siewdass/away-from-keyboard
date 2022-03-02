#include "path.h"
#include <vector>

using namespace std;

Array < Point > Path::find( vector < vector < int > > map, Point from, Point to ) {
	pf::Astar astar;
	astar.loadMap( map );
	astar.setDiagonalMovement( true );
	return astar.findPath( from, to );
}